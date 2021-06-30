/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "vpumft.h"
#include "vpumft_DecodeTask.h"
#include "CAutoLock.h"
#include "CVpuMediaBuffer.h"
#include <mfapi.h>
#include <mferror.h>
#include <tracing.h>

#include <initguid.h>

// {C5026A4C-A546-4B93-9FAA-9D0DD16941FE}
DEFINE_GUID(VpuMft_MFSampleExtension_Marker,
0xc5026a4c, 0xa546, 0x4b93, 0x9f, 0xaa, 0x9d, 0xd, 0xd1, 0x69, 0x41, 0xfe);

#define MFT_NUM_DEFAULT_ATTRIBUTES  4
#define VPU_HW_URL     L"i.MX VPU MFT hardware accelerator"

// - Copy VPU frame buffer to intermediate buffer
// #define VPU_COPY_FRAMEBUFFER

#undef FAILED
#define FAILED(hr) MyFailed(hr)

bool VpuFailed(VpuDecRetCode result);

// Global Variables
const struct _codecMap g_ppguidInputTypes[] =
    {
        { &MFVideoFormat_H264, VPU_V_AVC},
        { &MFVideoFormat_H265, VPU_V_HEVC},
        { &MFVideoFormat_HEVC, VPU_V_HEVC},
        { &MFVideoFormat_VP80, VPU_V_VP8},
        { &MFVideoFormat_VP90, VPU_V_VP9},
    };
const DWORD     g_dwNumInputTypes   = sizeof(g_ppguidInputTypes) / sizeof(g_ppguidInputTypes[0]);

const GUID*     g_ppguidOutputTypes[] =
    {
        &MFVideoFormat_NV12,
    };
const DWORD     g_dwNumOutputTypes   = sizeof(g_ppguidOutputTypes) / sizeof(g_ppguidOutputTypes[0]);


// DEBUG: global instance reference
CVpuMft *g_Instance = NULL;

struct _timespec64 CVpuMft::m_clockStart = { 0, 0 };

// Initializer
HRESULT CVpuMft::CreateInstance(IMFTransform** ppHWMFT)
{
    HRESULT hr          = S_OK;
    CVpuMft* pMyHWMFT    = NULL;

    do
    {
        if(ppHWMFT == NULL)
        {
            hr = E_POINTER;
            break;
        }

        pMyHWMFT = new CVpuMft();
        if(FAILED(hr))
        {
            break;
        }

        // TODO: remove this
        g_Instance = pMyHWMFT;

        hr = pMyHWMFT->InitializeTransform();
        if(FAILED(hr))
        {
            break;
        }

        hr = pMyHWMFT->QueryInterface(IID_IMFTransform, (void**)ppHWMFT);
        if(FAILED(hr))
        {
            break;
        }
    }while(false);

    SAFERELEASE(pMyHWMFT);

    return hr;
}

/****************************
********** ***********
****************************/

CVpuMft::CVpuMft(void)
{
    /****************************************************
    ** Todo: Initialize All Member variables used by your
    ** MFT
    ****************************************************/

    // Do not insert anything before this call, this is the DLLs object count
    InterlockedIncrement(&m_ulNumObjects);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);
    
    m_ulRef                 = 1;
    m_ulInputCounter        = 0;
    m_ulOutputCounter       = 0;
    m_ulInputSamplesAdded   = 0;
    m_ulInputSamplesDequeued = 0;
    m_ulKeyFrames           = 0;
    m_ulDroppedFrames       = 0;
    m_llNextSampleTime      = 0;

    m_Sub5000               = 0;
    m_Above5000             = 0;
    m_AboveFPS              = 0;

    m_dropMode = MF_DROP_MODE_NONE;

    m_streamStart.tv_nsec   = 0;
    m_streamStart.tv_sec    = 0;
    m_pInputMT              = NULL;
    m_pOutputMT             = NULL;
    m_pAttributes           = NULL;
    memset(&m_fps, 0, sizeof(m_fps));
    m_inputHeight           = 0;
    m_inputWidth            = 0;
    m_outputHeight          = 0;
    m_outputWidth           = 0;
    m_uiInterlaceMode       = MFVideoInterlace_Unknown;
    m_bInterlaced           = FALSE;
    m_pEventQueue           = NULL;
    m_dwStatus              = 0;
    m_dwNeedInputCount      = 0;
    m_dwHaveOutputCount     = 0;
    m_dwPendingFrameDecodeCount = 0;
    m_dwDecodeWorkQueueID   = 0;
    m_bShutdown             = FALSE;
    m_bFirstSample          = TRUE;
    m_bDXVA                 = FALSE;
    m_bStreamChange         = FALSE;
    m_pInputSampleQueue     = NULL;
    m_pOutputSampleQueue    = NULL;
    m_bufferCollection      = NULL;

    InitializeCriticalSection(&m_csLock);

    m_vpuDecLoaded = false;
    m_vpuDecInit = false;
    memset(&m_vpuMemInfo, 0, sizeof(m_vpuMemInfo));
    m_vpuHandle = 0;
    memset(&m_vpuMemBlocks, 0, sizeof(m_vpuMemBlocks));
    memset(&m_vpuDescArray, 0, sizeof(m_vpuDescArray));
    memset(&m_vpuFrameBuffers, 0, sizeof(m_vpuFrameBuffers));
    memset(&m_vpuInfo, 0, sizeof(m_vpuInfo));
    m_ulYSize = 0;
    m_ulUSize = 0;
    m_ulVSize = 0;

    if (m_clockStart.tv_sec == 0 && m_clockStart.tv_nsec == 0)
    {
        _timespec64_get(&m_clockStart, TIME_UTC);
    }

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit", __FUNCTION__);
}

CVpuMft::~CVpuMft(void)
{
    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    // release the sample queues before closing the VPU handle. Output MediaSamples could
    // still call into m_vpuHandle->VPU_DecOutFrameDisplayed on release and attempt to use the 
    // codec function table.
    if (m_pEventQueue != NULL)
    {
        m_pEventQueue->Shutdown();
        SAFERELEASE(m_pEventQueue);
    }
    SAFERELEASE(m_pInputSampleQueue);
    SAFERELEASE(m_pOutputSampleQueue);
    SAFERELEASE(m_bufferCollection);

    SAFERELEASE(m_pInputMT);
    SAFERELEASE(m_pOutputMT);
    SAFERELEASE(m_pAttributes);

    if (m_vpuHandle != 0)
    {
        VPU_DecClose(m_vpuHandle);
        m_vpuHandle = 0;
    }

    FreeVpuMem();

    for (int i = 0; i < sizeof(m_vpuDescArray) / sizeof(m_vpuDescArray[0]); i++)
    {
        VpuMemDesc *pMemDesc = m_vpuDescArray + i;
        if (pMemDesc->nVirtAddr != (addr_t)0)
        {
            VpuDecRetCode decRes = VPU_DecFreeMem(pMemDesc);
            if (VpuFailed(decRes))
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecFreeMem() returned error %d", decRes);
                continue;
            }

            memset(pMemDesc, 0, sizeof(VpuMemDesc));
        }
    }
    if (m_vpuDecLoaded)
    {
        VPU_DecUnLoad();
    }

    DeleteCriticalSection(&m_csLock);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit", __FUNCTION__);

    InterlockedDecrement(&m_ulNumObjects);
    // Do no insert anything after this call, this is the DLLs object count
}

