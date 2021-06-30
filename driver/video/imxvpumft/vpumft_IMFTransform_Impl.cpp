/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "vpumft.h"
#include "CAutoLock.h"
#include <mferror.h>
#include <mfapi.h>
#include <tracing.h>

#undef FAILED
#define FAILED(hr) MyFailed(hr)

/****************************
******* IMFTransform ********
****************************/

HRESULT CVpuMft::AddInputStreams(
    DWORD   dwStreams,
    DWORD*  pdwStreamIDs)
{
    /*****************************************
    ** Todo: If your MFT does not have a fixed
    ** number of input streams, you must implement
    ** this function, see:
    ** http://msdn.microsoft.com/en-us/library/ms696211(v=VS.85).aspx
    *****************************************/

    return E_NOTIMPL;
}

HRESULT CVpuMft::DeleteInputStream(
    DWORD   dwStreamID)
{
    /*****************************************
    ** Todo: If your MFT does not have a fixed
    ** number of input streams, you must implement
    ** this function, see:
    ** http://msdn.microsoft.com/en-us/library/ms703159(v=VS.85).aspx
    *****************************************/

    return E_NOTIMPL;
}

HRESULT CVpuMft::GetAttributes(
    IMFAttributes** ppAttributes)
{
    HRESULT hr = S_OK;

    do
    {
        if(ppAttributes == NULL)
        {
            hr = E_POINTER;
            break;
        }

        (*ppAttributes) = m_pAttributes;

        if((*ppAttributes) == NULL)
        {
            hr = E_UNEXPECTED;
            break;
        }

        (*ppAttributes)->AddRef();
    }while(false);

    return hr;
}

HRESULT CVpuMft::GetInputAvailableType(
    DWORD           dwInputStreamID,
    DWORD           dwTypeIndex,
    IMFMediaType**  ppType)
{
    /*****************************************
    ** Todo: This function will return a media
    ** type at a given index. The SDK 
    ** implementation uses a static array of
    ** media types. Your MFT may want to use
    ** a dynamic array and modify the list 
    ** order depending on the MFTs state
    ** See http://msdn.microsoft.com/en-us/library/ms704814(v=VS.85).aspx
    *****************************************/

    HRESULT         hr      = S_OK;
    IMFMediaType*   pMT     = NULL;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(ppType == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwInputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        /*****************************************
        ** Todo: Modify the accepted input list
        ** g_ppguidInputTypes or use your own
        ** implementation of this function
        *****************************************/
        if(dwTypeIndex >= g_dwNumInputTypes)
        {
            hr = MF_E_NO_MORE_TYPES;
            break;
        }

        {
            CAutoLock lock(&m_csLock);

            hr = MFCreateMediaType(&pMT);
            if(FAILED(hr))
            {
                break;
            }

            hr = pMT->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
            if(FAILED(hr))
            {
                break;
            }

            hr = pMT->SetGUID(MF_MT_SUBTYPE, *(g_ppguidInputTypes[dwTypeIndex].guid));
            if(FAILED(hr))
            {
                break;
            }

            (*ppType) = pMT;
            (*ppType)->AddRef();
        }
    }while(false);

    SAFERELEASE(pMT);

    return hr;
}

HRESULT CVpuMft::GetInputCurrentType(
    DWORD           dwInputStreamID,
    IMFMediaType**  ppType)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms705607(v=VS.85).aspx
    *****************************************/

    HRESULT         hr      = S_OK;
    IMFMediaType*   pMT     = NULL;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(ppType == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwInputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        {
            CAutoLock lock(&m_csLock);

            if(m_pInputMT == NULL)
            {
                hr = MF_E_TRANSFORM_TYPE_NOT_SET;
                break;
            }

            /*******************************************
            ** Return a copy of the media type, not the 
            ** internal one. Returning the internal one 
            ** will allow an external component to modify
            ** the internal media type
            *******************************************/

            hr = MFCreateMediaType(&pMT);
            if(FAILED(hr))
            {
                break;
            }

            hr = DuplicateAttributes(pMT, m_pInputMT);
            if(FAILED(hr))
            {
                break;
            }
        }

        (*ppType) = pMT;
        (*ppType)->AddRef();
    }while(false);

    SAFERELEASE(pMT);

    return hr;
}

