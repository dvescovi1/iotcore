/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "vpumft.h"
#include "CAutoLock.h"
#include <mferror.h>
#include <mfapi.h>

HRESULT CVpuMft::GetShutdownStatus(
    MFSHUTDOWN_STATUS*  pStatus)
{
    HRESULT hr = S_OK;

    do
    {
        if(pStatus == NULL)
        {
            hr = E_POINTER;
            break;
        }

        {
            CAutoLock lock(&m_csLock);

            if(m_bShutdown == FALSE)
            {
                hr = MF_E_INVALIDREQUEST;
                break;
            }

            *pStatus = MFSHUTDOWN_COMPLETED;
        }
    }while(false);

    return hr;
}

HRESULT CVpuMft::Shutdown(void)
{
    HRESULT hr = S_OK;

    do
    {
        CAutoLock lock(&m_csLock);

        hr = ShutdownEventQueue();
        if(FAILED(hr))
        {
            break;
        }

        hr = MFUnlockWorkQueue(m_dwDecodeWorkQueueID);
        if(FAILED(hr))
        {
            break;
        }

        m_bShutdown = TRUE;
    }while(false);

    return hr;
}