HRESULT CVpuMft::InitializeTransform(void)
{
    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    do
    {
        VpuDecRetCode vpuRes;
        VpuVersionInfo vpuVersion;
        VpuWrapperVersionInfo vpuWrapper;

        vpuRes = VPU_DecLoad();
        if (vpuRes != VPU_DEC_RET_SUCCESS)
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecLoad returned error %d", vpuRes);
            hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ACCELERATOR, vpuRes);
            break;
        }

        vpuRes = VPU_DecGetVersionInfo(&vpuVersion);
        if (vpuRes != VPU_DEC_RET_SUCCESS)
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecGetVersionInfo returned error %d", vpuRes);
            hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ACCELERATOR, vpuRes);
            break;
        }

        TraceString(
            CHMFTTracing::TRACE_INFORMATION, 
            L"VPU_DecGetVersionInfo returned firmware %d.%d.%d_r%d library %d.%d.%d", 
            vpuVersion.nFwMajor,
            vpuVersion.nFwMinor,
            vpuVersion.nFwRelease,
            vpuVersion.nFwCode,
            vpuVersion.nLibMajor,
            vpuVersion.nLibMinor,
            vpuVersion.nLibRelease);

        vpuRes = VPU_DecGetWrapperVersionInfo(&vpuWrapper);
        if (vpuRes != VPU_DEC_RET_SUCCESS)
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecGetWrapperVersionInfo returned error %d", vpuRes);
            hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ACCELERATOR, vpuRes);
            break;
        }

        TraceString(
            CHMFTTracing::TRACE_INFORMATION,
            L"VPU_DecGetWrapperVersionInfo returned %d.%d.%d %S",
            vpuWrapper.nMajor,
            vpuWrapper.nMinor,
            vpuWrapper.nRelease,
            vpuWrapper.pBinary);

        vpuRes = VPU_DecQueryMem(&m_vpuMemInfo);
        if (vpuRes != VPU_DEC_RET_SUCCESS)
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecQueryMem returned error %d", vpuRes);
            hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ACCELERATOR, vpuRes);
            break;
        }

        hr = AllocVpuMem();
        if (!SUCCEEDED(hr))
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"VPU Malloc memory block failed");
            break;
        }

        //
        hr = MFCreateAttributes(&m_pAttributes, MFT_NUM_DEFAULT_ATTRIBUTES);
        if(FAILED(hr))
        {
            break;
        }

        /*********************************
        ** Certain Attributes are required
        ** for HW MFTs
        ** See http://msdn.microsoft.com/en-us/library/dd940330(VS.85).aspx#attributes
        *********************************/
        hr = m_pAttributes->SetUINT32(MF_TRANSFORM_ASYNC, TRUE);
        if(FAILED(hr))
        {
            break;
        }

        /****************************************
        ** !!MSFT_TODO: Report as HW MFT
        ****************************************
        hr = m_pAttributes->SetString(MFT_ENUM_HARDWARE_URL_Attribute, MFT_HW_URL);
        if(FAILED(hr))
        {
            break;
        }

        hr = m_pAttributes->SetString(MFT_ENUM_HARDWARE_VENDOR_ID_Attribut, MFT_HW_VENDOR_ID);
        if(FAILED(hr))
        {
            break;
        }
        */

        hr = m_pAttributes->SetUINT32(MFT_SUPPORT_DYNAMIC_FORMAT_CHANGE, TRUE);
        if(FAILED(hr))
        {
            break;
        }

        /**********************************
        ** Since this is an Async MFT, an
        ** event queue is required
        ** MF Provides a standard implementation
        **********************************/
        hr = MFCreateEventQueue(&m_pEventQueue);
        if(FAILED(hr))
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Failed to create MF Event Queue (hr=0x%x)", __FUNCTION__, hr);
            break;
        }

        hr = CSampleQueue::Create(&m_pInputSampleQueue);
        if(FAILED(hr))
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Failed to create Input Sample Queue (hr=0x%x)", __FUNCTION__, hr);
            break;
        }

        hr = CSampleQueue::Create(&m_pOutputSampleQueue);
        if(FAILED(hr))
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Failed to create Output Sample Queue (hr=0x%x)", __FUNCTION__, hr);
            break;
        }

        /**********************************
        ** Since this is an Async MFT, all
        ** work will be done using standard
        ** MF Work Queues
        **********************************/
        hr = MFAllocateWorkQueueEx(MF_STANDARD_WORKQUEUE, &m_dwDecodeWorkQueueID);
        if(FAILED(hr))
        {
            break;
        }
    }while(false);
    
    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit (hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::CheckInputType(
    IMFMediaType*   pMT,
    VpuCodStd*      pCodec)
{
    HRESULT hr      = S_OK;
    GUID    guid    = GUID_NULL;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    do
    {
        /******************************
        ** No need to verify pMT != NULL
        ** as this is a private function
        ******************************/

        hr = pMT->GetGUID(MF_MT_MAJOR_TYPE, &guid);
        if(FAILED(hr))
        {
            break;
        }

        if(guid != MFMediaType_Video)
        {
            hr = MF_E_INVALIDMEDIATYPE;
            break;
        }

        hr = pMT->GetGUID(MF_MT_SUBTYPE, &guid);
        if(FAILED(hr))
        {
            break;
        }

        hr = MF_E_INVALIDMEDIATYPE; // Gets set to S_OK if MT is acceptable

        for(DWORD i = 0; i < g_dwNumInputTypes; i++)
        {
            if(guid == *(g_ppguidInputTypes[i].guid))
            {
                *pCodec = g_ppguidInputTypes[i].codec;
                hr = S_OK;
                break;
            }
        }

        if(FAILED(hr))
        {
            // The Mediatype is acceptable
            break;
        }
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::CheckOutputType(
    IMFMediaType*   pMT)
{
    /*************************************
    ** Todo: Your MFT should verify the
    ** Output media type is acceptable
    ** Modify this function as necessary
    *************************************/

    HRESULT hr              = S_OK;
    GUID    guid            = GUID_NULL;
    UINT32  un32HighWord    = 0;
    UINT32  un32LowWord     = 0;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    do
    {
        /******************************
        ** No need to verify pMT != NULL
        ** as this is a private function
        ******************************/

        hr = pMT->GetGUID(MF_MT_MAJOR_TYPE, &guid);
        if(FAILED(hr))
        {
            break;
        }

        if(guid != MFMediaType_Video)
        {
            hr = MF_E_INVALIDMEDIATYPE;
            break;
        }

        hr = pMT->GetGUID(MF_MT_SUBTYPE, &guid);
        if(FAILED(hr))
        {
            break;
        }

        hr = MF_E_INVALIDMEDIATYPE; // Gets set to S_OK if MT is acceptable

        for(DWORD i = 0; i < g_dwNumOutputTypes; i++)
        {
            if(guid == *(g_ppguidOutputTypes[i]))
            {
                hr = S_OK;
                break;
            }
        }

        if(FAILED(hr))
        {
            break;
        }

        hr = MFGetAttributeSize(pMT, MF_MT_FRAME_SIZE, &un32HighWord, &un32LowWord);
        if(FAILED(hr))
        {
            break;
        }

        if((un32HighWord != m_outputWidth) || (un32LowWord != m_outputHeight))
        {
            hr = MF_E_INVALIDMEDIATYPE;
            break;
        }

        hr = MFGetAttributeRatio(pMT, MF_MT_FRAME_RATE, &un32HighWord, &un32LowWord);
        if(FAILED(hr))
        {
            break;
        }

        if((un32HighWord != m_fps.Numerator) || (un32LowWord != m_fps.Denominator))
        {
            hr = MF_E_INVALIDMEDIATYPE;
            break;
        }

        // The Mediatype is acceptable
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::ShutdownEventQueue(void)
{
    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    do
    {
        /***************************************
        ** Since this in an internal function
        ** we know m_pEventQueue can never be
        ** NULL due to InitializeTransform()
        ***************************************/

        hr = m_pEventQueue->Shutdown();
        if(FAILED(hr))
        {
            break;
        }
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::OnStartOfStream(void)
{
    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    do
    {
        {
            CAutoLock lock(&m_csLock);
                
            m_dwStatus |= MYMFT_STATUS_STREAM_STARTED;
        }

        _timespec64_get(&m_streamStart, TIME_UTC);

        hr = RequestSample(0);  
        if(FAILED(hr))
        {
            break;
        }
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::OnEndOfStream(void)
{
    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    do
    {
        CAutoLock lock(&m_csLock);
                
        m_dwStatus &= (~MYMFT_STATUS_STREAM_STARTED);

        /*****************************************
        ** See http://msdn.microsoft.com/en-us/library/dd317909(VS.85).aspx#processinput
        ** Upon receiving EOS, the outstanding process
        ** input request should be reset to 0
        *****************************************/
        m_dwNeedInputCount = 0;
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::OnDrain(
    const UINT32 un32StreamID)
{
    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    do
    {
        CAutoLock lock(&m_csLock);
                
        m_dwStatus |= (MYMFT_STATUS_DRAINING);

        if (m_dwHaveOutputCount == 0 && m_dwPendingFrameDecodeCount == 0)
        {
            hr = SendDrainCompleteEvent();
        }
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::OnFlush(void)
{
    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);
    
    do
    {
        CAutoLock lock(&m_csLock);
                
        m_dwStatus &= (~MYMFT_STATUS_STREAM_STARTED);

        hr = FlushSamples();
        if(FAILED(hr))
        {
            break;
        }

        m_llNextSampleTime   = 0;    // Reset our sample time to 0 on a flush
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::OnMarker(
    const ULONG_PTR pulID)
{
    HRESULT hr  = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter", __FUNCTION__);

    do
    {
        // No need to lock, our sample queue is thread safe

        /***************************************
        ** Since this in an internal function
        ** we know m_pInputSampleQueue can never be
        ** NULL due to InitializeTransform()
        ***************************************/

        hr = m_pInputSampleQueue->MarkerNextSample(pulID);
        if(FAILED(hr))
        {
            break;
        }
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::RequestSample(
    const UINT32 un32StreamID)
{
    HRESULT         hr      = S_OK;
    IMFMediaEvent*  pEvent  = NULL;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        {
            CAutoLock lock(&m_csLock);

            if((m_dwStatus & MYMFT_STATUS_STREAM_STARTED) == 0)
            {
                // Stream hasn't started
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Stream Hasn't Started",  __FUNCTION__);
                hr = MF_E_NOTACCEPTING;
                break;
            }
        }

        hr = MFCreateMediaEvent(METransformNeedInput, GUID_NULL, S_OK, NULL, &pEvent);
        if(FAILED(hr))
        {
            break;
        }

        hr = pEvent->SetUINT32(MF_EVENT_MFT_INPUT_STREAM_ID, un32StreamID);
        if(FAILED(hr))
        {
            break;
        }

        // Increment the input needed counter before enqueueing the input needed event.
        {
            CAutoLock lock(&m_csLock);

            m_dwNeedInputCount++;

            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): NeedInputCount: %u", __FUNCTION__, m_dwNeedInputCount);
        }

        hr = m_pEventQueue->QueueEvent(pEvent);
        if(FAILED(hr))
        {
            break;
        }

    }while(false);

    SAFERELEASE(pEvent);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}


HRESULT CVpuMft::FlushSamples(void)
{
    HRESULT hr = S_OK;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        CAutoLock lock(&m_csLock);

        hr = OnEndOfStream();       // Treat this like an end of stream, don't accept new samples until
        if(FAILED(hr))              // a new stream is started
        {
            break;
        }        

        m_dwHaveOutputCount = 0;    // Don't Output samples until new input samples are given

        hr = m_pInputSampleQueue->RemoveAllSamples();
        if(FAILED(hr))
        {
            break;
        }

        hr = m_pOutputSampleQueue->RemoveAllSamples();
        if(FAILED(hr))
        {
            break;
        }

        m_bFirstSample = TRUE; // Be sure to reset our first sample so we know to set discontinuity
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::ScheduleFrameDecode(void)
{
    HRESULT             hr              = S_OK;
    IMFSample*          pInputSample    = NULL;
    CDecodeTask*        pDecodeTask     = NULL;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        // No need to lock, sample queues are thread safe

        /***************************************
        ** Since this in an internal function
        ** we know m_pInputSampleQueue can never be
        ** NULL due to InitializeTransform()
        ***************************************/
        hr = m_pInputSampleQueue->GetNextSample(&pInputSample);
        if(FAILED(hr))
        {
            break;
        }

        if (hr == S_FALSE)
        {
            // empty sample queue
            hr = S_OK;
            break;
        }

        {
            CAutoLock lock(&m_csLock);
            InterlockedIncrement(&m_ulInputSamplesDequeued);
            m_dwPendingFrameDecodeCount++;
        }

        hr = CDecodeTask::Create(
            m_dwDecodeWorkQueueID,
            pInputSample,
            (IMFAsyncCallback**)&pDecodeTask);
        if(FAILED(hr))
        {
            CAutoLock lock(&m_csLock);
            m_dwPendingFrameDecodeCount--;
            break;
        }

        hr = pDecodeTask->Begin(this);
        if(FAILED(hr))
        {
            CAutoLock lock(&m_csLock);
            m_dwPendingFrameDecodeCount--;
            break;
        }
    }while(false);

    SAFERELEASE(pInputSample);
    SAFERELEASE(pDecodeTask);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::DecodeInputFrame(
    IMFSample*  pInputSample)
{
    HRESULT         hr = S_OK;
    DWORD           dwDataLen = 0;
    IMFMediaBuffer* pInputBuffer = NULL;
    BYTE*           pbInputData = NULL;
    IMFSample*      pOutputSample = NULL;
    IMFMediaBuffer* pOutputBuffer = NULL;
    ULONG           ulCurrentSample = 0;
    UINT64          pun64MarkerID = 0;
    bool            needInput = false;
    bool            isKeyFrame = false;
    struct _timespec64 frameStart = { 0 };
    struct _timespec64 frameEnd = { 0 };

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        /****************************************
        ** Todo, here's where the MFT transforms
        ** the input to output samples. In the
        ** SDK sample, the output is simply a 
        ** numbered frame, so no complicated
        ** processing is done here. Your MFT
        ** should reference it's input and output
        ** types to ensure it does the right thing
        ** here and modify this function accordingly
        ** Addionally, your MFT must monitor for
        ** format changes and act accordly
        ** See http://msdn.microsoft.com/en-us/library/ee663587(VS.85).aspx
        ****************************************/

        ulCurrentSample = InterlockedIncrement(&m_ulInputCounter);
        _timespec64_get(&frameStart, TIME_UTC);

        if(m_bDXVA != FALSE) // Not thread safe!
        {
            /****************************************
            ** !!MSFT_TODO: handle dxva!
            ****************************************/

            //MessageBox(NULL, L"TODO: Make MFT Handle DXVA!", L"ERROR!", MB_OK);
            hr = E_NOTIMPL;
            break;
        }
        else
        {
            dwDataLen = (MFT_OUTPUT_WIDTH * MFT_OUTPUT_HEIGHT) * 4; // This is the max needed for the
                                                                    // biggest supported output type
        }

        if (IsKeyFrame(pInputSample))
        {
            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Sample %ld is key frame",
                __FUNCTION__, ulCurrentSample);
            InterlockedIncrement(&m_ulKeyFrames);
            isKeyFrame = true;
        }

        // Start frame decoding here
        DWORD maxLen = 0;
        DWORD curLen = 0;
        VpuDecRetCode retCode;
        VpuBufferNode inData = { 0 };
        VpuFrameBuffer *vpuOutFrameBuffer = NULL;
        int bufRetCode = 0;

        hr = pInputSample->ConvertToContiguousBuffer(&pInputBuffer);
        if (FAILED(hr)) {
            break;
        }

        hr = pInputBuffer->Lock(&pbInputData, &maxLen, &curLen);
        if (FAILED(hr)) {
            break;
        }

        inData.nSize = curLen;
        inData.pVirAddr = pbInputData;
        inData.sCodecData.nSize = 0;
        inData.sCodecData.pData = NULL;

        do {
            bufRetCode = 0;

            if (m_vpuDecInit && (m_dropMode > MF_DROP_MODE_2))
            {
                if (isKeyFrame)
                {
                    // force a discontinuity and resync presentation time stamp
                    m_bFirstSample = true;
                    m_llNextSampleTime = 0;
                }
                else
                {
                    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Dropping P or B frame %ld ",
                        __FUNCTION__, ulCurrentSample);
                    InterlockedIncrement(&m_ulDroppedFrames);
                    needInput = true;
                    break;
                }
            }

//          _timespec64_get(&frameStart, TIME_UTC);
            retCode = VPU_DecDecodeBuf(m_vpuHandle, &inData, &bufRetCode);
            if (VpuFailed(retCode))
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecDecodeBuf failed %d",
                    __FUNCTION__, retCode);
                hr = E_UNEXPECTED;
                break;
            }

            if (bufRetCode & VPU_DEC_INPUT_USED)
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): VPU_DEC_INPUT_USED",
                    __FUNCTION__);
                inData.nSize = 0;
                inData.pVirAddr = nullptr;
            }

            if (bufRetCode & VPU_DEC_INIT_OK)
            {
                // VPU stream initialized, allocate buffers and pass to vpu
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): VPU_DEC_INIT_OK",
                    __FUNCTION__);
                memset(&m_vpuInfo, 0, sizeof(m_vpuInfo));
                retCode = VPU_DecGetInitialInfo(m_vpuHandle, &m_vpuInfo);
                if (VpuFailed(retCode))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecGetInitialInfo failed %d",
                        __FUNCTION__, retCode);
                    hr = E_UNEXPECTED;
                    break;
                }

                TraceString(CHMFTTracing::TRACE_INFORMATION, L"VPU_DecGetInitialInfo returned:"
                    " x %d y %d bits %d interlace %d min frame count %d", m_vpuInfo.nPicWidth, m_vpuInfo.nPicHeight,
                    m_vpuInfo.nBitDepth, m_vpuInfo.nInterlace, m_vpuInfo.nMinFrameBufferCount);

                if (m_vpuInfo.nMinFrameBufferCount + MFT_MIN_BUFFERS > MFT_MAX_BUFFERS)
                {
                    TraceString(CHMFTTracing::TRACE_INFORMATION, L"codec min frame count %d  + mft min buffers %d > MFT_MAX_BUFFERS %d. "
                        "Running at reduced capacity.", m_vpuInfo.nMinFrameBufferCount, MFT_MIN_BUFFERS, MFT_MAX_BUFFERS);
                }

                // calculate yuv buffer sizes
                int bufCount = min(m_vpuInfo.nMinFrameBufferCount + MFT_MIN_BUFFERS, MFT_MAX_BUFFERS);
                int yStride = Align(m_vpuInfo.nPicWidth, 16);
                int ySize = yStride * Align(m_vpuInfo.nPicHeight, (m_vpuInfo.nInterlace ? 32 : 16));

                // Assume 4:2:0 video
                int uStride = yStride / 2;
                int vStride = uStride;
                int uSize = ySize / 4;
                int vSize = uSize;

                int alignment = m_vpuInfo.nAddressAlignment;
                if (alignment > 1)
                {
                    ySize = Align(ySize, alignment);
                    uSize = Align(uSize, alignment);
                    vSize = Align(vSize, alignment);
                }

                memset(m_vpuFrameBuffers, 0, sizeof(m_vpuFrameBuffers));
                memset(m_vpuDescArray, 0, sizeof(m_vpuDescArray));
                if (bufCount > sizeof(m_vpuDescArray) / sizeof(m_vpuDescArray[0]))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Too many buffers to allocate", __FUNCTION__);
                    hr = E_UNEXPECTED;
                    FAILED(hr);
                    break;
                }

                m_ulYSize = ySize;
                m_ulUSize = uSize;
                m_ulVSize = vSize;

                int totalSize = ySize + uSize + vSize + vSize + alignment;

                TraceString(CHMFTTracing::TRACE_INFORMATION, L"calculated frame size %d, VPU requested frame size %d",
                    totalSize, m_vpuInfo.nFrameSize);
                totalSize = max(totalSize, m_vpuInfo.nFrameSize);

                hr = CVpuBufferCollection::CreateInstance(this, bufCount, m_vpuInfo.nMinFrameBufferCount, &m_bufferCollection);
                if (FAILED(hr)) {
                    break;
                }

                for (int i = 0; i < bufCount; i++)
                {
                    VpuMemDesc *vpuDesc = m_vpuDescArray + i;
                    VpuFrameBuffer *vpuFrameBuffer = m_vpuFrameBuffers + i;

                    vpuDesc->nSize = totalSize;
                    // VPU_MEM_DESC_NORMAL allocates cachable continguous system memory. We rely on updated VPU code to invalidate
                    // the cache when it returns a buffer for output.
                    vpuDesc->nType = VPU_MEM_DESC_NORMAL;
                    retCode = VPU_DecGetMem(vpuDesc);
                    if (retCode != VPU_DEC_RET_SUCCESS)
                    {
                        break;
                    }

                    unsigned char *ptr = (unsigned char *)vpuDesc->nPhyAddr;
                    
                    // fill in frame buffer strides
                    vpuFrameBuffer->nStrideY = yStride;
                    vpuFrameBuffer->nStrideC = uStride;

                    // frame buffer physical addresses
                    vpuFrameBuffer->pbufY = ptr;
                    vpuFrameBuffer->pbufCb = ptr + ySize;
                    vpuFrameBuffer->pbufCr = ptr + ySize + uSize;
                    vpuFrameBuffer->pbufMvCol = ptr + ySize + uSize + vSize;

                    ptr = (unsigned char *)vpuDesc->nVirtAddr;
                    vpuFrameBuffer->pbufVirtY = ptr;
                    vpuFrameBuffer->pbufVirtCb = ptr + ySize;
                    vpuFrameBuffer->pbufVirtCr = ptr + ySize + uSize;
                    vpuFrameBuffer->pbufVirtMvCol = ptr + ySize + uSize + vSize;
                }

                if (VpuFailed(retCode))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Out of memory allocating frame buffers", __FUNCTION__);
                    hr = E_UNEXPECTED;
                    break;
                }

                // calling VPU_DecRegisterFrameBuffer immediately after VPU_DecDecodeBuf returns INIT_OK results
                // in MEMERROR result, must call into VPU_DecDecodeBuf() again to set raster buffer mgr and set
                // vpu code into BUFFER_NEEDED state.  
                VpuBufferNode temp = { 0 };
                retCode = VPU_DecDecodeBuf(m_vpuHandle, &temp, &bufRetCode);
                if (VpuFailed(retCode))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): 2nd VPU_DecDecodeBuf() failed %d",
                        __FUNCTION__, retCode);
                    hr = E_UNEXPECTED;
                    break;
                }

                retCode = VPU_DecRegisterFrameBuffer(m_vpuHandle, m_vpuFrameBuffers, bufCount);
                if (VpuFailed(retCode))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Out of memory allocating frame buffers", __FUNCTION__);
                    hr = E_UNEXPECTED;
                    break;
                }

                m_vpuDecInit = true;
            } // if (VPU_DEC_INIT_OK)

            if (bufRetCode & VPU_DEC_ONE_FRM_CONSUMED)
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): VPU_DEC_ONE_FRM_CONSUMED",
                    __FUNCTION__);
                VpuDecFrameLengthInfo frameInfo = { 0 };
                retCode = VPU_DecGetConsumedFrameInfo(m_vpuHandle, &frameInfo);
                if (VpuFailed(retCode))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecGetConsumedFrameInfo failed error %d",
                        __FUNCTION__, retCode);
                    hr = E_UNEXPECTED;
                    break;
                }

                //  TODO: vpuOutFrameBuffer is never set by Hantro decoder - this code is of no use.
