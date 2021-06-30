/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "CAutoLock.h"
#include "vpumft.h"
#include <tracing.h>

extern bool VpuFailed(VpuDecRetCode result);

static void * Align(void *ptr, unsigned int align)
{
    return (void *)((((intptr_t)ptr + (align - 1)) / align) * align);
}

ULONG CVpuMediaBuffer::AddRef(void)
{
    return InterlockedIncrement(&m_ulRef);
}

HRESULT CVpuMediaBuffer::QueryInterface(
    REFIID riid,
    void** ppvObject)
{
    HRESULT hr = S_OK;

    do
    {
        if(ppvObject == NULL)
        {
            hr = E_POINTER;
            break;
        }

        if(riid == IID_IUnknown)
        {
            *ppvObject = (IUnknown*)this;
        }
        if (riid == IID_IMFMediaBuffer)
        {
            *ppvObject = (IMFMediaBuffer*)this;
        }
        else
        {
            *ppvObject = NULL;
            hr = E_NOINTERFACE;
            break;
        }

        AddRef();
    }while(false);

    return hr;
}

ULONG CVpuMediaBuffer::Release(void)
{
    ULONG ulRef = 0;
    
    if(m_ulRef > 0)
    {
        ulRef = InterlockedDecrement(&m_ulRef);
    }

    if(ulRef == 0)
    {
        m_Collection->MarkBufferFree(this);
        delete this;
    }

    return ulRef;
}

HRESULT CVpuMediaBuffer::Lock(
        BYTE **ppbBuffer,  // [OutPtr]
        DWORD *pcbMaxLength,  // [Out optional]
        DWORD *pcbCurrentLength) // [Out optional]

{
    HRESULT hr          = S_OK;

    TraceString(CHMFTTracing::TRACE_MEMORY, L"%S(): Enter",  __FUNCTION__);

    do
    {
        if (ppbBuffer == nullptr)
        {
            hr = E_POINTER;
            break;
        }

        (*ppbBuffer) = (unsigned char*)m_pVpuFrameBuffer->pbufVirtY;

        if (pcbMaxLength != nullptr)
        {
            *pcbMaxLength = (DWORD)m_MaxSize;
        }

        if (pcbCurrentLength != nullptr)
        {
            *pcbCurrentLength = (DWORD)m_Size;
        }
    }while(false);

    TraceString(CHMFTTracing::TRACE_MEMORY, L"%S(): Exit (hr=0x%x)",  __FUNCTION__, hr);

    return hr;
}


HRESULT CVpuMediaBuffer::Unlock(void)
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CVpuMediaBuffer::GetCurrentLength(
    DWORD *pcbCurrentLength) // [out]
{
    HRESULT hr = S_OK;

    if (pcbCurrentLength == NULL)
    {
        return E_POINTER;
    }

    *pcbCurrentLength = (DWORD)m_Size;

    return hr;
}


HRESULT CVpuMediaBuffer::SetCurrentLength(
    DWORD cbCurrentLength)
{
    HRESULT hr = S_OK;

    if (cbCurrentLength <= m_MaxSize)
    {
        m_Size = cbCurrentLength;
    }
    else
    {
        hr = E_INVALIDARG;
    }

    return hr;
}

HRESULT CVpuMediaBuffer::GetMaxLength(
    DWORD *pcbMaxLength) // [out]
{
    HRESULT hr = S_OK;

    if (pcbMaxLength == NULL)
    {
        return E_POINTER;
    }

    *pcbMaxLength = (DWORD)m_MaxSize;

    return hr;
}


CVpuMediaBuffer::CVpuMediaBuffer(void) :
    m_Collection(nullptr), m_ulRef(1), m_Size(0), m_MaxSize(0), m_vpuHandle(nullptr)
{
    InitializeCriticalSection(&m_csLock);
}

CVpuMediaBuffer::CVpuMediaBuffer(
    IVpuBufferCollection *collection,
    VpuDecHandle vpuHandle,
    UINT32 maxSize,
    VpuFrameBuffer* frameBuffer) :
    m_Collection(collection), m_ulRef(1), m_Size(0), m_MaxSize(maxSize), m_vpuHandle(vpuHandle), m_pVpuFrameBuffer(frameBuffer)
{
    InitializeCriticalSection(&m_csLock);
    m_Collection->AddRef();
}

CVpuMediaBuffer::~CVpuMediaBuffer(void)
{
    if (m_vpuHandle != nullptr)
    {
        VpuDecRetCode retCode;

        retCode = VPU_DecOutFrameDisplayed(m_vpuHandle, m_pVpuFrameBuffer);
        if (VpuFailed(retCode))
        {
            TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): VPU_DecOutFrameDisplayed failed error %d", retCode);
        }
    }
    m_pVpuFrameBuffer = nullptr;
    SAFERELEASE(m_Collection);
    DeleteCriticalSection(&m_csLock);
}

//
// CvpuBufferCollection class implementation
//

CVpuBufferCollection::CVpuBufferCollection(IVpuMft *mft, ULONG totalBuffers, ULONG minBuffers) :
    m_ulRef(0), m_Mft(mft), m_minVpuBuffers(minBuffers), m_totalBuffers(totalBuffers), m_inUse(0), m_maxInUse(0),
    m_deferredFrameDecodes(0), m_freeBuffers(totalBuffers), m_scheduleError(S_OK)
{
    InitializeCriticalSection(&m_csLock);
}

