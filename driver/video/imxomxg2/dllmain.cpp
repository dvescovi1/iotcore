/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include <Windows.h>
#include <tracing.h>

struct _traceContext g_traceContext = { &guidVpuHantroTrace, 0, 0, 0 };

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        TraceInitialize(&g_traceContext);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        TraceUninitialize(&g_traceContext);
        break;
    }
    return TRUE;
}