#if 0
                vpuOutFrameBuffer = frameInfo.pFrame;

                if (!vpuOutFrameBuffer)
                {   
//                  TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecGetConsumedFrameInfo returned null output frame",
//                      __FUNCTION__);
//                  hr = E_UNEXPECTED;
//                  break;
                }
#endif
            }

            if (bufRetCode & VPU_DEC_RESOLUTION_CHANGED)
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): VPU_DEC_RESOLUTION_CHANGED",
                    __FUNCTION__);
            }

            if (m_bInterlaced != !!(m_vpuInfo.nInterlace))
            {
                // Adjust frame size alignment due to interlacing
                CAutoLock lock(&m_csLock);

                m_bInterlaced = !!m_vpuInfo.nInterlace;
                m_bStreamChange = TRUE;

                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): interlace mode changed to %d", __FUNCTION__, m_bInterlaced);
                hr = SendOutputFrameEvent(NULL);
                if (FAILED(hr))
                {
                    break;
                }
                break;
            }

            if (bufRetCode & VPU_DEC_NO_ENOUGH_BUF)
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DEC_NO_ENOUGH_BUF",
                    __FUNCTION__);
                // not enough output buffers?
                hr = E_UNEXPECTED;
                break;
            }

            if ((bufRetCode & VPU_DEC_OUTPUT_DIS) || (bufRetCode & VPU_DEC_OUTPUT_MOSAIC_DIS))
            {
                LONGLONG decodeTime = 0;
                LONGLONG framePeriod = 0;
                LONGLONG decodeTimestamp = 0;
                LONGLONG presentationTimestamp = 0;
                ULONG outputFrameCount = 0;
                VpuDecOutFrameInfo outFrameInfo = { 0 };

                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): VPU_DEC_OUTPUT_DIS/MOSAIC_DIS",
                    __FUNCTION__);
                retCode = VPU_DecGetOutputFrame(m_vpuHandle, &outFrameInfo);
                if (VpuFailed(retCode))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecGetOutputFrame failed error %d",
                        __FUNCTION__, retCode);
                    hr = E_UNEXPECTED;
                    break;
                }

                outputFrameCount = InterlockedIncrement(&m_ulOutputCounter);

                VpuPicType picType = outFrameInfo.ePicType;
                VpuFrameExtInfo *pExtInfo = outFrameInfo.pExtInfo;
                (void)picType;
                vpuOutFrameBuffer = outFrameInfo.pDisplayFrameBuf;

                if (!vpuOutFrameBuffer)
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecGetOutputFrame returned null output frame",
                        __FUNCTION__);
                    hr = E_UNEXPECTED;
                    break;
                }

                GetElapsedTime(&frameStart, &frameEnd);
                decodeTime = (LONGLONG)frameEnd.tv_sec * 1000000LL + (LONGLONG)frameEnd.tv_nsec / 1000LL;
                framePeriod = 1000000LL * (LONGLONG)m_fps.Denominator / (LONGLONG)m_fps.Numerator;
                if (decodeTime > framePeriod)
                {
                    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): vpu frame decode took longer than frame rate. us = %ld period = %ld",
                        __FUNCTION__, decodeTime, framePeriod);
                    InterlockedIncrement(&m_AboveFPS);
                }
                else if (decodeTime > 5000)
                {
                    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): vpu frame decode took longer than 5ms. us = %ld",
                        __FUNCTION__, decodeTime);
                    InterlockedIncrement(&m_Above5000);
                }
                else
                {
                    InterlockedIncrement(&m_Sub5000);
                }

                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): vpu frame decode time from VPU_DecDecodeBuf to VPU_DecGetOutputFrame took %ld us",
                    __FUNCTION__, decodeTime);

                // reset frameStart timestamp for next decoding.
                _timespec64_get(&frameStart, TIME_UTC);

                SAFERELEASE(pOutputSample);
                hr = CreateOutputSample(ulCurrentSample, pInputSample, &pOutputSample);
                if (FAILED(hr)) {
                    break;
                }

                // Use VPU Frame buffers or copy to intermediate buffer