CVpuBufferCollection::~CVpuBufferCollection()
{
    DeleteCriticalSection(&m_csLock);
}

HRESULT CVpuBufferCollection::CreateInstance(
    _In_ IVpuMft *vpuMft,
    _In_ ULONG totalBuffers,
    _In_ ULONG minVpuBuffers,
    _Out_ IVpuBufferCollection **collection)
{
    *collection = new CVpuBufferCollection(vpuMft, totalBuffers, minVpuBuffers);

    (*collection)->AddRef();

    return S_OK;
}

HRESULT CVpuBufferCollection::QueryInterface(
    REFIID riid,
    void** ppvObject)
{
    HRESULT hr = S_OK;

    do
    {
        if (ppvObject == NULL)
        {
            hr = E_POINTER;
            break;
        }

        if (riid == IID_IUnknown)
        {
            *ppvObject = (IUnknown*)this;
        }
        if (riid == IID_IVpuBufferCollection)
        {
            *ppvObject = (IVpuBufferCollection*)this;
        }
        else
        {
            *ppvObject = NULL;
            hr = E_NOINTERFACE;
            break;
        }

        AddRef();
    } while (false);

    return hr;
}

ULONG CVpuBufferCollection::AddRef(void)
{
    return InterlockedIncrement(&m_ulRef);
}

ULONG CVpuBufferCollection::Release(void)
{
    ULONG ulRef = 0;

    if (m_ulRef > 0)
    {
        ulRef = InterlockedDecrement(&m_ulRef);
    }

    if (ulRef == 0)
    {
        delete this;
    }

    return ulRef;
}

HRESULT CVpuBufferCollection::CreateBufferInstance(
    VpuDecHandle vpuHandle, // [in]
    UINT32 maxLength,       // [in]
    VpuFrameBuffer *frameBuffer,    // [in]
    IMFMediaBuffer** ppBuffer)  // [out]
{
    HRESULT hr = S_OK;
    CVpuMediaBuffer* pNewBuffer = NULL;

    do
    {
        if (ppBuffer == NULL)
        {
            hr = E_POINTER;
            break;
        }

        if (frameBuffer == NULL)
        {
            hr = E_INVALIDARG;
            break;
        }

        pNewBuffer = new CVpuMediaBuffer(this, vpuHandle, maxLength, frameBuffer);
        if (pNewBuffer == NULL)
        {
            hr = E_OUTOFMEMORY;
            break;
        }

        (*ppBuffer) = pNewBuffer;
        (*ppBuffer)->AddRef();

        ULONG instances;

        instances = InterlockedIncrement(&m_inUse);
        if (instances > m_maxInUse)
        {
            InterlockedIncrement(&m_maxInUse);
            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): max buffers in use has grown to %d",
                __FUNCTION__, instances);
        }

        ULONG free;

        free = InterlockedDecrement(&m_freeBuffers);
        if (free < m_minVpuBuffers - 1)
        {
            // We warn if we dip more than one into the reserved vpu buffer space
            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): buffer collection is dipping into vpu buffer reserved space. inuse = %d, total = %d, reserved = %d",
                __FUNCTION__, instances, m_totalBuffers, m_minVpuBuffers);
        }
    } while (false);

    SAFERELEASE(pNewBuffer);

    return hr;
}

HRESULT CVpuBufferCollection::MarkBufferFree(
    _In_ IMFMediaBuffer * buffer)
{
    ULONG deferred;

    InterlockedDecrement(&m_inUse);
    InterlockedIncrement(&m_freeBuffers);

    {
        CAutoLock lock(&m_csLock);

        deferred = m_deferredFrameDecodes;
        if (deferred > 0)
        {
            HRESULT hr;

            TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Queuing deferring frame decode. deferred = %d",
                __FUNCTION__, m_freeBuffers, m_minVpuBuffers, deferred);
            hr = m_Mft->ScheduleFrameDecode();
            if (FAILED(hr))
            {
                TraceString(CHMFTTracing::TRACE_ERROR, L"%S(): Failed to queue deferred frame decode (hr=0x%x)", __FUNCTION__, hr);
                m_scheduleError = hr;
            }
            else
            {
                InterlockedDecrement(&m_deferredFrameDecodes);
            }
        }
    }
    return S_OK;
}


HRESULT CVpuBufferCollection::ScheduleFrameDecode(void)
{
    HRESULT hr = S_OK;

    if (m_freeBuffers > m_minVpuBuffers)
    {
        hr = m_Mft->ScheduleFrameDecode();
    }
    else
    {
        ULONG deferred;

        deferred = InterlockedIncrement(&m_deferredFrameDecodes);
        TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): Deferring frame decode. Free buffers = %d, min vpu = %d, deferred = %d",
            __FUNCTION__, m_freeBuffers, m_minVpuBuffers, deferred);
    }

    return hr;
}

HRESULT CVpuBufferCollection::FrameDecodeError(void)
{
    HRESULT hr;

    hr = m_scheduleError;
    m_scheduleError = S_OK;

    return hr;
}
