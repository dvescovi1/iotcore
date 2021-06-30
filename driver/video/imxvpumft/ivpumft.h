/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#pragma once

#include <Mfidl.h>

// {B47516A2-FA53-4B38-B1FA-CAFD12D030A8}
DEFINE_GUID(IID_IVPUMFT,
0xb47516a2, 0xfa53, 0x4b38, 0xb1, 0xfa, 0xca, 0xfd, 0x12, 0xd0, 0x30, 0xa8);

// {AB1AD049-3E60-4D93-852D-F5CBF6736EAD}
DEFINE_GUID(IID_IVpuBufferCollection,
    0xab1ad049, 0x3e60, 0x4d93, 0x85, 0x2d, 0xf5, 0xcb, 0xf6, 0x73, 0x6e, 0xad);

class IVpuMft:
    public IUnknown
{
public:
    virtual HRESULT __stdcall DecodeInputFrame(
                                  IMFSample*  pInputSample
                                  )  = 0;

    virtual HRESULT __stdcall ScheduleFrameDecode(void) = 0;
};

class IVpuBufferCollection:
    public IUnknown
{
public:
    virtual HRESULT __stdcall CreateBufferInstance(
                                _In_ VpuDecHandle vpuHandle,
                                _In_ UINT32 maxLength,
                                _In_ VpuFrameBuffer* frameBuffer,
                                _Out_ IMFMediaBuffer** ppBuffer) = 0;

    virtual HRESULT __stdcall MarkBufferFree(
                                _In_ IMFMediaBuffer * buffer) = 0;

    virtual HRESULT __stdcall ScheduleFrameDecode(void) = 0;

    virtual HRESULT __stdcall FrameDecodeError(void) = 0;
};