#if defined(VPU_COPY_FRAMEBUFFER)
                hr = MFCreateMemoryBuffer(m_ulYSize + m_ulVSize + m_ulUSize, &pOutputBuffer);
                if (FAILED(hr))
                {
                    break;
                }

                BYTE *pBuffer;
                pOutputBuffer->Lock(&pBuffer, nullptr, nullptr);
                memcpy(pBuffer, vpuOutFrameBuffer->pbufVirtY, m_ulYSize + m_ulVSize + m_ulUSize);
                pOutputBuffer->Unlock();

                retCode = VPU_DecOutFrameDisplayed(m_vpuHandle, vpuOutFrameBuffer);
#else
                hr = m_bufferCollection->CreateBufferInstance(m_vpuHandle, dwDataLen, vpuOutFrameBuffer, &pOutputBuffer);
                if (FAILED(hr))
                {
                    break;
                }
#endif

#if 0
                // DEBUG advancing line indicator
                ULONG * pFrame = (ULONG *)vpuOutFrameBuffer->pbufVirtY;

                for (int i = 16; i < 32; i++)
                {
                    int offset = outputFrameCount % 16;
                    for (int j = 0; j < 16; j++)
                    {
                        pFrame[(i * vpuOutFrameBuffer->nStrideY) / sizeof(ULONG) + j + 40] = 0;
                    }
                    pFrame[(i * vpuOutFrameBuffer->nStrideY) / sizeof(ULONG) + offset + 40] = 0x00FFFF00;
                }
