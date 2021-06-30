/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "vpumft.h"
#include "CAutoLock.h"
#include "CVpuMediaBuffer.h"
#include <mfapi.h>
#include <mferror.h>
#include <tracing.h>

#include <initguid.h>
HRESULT CVpuMft::SetDropMode(
    /* [in] */ MF_QUALITY_DROP_MODE eDropMode)
{
    if (this->m_bShutdown)
    {
        return MF_E_SHUTDOWN;
    }

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): value=%d", __FUNCTION__, (int)eDropMode);

    CAutoLock lock(&m_csLock);

    switch (eDropMode)
    {
    case MF_DROP_MODE_NONE:
    case MF_DROP_MODE_1:
    case MF_DROP_MODE_2:
        m_dropMode = eDropMode;
        break;

    default:
        return MF_E_NO_MORE_DROP_MODES;
    }

    return S_OK;
}

HRESULT CVpuMft::SetQualityLevel(
    /* [in] */ MF_QUALITY_LEVEL eQualityLevel)
{
    return MF_E_NO_MORE_QUALITY_LEVELS;
}

HRESULT CVpuMft::GetDropMode(
    /* [annotation][out] */
    _Out_  MF_QUALITY_DROP_MODE *peDropMode)
{
    if (this->m_bShutdown)
    {
        return MF_E_SHUTDOWN;
    }

    CAutoLock lock(&m_csLock);

    *peDropMode = m_dropMode;

    TraceString(CHMFTTracing::TRACE_INFORMATION, L"%S(): value=%d", __FUNCTION__, (int)m_dropMode);

    return S_OK;
}


HRESULT CVpuMft::GetQualityLevel(
    /* [annotation][out] */
    _Out_  MF_QUALITY_LEVEL *peQualityLevel)
{
    return E_NOTIMPL;
}


HRESULT CVpuMft::DropTime(
    /* [in] */ LONGLONG hnsAmountToDrop)
{
    return MF_E_DROPTIME_NOT_SUPPORTED;
}
