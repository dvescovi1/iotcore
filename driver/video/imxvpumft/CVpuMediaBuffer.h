/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#pragma once

#include "vpumft.h"
#include <Mfidl.h>

class CVpuMediaBuffer: public IMFMediaBuffer
{
public:
    CVpuMediaBuffer(IVpuBufferCollection *collection, VpuDecHandle vpuHandle, UINT32 maxSize, VpuFrameBuffer* frameBuffer);

#pragma region IUnknown
    // IUnknown Implementation
    ULONG   __stdcall AddRef(void);
    HRESULT __stdcall QueryInterface(
        REFIID  riid,
        void**  ppvObject
        );
    ULONG   __stdcall Release(void);
#pragma endregion IUnknown

#pragma region IMFMediaBuffer
    // IMFMediaBuffer Implementation
    HRESULT __stdcall Lock(
        /* [annotation][out] */
        _Outptr_result_bytebuffer_to_(*pcbMaxLength, *pcbCurrentLength)  BYTE **ppbBuffer,
        /* [annotation][out] */
        _Out_opt_  DWORD *pcbMaxLength,
        /* [annotation][out] */
        _Out_opt_  DWORD *pcbCurrentLength);

    HRESULT __stdcall Unlock(void);

    HRESULT __stdcall GetCurrentLength(
        /* [annotation][out] */
        _Out_  DWORD *pcbCurrentLength);

    HRESULT __stdcall SetCurrentLength(
        /* [in] */ DWORD cbCurrentLength);

    HRESULT __stdcall GetMaxLength(
        /* [annotation][out] */
        _Out_  DWORD *pcbMaxLength);
#pragma endregion IMFMediaBuffer

protected:
    CVpuMediaBuffer(void);
    ~CVpuMediaBuffer(void);

    volatile static ULONG m_ulInUse;
    volatile static ULONG m_ulMaxInUse;

    volatile ULONG m_ulRef;
    VpuDecHandle m_vpuHandle;
    VpuFrameBuffer *m_pVpuFrameBuffer;
    UINT32 m_MaxSize;
    UINT32 m_Size;
    CRITICAL_SECTION m_csLock;
    IVpuBufferCollection* m_Collection;
};

class CVpuBufferCollection : public IVpuBufferCollection
{
public:
    static HRESULT CreateInstance(
        _In_ IVpuMft *vpuMft,
        _In_ ULONG totalBuffers,
        _In_ ULONG minVpuBuffers,
        _Out_ IVpuBufferCollection **collection);

#pragma region IUnknown
    // IUnknown Implementation
    ULONG   __stdcall AddRef(void);
    HRESULT __stdcall QueryInterface(
        REFIID  riid,
        void**  ppvObject
    );
    ULONG   __stdcall Release(void);
#pragma endregion IUnknown

    HRESULT __stdcall CreateBufferInstance(
        _In_ VpuDecHandle vpuHandle,
        _In_ UINT32 maxLength,
        _In_ VpuFrameBuffer* frameBuffer,
        _Out_ IMFMediaBuffer** ppBuffer);

    HRESULT __stdcall MarkBufferFree(
        _In_ IMFMediaBuffer * buffer);

    HRESULT __stdcall ScheduleFrameDecode(void);

    HRESULT __stdcall FrameDecodeError(void);

protected:
    CVpuBufferCollection(IVpuMft *mft, ULONG totalBuffers, ULONG minBuffers);
    ~CVpuBufferCollection();

    volatile ULONG m_ulRef;

    // do not refcount m_Mft. CVpuBufferCollection is created by CVpuMft and would cause circular reference.
    IVpuMft* m_Mft;
    ULONG    m_totalBuffers;
    ULONG    m_minVpuBuffers;
    HRESULT  m_scheduleError;
    CRITICAL_SECTION m_csLock;

    volatile ULONG m_inUse;
    volatile ULONG m_maxInUse;
    volatile ULONG m_freeBuffers;
    volatile ULONG m_deferredFrameDecodes;
};