#endif

                hr = pOutputBuffer->SetCurrentLength(m_ulYSize + m_ulVSize + m_ulUSize);
                if (FAILED(hr))
                {
                    break;
                }

                hr = pOutputSample->AddBuffer(pOutputBuffer);
                if (FAILED(hr))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecOutFrameDisplayed failed error %d",
                        __FUNCTION__, retCode);
                    hr = E_UNEXPECTED;
                    break;
                }

                SAFERELEASE(pOutputBuffer);

                hr = UpdateSampleTime(ulCurrentSample, pOutputSample);
                if (FAILED(hr))
                {
                    break;
                }

                // VPU_DecOutFrameDisplayed is called when pOutputBuffer is released
                hr = SendOutputFrameEvent(pOutputSample);
                if (FAILED(hr))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Failed to output sample (hr=0x%x)", __FUNCTION__, hr);
                    break;
                }
                SAFERELEASE(pOutputSample);
            }

            if (bufRetCode & VPU_DEC_NO_ENOUGH_INBUF)
            {
                needInput = true;
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): VPU_DEC_NO_ENOUGH_INBUF",
                    __FUNCTION__);
            }

            if (bufRetCode & VPU_DEC_FLUSH)
            {
                TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): VPU_DEC_FLUSH",
                    __FUNCTION__);
                retCode = VPU_DecFlushAll(m_vpuHandle);
                if (VpuFailed(retCode))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecFlushAll failed error %d",
                        __FUNCTION__, retCode);
                    hr = E_UNEXPECTED;
                    break;
                }
            }
        } while (!needInput);

        if (FAILED(hr))
        {
            break;
        }

        if(pInputSample->GetUINT64(VpuMft_MFSampleExtension_Marker, &pun64MarkerID) == S_OK)
        {
            // This input sample flagged a marker
            IMFMediaEvent*  pMarkerEvent    = NULL;

            do
            {
                hr = MFCreateMediaEvent(METransformMarker, GUID_NULL, S_OK, NULL, &pMarkerEvent);
                if(FAILED(hr))
                {
                    break;
                }

                hr = pMarkerEvent->SetUINT64(MF_EVENT_MFT_CONTEXT, pun64MarkerID);
                if(FAILED(hr))
                {
                    break;
                }

                /***************************************
                ** Since this in an internal function
                ** we know m_pEventQueue can never be
                ** NULL due to InitializeTransform()
                ***************************************/

                hr = m_pEventQueue->QueueEvent(pMarkerEvent);
                if(FAILED(hr))
                {
                    break;
                }
            }while(false);

            SAFERELEASE(pMarkerEvent);

            if(FAILED(hr))
            {
                break;
            }
        }

        // Done processing this sample, request another if required
        if (needInput)
        {
            CAutoLock lock(&m_csLock);

            // CVpuMft::ProcessOutput causes the next sample to be requested. Only if we don't expect it to be called do
            // we request a sample here.
            if (!(m_dwStatus & MYMFT_STATUS_DRAINING) && (m_dwHaveOutputCount == 0) && (m_dwPendingFrameDecodeCount == 1))
            {
                hr = RequestSample(0);
                if (FAILED(hr))
                {
                    TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): RequestSample failed (hr=0x%x)", __FUNCTION__, hr);
                    break;
                }
                break;
            }
        }
    }while(false);

    SAFERELEASE(pOutputBuffer);
    SAFERELEASE(pOutputSample);

    if (pInputBuffer != NULL)
    {
        if (pbInputData != NULL)
        {
            pInputBuffer->Unlock();
        }
        SAFERELEASE(pInputBuffer);
    }

    {
        CAutoLock lock(&m_csLock);
        m_dwPendingFrameDecodeCount--;

        if ((m_dwStatus & MYMFT_STATUS_DRAINING) && m_dwPendingFrameDecodeCount == 0)
        {
            hr = SendDrainCompleteEvent();
        }
    }

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