HRESULT CVpuMft::GetInputStatus(
    DWORD   dwInputStreamID,
    DWORD*  pdwFlags)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms697478(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(pdwFlags == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwInputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        *pdwFlags = 0;

        {
            CAutoLock lock(&m_csLock);

            // TODO:  one MFT I saw only says it accepts input if there's no  pending output
            // if (!m_fHasOutput)
            if((m_dwStatus & MYMFT_STATUS_INPUT_ACCEPT_DATA) != 0)
            {
                *pdwFlags = MFT_INPUT_STATUS_ACCEPT_DATA;
            }
        }
    }while(false);

    return hr;
}

HRESULT CVpuMft::GetInputStreamAttributes(
    DWORD           dwInputStreamID,
    IMFAttributes** ppAttributes)
{
    /*****************************************
    ** Todo: Becuase this MFT is acting as a decoder
    ** There will not be an upstream HW MFT.
    ** As such, no input stream attributes
    ** are required.
    ** See http://msdn.microsoft.com/en-us/library/ms695366(v=VS.85).aspx,
    ** http://msdn.microsoft.com/en-us/library/dd940330(VS.85).aspx#handshake
    *****************************************/

    return E_NOTIMPL;
}

HRESULT CVpuMft::GetInputStreamInfo(
    DWORD                   dwInputStreamID,
    MFT_INPUT_STREAM_INFO*  pStreamInfo)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms703894(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(pStreamInfo == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwInputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        pStreamInfo->hnsMaxLatency  = 0;
        pStreamInfo->dwFlags        = MFT_INPUT_STREAM_SINGLE_SAMPLE_PER_BUFFER | MFT_INPUT_STREAM_WHOLE_SAMPLES;
        pStreamInfo->cbSize         = 0; // No minimum size is required
        pStreamInfo->cbMaxLookahead = 0; // No lookahead is performed
        pStreamInfo->cbAlignment    = 0; // No memory allignment is required
    }while(false);

    return hr;
}

HRESULT CVpuMft::GetOutputAvailableType(
    DWORD           dwOutputStreamID,
    DWORD           dwTypeIndex,
    IMFMediaType**  ppType)
{
    /*****************************************
    ** Todo: This function will return a media
    ** type at a given index. The SDK 
    ** implementation uses a static array of
    ** media types. Your MFT may want to use
    ** a dynamic array and modify the list 
    ** order depending on the MFTs state
    ** See http://msdn.microsoft.com/en-us/library/ms703812(v=VS.85).aspx
    *****************************************/

    HRESULT         hr      = S_OK;
    IMFMediaType*   pMT     = NULL;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(ppType == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwOutputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        /*****************************************
        ** Todo: Modify the accepted output list
        ** g_ppguidOutputTypes or use your own
        ** implementation of this function
        *****************************************/
        if(dwTypeIndex >= g_dwNumOutputTypes)
        {
            hr = MF_E_NO_MORE_TYPES;
            break;
        }

        {
            CAutoLock lock(&m_csLock);

            hr = MFCreateMediaType(&pMT);
            if(FAILED(hr))
            {
                break;
            }

            hr = pMT->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
            if(FAILED(hr))
            {
                break;
            }

            hr = pMT->SetGUID(MF_MT_SUBTYPE, *(g_ppguidOutputTypes[dwTypeIndex]));
            if(FAILED(hr))
            {
                break;
            }

            /*****************************************
            ** Todo: The following implementation
            ** forces a standard output resolution
            ** and framerate. Your MFT should set these
            ** values properly and update the Media
            ** Type as necessary after decoding the
            ** stream
            *****************************************/
            if ((m_inputHeight == 0) || (m_inputWidth == 0))
            {
                m_outputWidth = MFT_OUTPUT_WIDTH;
                m_outputHeight = MFT_OUTPUT_HEIGHT;
            }
            else
            {
                m_outputWidth = Align(m_inputWidth, 16);
                m_outputHeight = Align(m_inputHeight, (m_bInterlaced ? 32 : 16));
            }
            hr = MFSetAttributeSize(pMT, MF_MT_FRAME_SIZE, m_outputWidth, m_outputHeight);
            if(FAILED(hr))
            {
                break;
            }

            // Set the display aperture to handle overscan due to output width and height alignment adjustments.
            MFVideoArea displayAperture = { 0 };

            displayAperture.Area.cx = m_inputWidth;
            displayAperture.Area.cy = m_inputHeight;
            hr = pMT->SetBlob(MF_MT_MINIMUM_DISPLAY_APERTURE, (UINT8*)&displayAperture, sizeof(displayAperture));
            if(FAILED(hr))
            {
                break;
            }

            if ((m_fps.Denominator != 0) && (m_fps.Numerator != 0))
            {
                hr = MFSetAttributeRatio(pMT, MF_MT_FRAME_RATE, m_fps.Numerator, m_fps.Denominator);
            }
            if(FAILED(hr))
            {
                break;
            }

            (*ppType) = pMT;
            (*ppType)->AddRef();
        }
    }while(false);

    SAFERELEASE(pMT);

    return hr;
}

