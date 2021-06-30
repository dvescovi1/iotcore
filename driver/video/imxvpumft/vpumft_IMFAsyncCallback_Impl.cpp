/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "vpumft.h"

HRESULT CVpuMft::GetParameters(
    DWORD*  pdwFlags,
    DWORD*  pdwQueue)
{
    HRESULT hr = S_OK;

    do
    {
        if((pdwFlags == NULL) || (pdwQueue == NULL))
        {
            hr = E_POINTER;
            break;
        }

        (*pdwFlags) = 0;
        (*pdwQueue) = m_dwDecodeWorkQueueID;
    }while(false);

    return hr;
}

HRESULT CVpuMft::Invoke(
    IMFAsyncResult *pAsyncResult)
{
    /*********************************
    ** Todo: This function is called
    ** when you schedule an async event
    ** Determine the event type from
    ** the result and take appropriate
    ** action
    *********************************/
    
    HRESULT hr = S_OK;

    do
    {
        if(pAsyncResult == NULL)
        {
            hr = E_POINTER;
            break;
        }
    }while(false);

    return hr;
}