HRESULT CVpuMft::SendOutputFrameEvent(IMFSample* pOutputSample)
{
    HRESULT hr;
    IMFMediaEvent*  pHaveOutputEvent = NULL;

    do {
        if (pOutputSample != NULL)
        {
            hr = m_pOutputSampleQueue->AddSample(pOutputSample);
            if (FAILED(hr))
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Failed to add sample to output queue (hr=0x%x)", __FUNCTION__, hr);
                break;
            }
        }
        else
        {
            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Sending empty output frame for format change", __FUNCTION__);
        }

        // Now that the sample is in the output queue, send out have output event
        hr = MFCreateMediaEvent(METransformHaveOutput, GUID_NULL, S_OK, NULL, &pHaveOutputEvent);
        if (FAILED(hr))
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Failed to create METransformHaveOutput event (hr=0x%x)", __FUNCTION__, hr);
            break;
        }

        {
            CAutoLock lock(&m_csLock);

            hr = m_pEventQueue->QueueEvent(pHaveOutputEvent);
            if (FAILED(hr))
            {
                // If this fails, consider decrementing m_dwHaveOutputCount
                TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Failed to queue METransformHaveOutput event (hr=0x%x)", __FUNCTION__, hr);
                break;
            }

            m_dwHaveOutputCount++;

            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): HaveOutputCount: %u", __FUNCTION__, m_dwHaveOutputCount);

            m_dwStatus |= MYMFT_STATUS_OUTPUT_SAMPLE_READY;
        }
    } while (FALSE);

    SAFERELEASE(pHaveOutputEvent);

    return hr;
}