HRESULT CVpuMft::GetOutputCurrentType(
    DWORD           dwOutputStreamID,
    IMFMediaType**  ppType)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms696985(v=VS.85).aspx
    *****************************************/

    HRESULT         hr      = S_OK;
    IMFMediaType*   pMT     = NULL;

    do
    {
        /************************************
        ** Since this MFT is a decoder, it
        ** must not allow this function to be
        ** called until it is unlocked. If
        ** your MFT is an encoder, this function
        ** CAN be called before the MFT is
        ** unlocked
        ************************************/
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(ppType == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwOutputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        {
            CAutoLock lock(&m_csLock);

            if(m_pOutputMT == NULL)
            {
                hr = MF_E_TRANSFORM_TYPE_NOT_SET;
                break;
            }

            /*******************************************
            ** Return a copy of the media type, not the 
            ** internal one. Returning the internal one 
            ** will allow an external component to modify
            ** the internal media type
            *******************************************/

            hr = MFCreateMediaType(&pMT);
            if(FAILED(hr))
            {
                break;
            }

            hr = DuplicateAttributes(pMT, m_pOutputMT);
            if(FAILED(hr))
            {
                break;
            }
        }

        (*ppType) = pMT;
        (*ppType)->AddRef();
    }while(false);

    SAFERELEASE(pMT);

    return hr;
}

HRESULT CVpuMft::GetOutputStatus(
    DWORD*  pdwFlags)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms696269(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(pdwFlags == NULL)
        {
            hr = E_POINTER;
            break;
        }

        (*pdwFlags) = 0;

        {
            CAutoLock lock(&m_csLock);

            // TODO: Should reset m_dwStatus when output is retreived?
            if((m_dwStatus & MYMFT_STATUS_OUTPUT_SAMPLE_READY) != 0)
            {
                *pdwFlags = MFT_OUTPUT_STATUS_SAMPLE_READY;
            }
        }

        TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Output Status Flags: 0x%x",  __FUNCTION__, (*pdwFlags));
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit (hr=0x%x)",  __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::GetOutputStreamAttributes(
    DWORD           dwOutputStreamID,
    IMFAttributes** ppAttributes)
{
    /*****************************************
    ** Todo: This MFT does not support a 
    ** hardware handshake, so this function
    ** is not implemented
    ** See http://msdn.microsoft.com/en-us/library/ms703886(v=VS.85).aspx,
    ** http://msdn.microsoft.com/en-us/library/dd940330(VS.85).aspx#handshake
    *****************************************/

    return E_NOTIMPL;
}

HRESULT CVpuMft::GetOutputStreamInfo(
    DWORD                   dwOutputStreamID,
    MFT_OUTPUT_STREAM_INFO* pStreamInfo)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms693880(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(pStreamInfo == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwOutputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        pStreamInfo->dwFlags        =   MFT_OUTPUT_STREAM_WHOLE_SAMPLES             | 
                                        MFT_OUTPUT_STREAM_SINGLE_SAMPLE_PER_BUFFER  |
                                        MFT_OUTPUT_STREAM_FIXED_SAMPLE_SIZE         |
                                        MFT_OUTPUT_STREAM_CAN_PROVIDE_SAMPLES;         
        pStreamInfo->cbSize         = (MFT_OUTPUT_WIDTH * MFT_OUTPUT_HEIGHT) * 4; // Since the MFT can output RGB32,
                                                                                  // it may need as many as 4 bytes
                                                                                  // per pixel, so the output buffer
                                                                                  // size must be set accordinly
                                                                                  // Todo: Change this value depending
                                                                                  // On the current output type
        pStreamInfo->cbAlignment    = 0; // No memory allignment is required
    }while(false);

    return hr;
}

