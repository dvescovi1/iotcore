/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#pragma once

#include <windows.h>
#include <Evntprov.h>

namespace CHMFTTracing
{
    enum eTraceLevel
    {
        TRACE_MEMORY        = 8,
        TRACE_INFORMATION   = 4,
        TRACE_ERROR         = 2
    };
}

struct _traceContext
{
    const GUID * traceGuid;
    REGHANDLE etwHandle;
    bool enabled;
    UCHAR level;
};

extern struct _traceContext g_traceContext;

// Provider ID: {2183964F-3782-48A6-9E93-52580016156E}
static const GUID guidMftTrace =
{ 0x2183964f, 0x3782, 0x48a6, { 0x9e, 0x93, 0x52, 0x58, 0x0, 0x16, 0x15, 0x6e } };

// Provider ID: {73D6D9DF-DD8F-474A-9BBF-692F4E5BECD6}
static const GUID guidVpuHantroTrace =
{ 0x73d6d9df, 0xdd8f, 0x474a, { 0x9b, 0xbf, 0x69, 0x2f, 0x4e, 0x5b, 0xec, 0xd6 } };

// {5AD3FEA7-0D7B-4397-8F66-BB63C04A4714}
static const GUID guidVpuDwlTrace =
{ 0x5ad3fea7, 0xd7b, 0x4397, { 0x8f, 0x66, 0xbb, 0x63, 0xc0, 0x4a, 0x47, 0x14 } };

EXTERN_C void TraceInitialize(
    struct _traceContext * context);

EXTERN_C void TraceUninitialize(
    struct _traceContext * context);

EXTERN_C void TraceStringA(
    struct _traceContext * context,
    const UCHAR ucLevel,
    const CHAR* pwszFormat,
    ...
);

EXTERN_C void TraceStringW(
    struct _traceContext * context,
    const UCHAR ucLevel,
    const WCHAR* pwszFormat, 
    ...
    );

#define TraceString(level, format, ...) TraceStringW(&g_traceContext, level, format, __VA_ARGS__)