BOOL CVpuMft::IsLocked(void)
{
    /***************************************
    ** Since this in an internal function
    ** we know m_pAttributes can never be
    ** NULL due to InitializeTransform()
    ***************************************/

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    BOOL bUnlocked  = MFGetAttributeUINT32(m_pAttributes,
        MF_TRANSFORM_ASYNC_UNLOCK,
        FALSE
        );

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(%s)", __FUNCTION__, (bUnlocked != FALSE) ? L"False" : L"True");

    return (bUnlocked != FALSE) ? FALSE : TRUE;
}

BOOL CVpuMft::IsMFTReady(void)
{
    /*******************************
    ** The purpose of this function
    ** is to ensure that the MFT is
    ** ready for processing
    *******************************/

    BOOL bReady = FALSE;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        CAutoLock lock(&m_csLock);

        m_dwStatus &= (~MYMFT_STATUS_INPUT_ACCEPT_DATA);

        if(m_pInputMT == NULL)
        {
            // The Input type is not set
            break;
        }

        if(m_pOutputMT == NULL)
        {
            // The output type is not set
            break;
        }

        m_dwStatus |= MYMFT_STATUS_INPUT_ACCEPT_DATA; // The MFT is ready for data

        bReady = TRUE;
    }while(false);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(%s)", __FUNCTION__, bReady ? L"True" : L"False");

    return bReady;
}

HRESULT CVpuMft::AllocVpuMem()
{
    int i;
    void * p;
    HRESULT hr = S_OK;

    for (i = 0; i < m_vpuMemInfo.nSubBlockNum; i++)
    {
        VpuMemSubBlockInfo * subBlock = &(m_vpuMemInfo.MemSubBlock[i]);
        VpuMemDesc vpuMem = { 0 };

        if (subBlock->MemType == VPU_MEM_VIRT)
        {
            vpuMem.nSize = subBlock->nSize + subBlock->nAlignment;
            p = malloc(vpuMem.nSize);
            if (p == NULL)
            {
                hr = E_OUTOFMEMORY;
                TraceString(CHMFTTracing::TRACE_ERROR, L"malloc(size %d) returned NULL", vpuMem.nSize);
                goto exit;
            }

            vpuMem.nVirtAddr = (addr_t)p;

            subBlock->pVirtAddr = (unsigned char *)Align(p, m_vpuMemInfo.MemSubBlock[i].nAlignment);
        }
        else // VPU_MEM_PHY
        {
            VpuDecRetCode vpuRes;
            vpuMem.nSize = subBlock->nSize + subBlock->nAlignment;
            // TODO: HACK!  use VPU_MEM_DESC_SECURE to indicate create write-combined uncached memory.
            vpuMem.nType = VPU_MEM_DESC_SECURE;
            vpuRes = VPU_DecGetMem(&vpuMem);
            if (vpuRes != VPU_DEC_RET_SUCCESS)
            {
                hr = E_OUTOFMEMORY;
                TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecGetMem(size %d) returned error %d", vpuMem.nSize, vpuRes);
                goto exit;
            }

            subBlock->pVirtAddr = (unsigned char*)Align((void *)(intptr_t)vpuMem.nVirtAddr, subBlock->nAlignment);
            subBlock->pPhyAddr = (unsigned char*)Align((void *)(intptr_t)vpuMem.nPhyAddr, subBlock->nAlignment);
        }
        m_vpuMemBlocks[i] = vpuMem;
    }

exit:
    if (FAILED(hr))
    {
        FreeVpuMem();
    }
    return hr;
}

HRESULT CVpuMft::FreeVpuMem()
{
    HRESULT hr = S_OK;
    int i;
    for (i = 0; i < m_vpuMemInfo.nSubBlockNum; i++)
    {
        VpuMemSubBlockInfo * subBlock = &(m_vpuMemInfo.MemSubBlock[i]);
        VpuMemDesc * pMemDesc = m_vpuMemBlocks + i;

        if (pMemDesc->nVirtAddr != NULL)
        {
            if (subBlock->MemType == VPU_MEM_VIRT)
            {
                free((void *)pMemDesc->nVirtAddr);
            }
            else
            {
                VpuDecRetCode vpuRes = VPU_DecFreeMem(pMemDesc);
                if (vpuRes != VPU_DEC_RET_SUCCESS)
                {
                    hr = E_UNEXPECTED;
                    TraceString(CHMFTTracing::TRACE_ERROR, L"VPU_DecFreeMem(memDesc) returned error %d", vpuRes);
                }
            }
            memset(pMemDesc, 0, sizeof(VpuMemDesc));
            memset(subBlock, 0, sizeof(VpuMemSubBlockInfo));
        }
        else
        {
            hr = E_UNEXPECTED;
            FAILED(hr);
        }
    }
    return hr;
}

