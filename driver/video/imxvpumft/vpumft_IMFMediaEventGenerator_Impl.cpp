/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "vpumft.h"
#include <Mfapi.h>

HRESULT CVpuMft::BeginGetEvent(
    IMFAsyncCallback*   pCallback,
    IUnknown*           punkState)
{
    /***************************************
    ** Since this in an internal function
    ** we know m_pEventQueue can never be
    ** NULL due to InitializeTransform()
    ***************************************/

    return m_pEventQueue->BeginGetEvent(pCallback, punkState);
}

HRESULT CVpuMft::EndGetEvent(
    IMFAsyncResult* pResult,
    IMFMediaEvent** ppEvent)
{
    /***************************************
    ** Since this in an internal function
    ** we know m_pEventQueue can never be
    ** NULL due to InitializeTransform()
    ***************************************/

    return m_pEventQueue->EndGetEvent(pResult, ppEvent);
}

HRESULT CVpuMft::GetEvent(
    DWORD           dwFlags,
    IMFMediaEvent** ppEvent)
{
    /***************************************
    ** Since this in an internal function
    ** we know m_pEventQueue can never be
    ** NULL due to InitializeTransform()
    ***************************************/

    return m_pEventQueue->GetEvent(dwFlags, ppEvent);
}

HRESULT CVpuMft::QueueEvent(
    MediaEventType      met,
    REFGUID             guidExtendedType,
    HRESULT             hrStatus,
    const PROPVARIANT*  pvValue)
{
    /***************************************
    ** Since this in an internal function
    ** we know m_pEventQueue can never be
    ** NULL due to InitializeTransform()
    ***************************************/

    return m_pEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
}