HRESULT CVpuMft::GetStreamCount(
    DWORD*  pdwInputStreams,
    DWORD*  pdwOutputStreams)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms697018(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    do
    {
        if((pdwInputStreams == NULL) || (pdwOutputStreams == NULL))
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/

        *pdwInputStreams = MFT_MAX_STREAMS;
        *pdwOutputStreams = MFT_MAX_STREAMS;
    }while(false);

    return hr;
}

HRESULT CVpuMft::GetStreamIDs(
    DWORD   dwInputIDArraySize,
    DWORD*  pdwInputIDs,
    DWORD   dwOutputIDArraySize,
    DWORD*  pdwOutputIDs)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms693988(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if((pdwInputIDs == NULL) || (pdwOutputIDs == NULL))
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/

        if((dwInputIDArraySize < MFT_MAX_STREAMS) || (dwOutputIDArraySize < MFT_MAX_STREAMS))
        {
            hr = MF_E_BUFFERTOOSMALL;
            break;
        }

        pdwInputIDs[0]  = 0;
        pdwOutputIDs[0] = 0;
    }while(false);

    return hr;
}

HRESULT CVpuMft::GetStreamLimits(
    DWORD*  pdwInputMinimum,
    DWORD*  pdwInputMaximum,
    DWORD*  pdwOutputMinimum,
    DWORD*  pdwOutputMaximum)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms697040(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if((pdwInputMinimum == NULL) || (pdwInputMaximum == NULL) ||
            (pdwOutputMinimum == NULL) || (pdwOutputMaximum == NULL))
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/

        *pdwInputMinimum    = MFT_MAX_STREAMS;
        *pdwInputMaximum    = MFT_MAX_STREAMS;
        *pdwOutputMinimum   = MFT_MAX_STREAMS;
        *pdwOutputMaximum   = MFT_MAX_STREAMS;
    }while(false);

    return hr;
}

HRESULT CVpuMft::ProcessEvent(
    DWORD           dwInputStreamID,
    IMFMediaEvent*  pEvent)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms695394(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(pEvent == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwInputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        /****************************************
        ** Todo: this MFT does not handle any 
        ** events. It allows them all to be 
        ** propagated downstream. If your MFT
        ** needs to handle events, implement this
        ** function
        ****************************************/
        hr = E_NOTIMPL;
    }while(false);

    return hr;
}