HRESULT CVpuMft::UpdateSampleTime(ULONG ulSampleIdx, IMFSample *pSample)
{
    HRESULT hr = S_OK;
    LONGLONG llSampleDuration;

    do {
        CAutoLock lock(&m_csLock);

        if (m_bFirstSample != FALSE)
        {
            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Sample %u @%p is discontinuity", __FUNCTION__, ulSampleIdx, pSample);

            hr = pSample->SetUINT32(MFSampleExtension_Discontinuity, TRUE);
            if (FAILED(hr))
            {
                break;
            }

            m_bFirstSample = FALSE;
        }

        if (FAILED(pSample->GetSampleDuration(&llSampleDuration)))
        {
            llSampleDuration = MFT_DEFAULT_SAMPLE_DURATION;
        }
// TODO: Can use this to slow down playback.
//        m_llNextSampleTime += llSampleDuration * 4;
        m_llNextSampleTime += llSampleDuration;
    } while (FALSE);

    return hr;
}

HRESULT CVpuMft::CreateOutputSample(ULONG ulSampleIdx, IMFSample *pInputSample, IMFSample **pOutputSample)
{
    HRESULT hr = S_OK;
    IMFSample* newSample = nullptr;
    LONGLONG llSampleTime;
    LONGLONG llSampleDuration;

    do {
        hr = MFCreateSample(&newSample);
        if (FAILED(hr))
        {
            break;
        }

        hr = DuplicateAttributes(newSample, pInputSample);
        if (FAILED(hr))
        {
            break;
        }

        if (FAILED(pInputSample->GetSampleDuration(&llSampleDuration)))
        {
            llSampleDuration = MFT_DEFAULT_SAMPLE_DURATION;
        }

        hr = newSample->SetSampleDuration(llSampleDuration);
        if (FAILED(hr))
        {
            break;
        }

        {
            CAutoLock lock(&m_csLock);

            if (m_llNextSampleTime == 0 || IsKeyFrame(pInputSample))
            {
                // TODO: this is using the DTS (decode timestamp) value to set the PTS
                // (presentation timestamp value).  They are not necessarily the same.
                if (SUCCEEDED(pInputSample->GetSampleTime(&llSampleTime)))
                {
                    m_llNextSampleTime = llSampleTime;
                }
            }
            llSampleTime = m_llNextSampleTime;
        }

        hr = newSample->SetSampleTime(llSampleTime);
        if (FAILED(hr))
        {
            break;
        }

        *pOutputSample = newSample;
        newSample = nullptr;

        struct _timespec64 clocktime;
        GetElapsedTime(&m_clockStart, &clocktime);
        TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Output Sample %u @%p Created: Duration %I64u, Sample Time %I64d.%04I64d, Clock Time %d.%04d",
            __FUNCTION__, ulSampleIdx, *pOutputSample, llSampleDuration / MFT_MILLISECOND_TIMEBASE,
            (llSampleTime / MFT_MILLISECOND_TIMEBASE) / 1000, (llSampleTime / MFT_MILLISECOND_TIMEBASE) % 1000,
            clocktime.tv_sec, clocktime.tv_nsec / 1000000);
    } while (FALSE);

    SAFERELEASE(newSample);

    return hr;
}

int CVpuMft::Align(int i, unsigned int align)
{
    return ((i + (align - 1)) / align) * align;
}

void * CVpuMft::Align(void *ptr, unsigned int align)
{
    return (void *)((((intptr_t)ptr + (align - 1)) / align) * align);
}

BOOL CVpuMft::IsKeyFrame(IMFSample* pSample)
{
    UINT32 value = 0;

    if (pSample->GetUINT32(MFSampleExtension_CleanPoint, &value) == S_OK)
    {
        return !!value;
    }
    return false;
}

BOOL CVpuMft::IsInterlaced(UINT32 interlaceMode)
{
    switch (interlaceMode)
    {
    case MFVideoInterlace_FieldInterleavedUpperFirst:
    case MFVideoInterlace_FieldInterleavedLowerFirst:
    case MFVideoInterlace_FieldSingleUpper:
    case MFVideoInterlace_FieldSingleLower:
        return TRUE;

    // video stream defines interlaced/progressive setting
    case MFVideoInterlace_MixedInterlaceOrProgressive:
        return TRUE;

    case MFVideoInterlace_Progressive:
    case MFVideoInterlace_Unknown:
    default:
        return FALSE;
    }
}

HRESULT DuplicateAttributes(
    IMFAttributes*  pDest,
    IMFAttributes*  pSource)
{
    HRESULT     hr      = S_OK;
    GUID        guidKey = GUID_NULL;
    PROPVARIANT pv      = {0};

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Enter",  __FUNCTION__);

    do
    {
        if((pDest == NULL) || (pSource == NULL))
        {
            hr = E_POINTER;
            break;
        }

        PropVariantInit(&pv);

        for(UINT32 un32Index = 0; true; un32Index++)
        {
            PropVariantClear(&pv);
            PropVariantInit(&pv);

            hr = pSource->GetItemByIndex(un32Index, &guidKey, &pv);
            if (hr == E_INVALIDARG)
            {
                // all items copied
                hr = S_OK;
                break;
            }

            if(FAILED(hr))
            {
                break;
            }

            hr = pDest->SetItem(guidKey, pv);
            if(FAILED(hr))
            {
                break;
            }
        }
    }while(false);

    PropVariantClear(&pv);

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Exit(hr=0x%x)", __FUNCTION__, hr);

    return hr;
}

bool MyFailed(HRESULT hr)
{
    if (hr < 0)
        return TRUE;
    else
        return FALSE;
}

bool VpuFailed(VpuDecRetCode result)
{
    if (result != VPU_DEC_RET_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void GetElapsedTime(struct _timespec64 *startTime, struct _timespec64 *elapsedTime)
{
    struct _timespec64 now;
    _timespec64_get(&now, TIME_UTC);

    elapsedTime->tv_nsec = now.tv_nsec - startTime->tv_nsec;
    elapsedTime->tv_sec = now.tv_sec - startTime->tv_sec;

    if (now.tv_nsec - startTime->tv_nsec < 0)
    {
        elapsedTime->tv_nsec += 1000000000;
        elapsedTime->tv_sec -= 1;
    }
}
