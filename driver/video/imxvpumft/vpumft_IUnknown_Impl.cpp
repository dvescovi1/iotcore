/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "vpumft.h"
#include "ivpumft.h"

ULONG CVpuMft::AddRef(void)
{
    return InterlockedIncrement(&m_ulRef);
}

HRESULT CVpuMft::QueryInterface(
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

        /****************************************************
        ** Todo: add all supported interfaces by your MFT
        ****************************************************/
        if(riid == IID_IMFTransform)
        {
            *ppvObject = (IMFTransform*)this;
        }
        else if(riid == IID_IMFAttributes)
        {
            *ppvObject = (IMFAttributes*)this;
        }
        else if(riid == IID_IMFShutdown)
        {
            *ppvObject = (IMFShutdown*)this;
        }
        else if(riid == IID_IMFMediaEventGenerator)
        {
            *ppvObject = (IMFMediaEventGenerator*)this;
        }
        else if(riid == IID_IMFAsyncCallback)
        {
            *ppvObject = (IMFAsyncCallback*)this;
        }
        else if(riid == IID_IMFQualityAdvise)
        {
            *ppvObject = (IMFQualityAdvise*)this;
        }
        else if(riid == IID_IVPUMFT)
        {
            *ppvObject = (IVpuMft*)this;
        }
        else if(riid == IID_IUnknown)
        {
            *ppvObject = this;
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

ULONG CVpuMft::Release(void)
{
    ULONG   ulRef = 0;
    
    if(m_ulRef > 0)
    {
        ulRef = InterlockedDecrement(&m_ulRef);
    }

    if(ulRef == 0)
    {
        delete this;
    }

    return ulRef;
}