HRESULT CVpuMft::ProcessInput(
    DWORD       dwInputStreamID,
    IMFSample*  pSample,
    DWORD       dwFlags)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms703131(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if (dwFlags != 0)
        {
            MyFailed(E_UNEXPECTED);
        }

        {
            CAutoLock lock(&m_csLock);

            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): NeedInputCount: %u",  __FUNCTION__, m_dwNeedInputCount);

            if(m_dwNeedInputCount == 0)
            {
                // This call does not correspond to a need input call
                hr = MF_E_NOTACCEPTING;
                break;
            }
            else
            {
                m_dwNeedInputCount--;
            }
        }

        if(pSample == NULL)
        {
            hr = E_POINTER;
            break;
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwInputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        // First, put sample into the input Queue

        /***************************************
        ** Since this in an internal function
        ** we know m_pInputSampleQueue can never be
        ** NULL due to InitializeTransform()
        ***************************************/
        {
            CAutoLock lock(&m_csLock);
            InterlockedIncrement(&m_ulInputSamplesAdded);
        }

        hr = m_pInputSampleQueue->AddSample(pSample);
        if(FAILED(hr))
        {
            break;
        }

        UINT32 inputWidth = 0;
        UINT32 inputHeight = 0;
        UINT32 inputNumerator = 0;
        UINT32 inputDenominator = 0;
        UINT32 interlaceMode = MFVideoInterlace_Unknown;
                
        if (SUCCEEDED(MFGetAttributeSize(pSample, MF_MT_FRAME_SIZE, &inputWidth, &inputHeight)))
        {
            CAutoLock lock(&m_csLock);

            if ((m_inputHeight == 0) || (m_inputWidth == 0))
            {
                m_inputHeight = inputHeight;
                m_inputWidth = inputWidth;
                m_outputWidth = Align(m_inputWidth, 16);
                m_outputHeight = Align(m_inputHeight, (m_bInterlaced ? 32 : 16));
            }
        }

        if (SUCCEEDED(MFGetAttributeRatio(pSample, MF_MT_FRAME_RATE, &inputNumerator, &inputDenominator)))
        {
            CAutoLock lock(&m_csLock);

            if ((m_fps.Numerator == 0) || (m_fps.Denominator == 0))
            {
                m_fps.Numerator = inputNumerator;
                m_fps.Denominator = inputDenominator;
            }
        }

        if (SUCCEEDED(pSample->GetUINT32(MF_MT_INTERLACE_MODE, &interlaceMode)))
        {
            CAutoLock lock(&m_csLock);

            if (m_uiInterlaceMode != interlaceMode)
            {
                m_uiInterlaceMode = interlaceMode;
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): interlaceMode changed to %d",  __FUNCTION__, interlaceMode);
            }
        }

        // Now schedule the work to decode the sample
        if (m_bufferCollection != nullptr)
        {
            hr = m_bufferCollection->FrameDecodeError();
            if (FAILED(hr))
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): buffer collection has deferred FrameDecode return error (hr=0x%x)", __FUNCTION__, hr);
                break;
            }
            hr = m_bufferCollection->ScheduleFrameDecode();
        }
        else
        {
            hr = ScheduleFrameDecode();
        }
        
        if(FAILED(hr))
        {
            break;
        }
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit (hr=0x%x)",  __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::ProcessMessage(
    MFT_MESSAGE_TYPE eMessage,
    ULONG_PTR ulParam)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms701863(v=VS.85).aspx
    *****************************************/

    HRESULT hr = S_OK;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if((m_pInputMT == NULL) || (m_pOutputMT == NULL))
        {
            // Can't process messages until media types are set
            hr = MF_E_TRANSFORM_TYPE_NOT_SET;
            break;
        }

        switch(eMessage)
        {
        case MFT_MESSAGE_NOTIFY_START_OF_STREAM:
            {
                hr = OnStartOfStream();
                if(FAILED(hr))
                {
                    break;
                }
            }
            break;
        case MFT_MESSAGE_NOTIFY_END_OF_STREAM:
            {
                hr = OnEndOfStream();
                if(FAILED(hr))
                {
                    break;
                }
            }
            break;
        case MFT_MESSAGE_COMMAND_DRAIN:
            {
                hr = OnDrain((UINT32)ulParam);
                if(FAILED(hr))
                {
                    break;
                }
            }
            break;
        case MFT_MESSAGE_COMMAND_FLUSH:
            {
                hr = OnFlush();
                if(FAILED(hr))
                {
                    break;
                }
            }
            break;
        case MFT_MESSAGE_COMMAND_MARKER:
            {
                hr = OnMarker(ulParam);
                if(FAILED(hr))
                {
                    break;
                }
            }
            break;
        /************************************************
        ** Todo: Add any MFT Messages that are not already
        ** covered
        ************************************************/
        default:
            // Nothing to do, return S_OK
            break;
        };
    }while(false);

    return hr;
}

