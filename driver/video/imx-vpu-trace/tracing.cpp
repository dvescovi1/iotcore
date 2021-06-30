/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "tracing.h"
#include <evntrace.h>
#include <strsafe.h>

extern struct _traceContext g_traceContext;

static void NTAPI EventRegisterCallback(
    LPCGUID                     lpcguidSourceId,
    ULONG                       ulIsEnabled,
    UCHAR                       ucLevel,
    ULONGLONG                   ullMatchAnyKeyword,
    ULONGLONG                   ullMatchAllKeywords,
    PEVENT_FILTER_DESCRIPTOR    pefdFilterData,
    PVOID                       pvCallbackContext)
{
    struct _traceContext * context = (struct _traceContext *)pvCallbackContext;
    switch(ulIsEnabled)
    {
    case EVENT_CONTROL_CODE_ENABLE_PROVIDER:  
        context->enabled  = TRUE;
        context->level   = ucLevel;
        break;
    case EVENT_CONTROL_CODE_DISABLE_PROVIDER:  
        context->enabled  = FALSE;
        context->level   = 0;
        break;
    default:
        // Nothing to do
        break;
    };
}

EXTERN_C void TraceInitialize(struct _traceContext * context)
{
    if(context->enabled == FALSE)
    {
        EventRegister(
            context->traceGuid,
            &EventRegisterCallback,
            context,
            &context->etwHandle
            );
    }
}

EXTERN_C void TraceUninitialize(struct _traceContext * context)
{
    if(context->etwHandle != NULL)
    {
        EventUnregister(context->etwHandle);
    }

    context->etwHandle = NULL;
    context->enabled  = FALSE;
    context->level   = 0;
}

EXTERN_C void TraceStringW(
    struct _traceContext * context,
    const UCHAR ucLevel,
    const WCHAR* pwszFormat, 
    ...)
{
    do
    {
        if(context->enabled == FALSE)
        {
            // Do not trace
            break;
        }

        if( (ucLevel > context->level)   &&
            (context->level != 0)        )
        {
            // Do not trace
            break;
        }

        WCHAR   pwszTrace[1024] = {0};

        va_list args;
        va_start(args, pwszFormat);

        StringCchVPrintfW(
            pwszTrace,
            ARRAYSIZE(pwszTrace),
            pwszFormat,
            args
            );

        va_end(args);

        EventWriteString(context->etwHandle, ucLevel, 0, pwszTrace);
    }while(false);
}

EXTERN_C void TraceStringA(
    struct _traceContext * context,
    const UCHAR ucLevel,
    const CHAR* pszFormat,
    ...)
{
    do
    {
        if (context->enabled == FALSE)
        {
            // Do not trace
            break;
        }

        if ((ucLevel > context->level) &&
            (context->level != 0))
        {
            // Do not trace
            break;
        }

        CHAR   pszTrace[1024] = { 0 };
        WCHAR   pwszTrace[1024] = { 0 };

        va_list args;
        va_start(args, pszFormat);

        StringCchVPrintfA(
            pszTrace,
            ARRAYSIZE(pszTrace),
            pszFormat,
            args
        );

        va_end(args);

        StringCchPrintfW(pwszTrace, ARRAYSIZE(pwszTrace), L"%S", pszTrace);

        EventWriteString(context->etwHandle, ucLevel, 0, pwszTrace);
    } while (false);
}


EXTERN_C
void VpuDebugPrint(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    CHAR   pszTrace[1024] = { 0 };
    WCHAR   pwszTrace[1024] = { 0 };

    // TODO: check trace status so we don't waste time here if not tracing.
    StringCchVPrintfA(
        pszTrace,
        ARRAYSIZE(pszTrace),
        fmt,
        args
    );

    va_end(args);

    TraceStringW(&g_traceContext, CHMFTTracing::TRACE_INFORMATION, L"VPUHantro: %S", pszTrace);
}

EXTERN_C
void VpuErrorPrint(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    CHAR   pszTrace[1024] = { 0 };
    WCHAR   pwszTrace[1024] = { 0 };

    // TODO: check trace status so we don't waste time here if not tracing.
    StringCchVPrintfA(
        pszTrace,
        ARRAYSIZE(pszTrace),
        fmt,
        args
    );

    va_end(args);

    TraceStringW(&g_traceContext, CHMFTTracing::TRACE_ERROR, L"VPUHantro: %S", pszTrace);
}