HRESULT CVpuMft::ProcessOutput(
    DWORD                   dwFlags,
    DWORD                   dwOutputBufferCount,
    MFT_OUTPUT_DATA_BUFFER* pOutputSamples,
    DWORD*                  pdwStatus)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms704014(v=VS.85).aspx
    *****************************************/

    HRESULT     hr      = S_OK;
    IMFSample*  pSample = NULL;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if (dwFlags != 0)
        {
            MyFailed(E_UNEXPECTED);
        }

        {
            CAutoLock lock(&m_csLock);

            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): HaveOutputCount: %u",  __FUNCTION__, m_dwHaveOutputCount);

            if(m_dwHaveOutputCount == 0)
            {
                // This call does not correspond to a have output call
                hr = E_UNEXPECTED;
                break;
            }
            else
            {
                m_dwHaveOutputCount--;
            }

            if (m_dwNeedInputCount == 0)
            {
                if (!(m_dwStatus & MYMFT_STATUS_DRAINING))
                {
                    hr = RequestSample(0);
                    if (FAILED(hr))
                    {
                        break;
                    }
                }
            }
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwOutputBufferCount < MFT_MAX_STREAMS)
        {
            hr = E_INVALIDARG;
            break;
        }

        if(IsMFTReady() == FALSE)
        {
            hr = MF_E_TRANSFORM_TYPE_NOT_SET;
            break;
        }

        if(m_bStreamChange)
        {
            CAutoLock lock(&m_csLock);

            m_bStreamChange = FALSE;
            hr = MF_E_TRANSFORM_STREAM_CHANGE;
            pOutputSamples[0].dwStatus |= MFT_OUTPUT_DATA_BUFFER_FORMAT_CHANGE;
            break;
        }

        /***************************************
        ** Since this in an internal function
        ** we know m_pOutputSampleQueue can never be
        ** NULL due to InitializeTransform()
        ***************************************/
        hr = m_pOutputSampleQueue->GetNextSample(&pSample);
        if(FAILED(hr))
        {
            break;
        }

        if(pSample == NULL)
        {
            hr = MF_E_TRANSFORM_NEED_MORE_INPUT;
            break;
        }

        /*******************************
        ** Todo: This MFT only has one
        ** input stream, so the output
        ** samples array and stream ID
        ** will only use the first
        ** member
        *******************************/
        pOutputSamples[0].dwStreamID    = 0;
        
        if((pOutputSamples[0].pSample) == NULL)
        {
            // The MFT is providing it's own samples
            (pOutputSamples[0].pSample)   = pSample;
            (pOutputSamples[0].pSample)->AddRef();
        }
        else
        {
            // The pipeline has allocated the samples
            IMFMediaBuffer* pBuffer = NULL;

            do
            {
                hr = pSample->ConvertToContiguousBuffer(&pBuffer);
                if(FAILED(hr))
                {
                    break;
                }

                pOutputSamples[0].pSample->RemoveAllBuffers();

                hr = (pOutputSamples[0].pSample)->AddBuffer(pBuffer);
                if(FAILED(hr))
                {
                    break;
                }
            }while(false);

            SAFERELEASE(pBuffer);

            if(FAILED(hr))
            {
                break;
            }
        }

        // We're out of samples in the output queue
        if(m_pOutputSampleQueue->IsQueueEmpty() != FALSE)
        {
            CAutoLock lock(&m_csLock);

            // If we're draining, send the drain complete event
            if ((m_dwStatus & MYMFT_STATUS_DRAINING) != 0 && m_dwPendingFrameDecodeCount == 0)
            {
                hr = SendDrainCompleteEvent();
                if (FAILED(hr))
                {
                    break;
                }
            }
        }
    }while(false);

    SAFERELEASE(pSample);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit (hr=0x%x)",  __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::SendDrainCompleteEvent()
{
    HRESULT hr = S_OK;
    IMFMediaEvent*  pDrainCompleteEvent = NULL;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);
    do
    {
        hr = MFCreateMediaEvent(METransformDrainComplete, GUID_NULL, S_OK, NULL, &pDrainCompleteEvent);
        if (FAILED(hr))
        {
            break;
        }

        hr = pDrainCompleteEvent->SetUINT32(MF_EVENT_MFT_INPUT_STREAM_ID, 0);
        if (FAILED(hr))
        {
            break;
        }

        hr = m_pEventQueue->QueueEvent(pDrainCompleteEvent);
        if (FAILED(hr))
        {
            break;
        }
    }while(false);

    SAFERELEASE(pDrainCompleteEvent);

    if (!FAILED(hr))
    {
        m_dwStatus &= (~MYMFT_STATUS_DRAINING);
    }

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit (hr=0x%x)", __FUNCTION__, hr);
    return hr;
}

HRESULT CVpuMft::SetInputType(
    DWORD           dwInputStreamID,
    IMFMediaType*   pType,
    DWORD           dwFlags)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms700113(v=VS.85).aspx
    *****************************************/

    HRESULT         hr      = S_OK;
    IMFMediaType*   pMT     = NULL;
    VpuCodStd       codecFormat = VPU_V_MPEG4;

    do
    {
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(pType == NULL)
        {
            hr = E_POINTER;
            break;
        }

        if (dwFlags != 0)
        {
            MyFailed(E_UNEXPECTED);
        }

        if(dwInputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        hr = CheckInputType(pType, &codecFormat);
        if(FAILED(hr))
        {
            break;
        }

        /*******************************************
        ** Store a copy of the media type, not the
        ** one passed in by the caller. This way the
        ** caller is unable to modify the internal
        ** media type
        *******************************************/

        hr = MFCreateMediaType(&pMT);
        if(FAILED(hr))
        {
            break;
        }

        hr = DuplicateAttributes(pMT, pType);
        if(FAILED(hr))
        {
            break;
        }

        {
            CAutoLock lock(&m_csLock);
            VpuDecRetCode vpuRes;
            VpuDecOpenParam vpuParam = { 0 };
            UINT32 interlaceMode = MFVideoInterlace_Unknown;  
            UINT32 inputWidth = 0;
            UINT32 inputHeight = 0;

            hr = MFGetAttributeRatio(pMT, MF_MT_FRAME_RATE, (UINT32 *)&m_fps.Numerator, (UINT32 *)&m_fps.Denominator);
            if (FAILED(hr))
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): input stream MF_MT_FRAME_RATE unavailable, using defaults (hr=0x%x)", __FUNCTION__, hr);
                m_fps.Numerator = MFT_FRAMERATE_NUMERATOR;
                m_fps.Denominator = MFT_FRAMERATE_DENOMINATOR;
                hr = S_OK;
            }
            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): input stream MF_MT_FRAME_RATE numerator=%d, denominator=%d", __FUNCTION__, m_fps.Numerator, m_fps.Denominator);

            if (SUCCEEDED(pMT->GetUINT32(MF_MT_INTERLACE_MODE, &interlaceMode)))
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): input stream MF_MT_INTERLACE_MODE mode=%d", __FUNCTION__, interlaceMode);
                m_uiInterlaceMode = interlaceMode;
                if (IsInterlaced(interlaceMode) && (interlaceMode != MFVideoInterlace_MixedInterlaceOrProgressive))
                {
                    // input is definitely interlaced
                    m_bInterlaced = TRUE;
                }
            }
            else
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): input stream MF_MT_INTERLACE_MODE unavailable, using defaults (hr=0x%x)", __FUNCTION__, hr);
            }

            if (SUCCEEDED(MFGetAttributeSize(pMT, MF_MT_FRAME_SIZE, &inputWidth, &inputHeight)))
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): input stream MF_MT_FRAME_SIZE Width=%d, Height=%d", __FUNCTION__, inputWidth, inputHeight);
                if ((m_inputHeight == 0) || (m_inputWidth == 0))
                {
                    m_inputHeight = inputHeight;
                    m_inputWidth = inputWidth;
                    m_outputWidth = Align(m_inputWidth, 16);
                    m_outputHeight = Align(m_inputHeight, (m_bInterlaced ? 32 : 16));
                }
            }
            else
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): input stream MF_MT_FRAME_SIZE unavailable, using defaults (hr=0x%x)", __FUNCTION__, hr);
            }

            vpuParam.CodecFormat = codecFormat;
            vpuParam.nPixelFormat = 1;  // 8 bit pixel
            vpuParam.nMapType = 0;
            vpuParam.nChromaInterleave = 1;
            vpuParam.nReorderEnable = 1;
            vpuParam.nAdaptiveMode = 1;
            vpuParam.nPicHeight = inputHeight;
            vpuParam.nPicWidth = inputWidth;

            if (m_vpuHandle != 0)
            {
                VPU_DecClose(m_vpuHandle);
                m_vpuHandle = 0;
            }

            vpuRes = VPU_DecOpen(&m_vpuHandle, &vpuParam, &m_vpuMemInfo);
            if (vpuRes != VPU_DEC_RET_SUCCESS)
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecOpen returned error %d", vpuRes);
                hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ACCELERATOR, vpuRes);
                break;
            }

            int capability;

            vpuRes = VPU_DecGetCapability(m_vpuHandle, VPU_DEC_CAP_FRAMESIZE, &capability);
            if (vpuRes != VPU_DEC_RET_SUCCESS)
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecGetCapability(CAP_FRAMESIZE), returned error %d", vpuRes);
                hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ACCELERATOR, vpuRes);
                break;
            }

            TraceString(
                CHMFTTracing::TRACE_INFORMATION,
                L"VPU_DecGetCapability(VPU_DEC_CAP_FRAMESIZE) value = %d",
                capability);

            int param;

            // setup vpu decoder configuration to defaults
            param = VPU_DEC_SKIPNONE;
            vpuRes = VPU_DecConfig(m_vpuHandle, VPU_DEC_CONF_SKIPMODE, &param);
            if (vpuRes != VPU_DEC_RET_SUCCESS)
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecConfig(VPU_DEC_CONF_SKIPMODE, VPU_DEC_SKIPNONE), returned error %d", vpuRes);
                hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ACCELERATOR, vpuRes);
                break;
            }

            param = 0;
            vpuRes = VPU_DecConfig(m_vpuHandle, VPU_DEC_CONF_BUFDELAY, &param);
            if (vpuRes != VPU_DEC_RET_SUCCESS)
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecConfig(VPU_DEC_CONF_BUFDELAY,0), returned error %d", vpuRes);
                hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ACCELERATOR, vpuRes);
                break;
            }
            
            SAFERELEASE(m_pInputMT);

            m_pInputMT = pMT;
            m_pInputMT->AddRef();
        }

        IsMFTReady();
    }while(false);

    SAFERELEASE(pMT);

    return hr;
}

HRESULT CVpuMft::SetOutputBounds(
    LONGLONG hnsLowerBound,
    LONGLONG hnsUpperBound)
{
    /*****************************************
    ** Todo: This MFT does not support  
    ** sample boundries
    ** See http://msdn.microsoft.com/en-us/library/ms693812(v=VS.85).aspx
    *****************************************/

    return E_NOTIMPL;
}

HRESULT CVpuMft::SetOutputType(
    DWORD           dwOutputStreamID,
    IMFMediaType*   pType,
    DWORD           dwFlags)
{
    /*****************************************
    ** See http://msdn.microsoft.com/en-us/library/ms702016(v=VS.85).aspx
    *****************************************/

    HRESULT         hr      = S_OK;
    IMFMediaType*   pMT     = NULL;

    do
    {
        /************************************
        ** Since this MFT is a decoder, it
        ** must not allow this function to be
        ** called until it is unlocked. If
        ** your MFT is an encoder, this function
        ** CAN be called before the MFT is
        ** unlocked
        ************************************/
        if(IsLocked() != FALSE)
        {
            hr = MF_E_TRANSFORM_ASYNC_LOCKED;
            break;
        }

        if(pType == NULL)
        {
            hr = E_POINTER;
            break;
        }

        if (dwFlags != 0)
        {
            MyFailed(E_UNEXPECTED);
        }

        /*****************************************
        ** Todo: If your MFT supports more than one
        ** stream, make sure you modify
        ** MFT_MAX_STREAMS and adjust this function
        ** accordingly
        *****************************************/
        if(dwOutputStreamID >= MFT_MAX_STREAMS)
        {
            hr = MF_E_INVALIDSTREAMNUMBER;
            break;
        }

        hr = CheckOutputType(pType);
        if(FAILED(hr))
        {
            break;
        }

        /*******************************************
        ** Store a copy of the media type, not the
        ** one passed in by the caller. This way the
        ** caller is unable to modify the internal
        ** media type
        *******************************************/

        hr = MFCreateMediaType(&pMT);
        if(FAILED(hr))
        {
            break;
        }

        hr = DuplicateAttributes(pMT, pType);
        if(FAILED(hr))
        {
            break;
        }

        {
            CAutoLock lock(&m_csLock);

            SAFERELEASE(m_pOutputMT);

            m_pOutputMT = pMT;
            m_pOutputMT->AddRef();
        }

        IsMFTReady();
    }while(false);

    SAFERELEASE(pMT);

    return hr;
}