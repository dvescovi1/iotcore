/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License.

Module Name:

    imxvpu_driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#ifndef __IMXVPU_DRIVER_H
#define __IMXVPU_DRIVER_H

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>
#include <dwl_defs.h>

#include "device.h"
#include "queue.h"
#include "trace.h"

//
// Macros to be used for proper PAGED/NON-PAGED code placement
//

#define IMXVPU_NONPAGED_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    //__pragma(code_seg(.text))

#define IMXVPU_NONPAGED_SEGMENT_END \
    __pragma(code_seg(pop))

#define IMXVPU_PAGED_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    __pragma(code_seg("PAGE"))

#define IMXVPU_PAGED_SEGMENT_END \
    __pragma(code_seg(pop))

#define IMXVPU_INIT_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    __pragma(code_seg("INIT"))

#define IMXVPU_INIT_SEGMENT_END \
    __pragma(code_seg(pop))

#define IMXVPU_ASSERT_MAX_IRQL(Irql) NT_ASSERT(KeGetCurrentIrql() <= (Irql))

#define IMXVPU_CORE_COUNT 2

#define DWL_CLIENT_TYPE_H264_DEC 1U
#define DWL_CLIENT_TYPE_MPEG4_DEC 2U
#define DWL_CLIENT_TYPE_JPEG_DEC 3U
#define DWL_CLIENT_TYPE_PP 4U
#define DWL_CLIENT_TYPE_VC1_DEC 5U
#define DWL_CLIENT_TYPE_MPEG2_DEC 6U
#define DWL_CLIENT_TYPE_VP6_DEC 7U
#define DWL_CLIENT_TYPE_AVS_DEC 8U
#define DWL_CLIENT_TYPE_RV_DEC 9U
#define DWL_CLIENT_TYPE_VP8_DEC 10U
#define DWL_CLIENT_TYPE_VP9_DEC 11U
#define DWL_CLIENT_TYPE_HEVC_DEC 12U

//
// VPU KM pool allocation tags
//
enum VPU_KM_ALLOC_TAG {

//  VPU_KM_ALLOC_TAG_TEMP = '0VPU', // Temporary be freed in the same routine
    VPU_KM_ALLOC_TAG_WDF = '@VPU'  // Allocations WDF makes on our behalf

}; // enum VPU_KM_ALLOC_TAG

//
// WDFDRIVER Events
//

EXTERN_C_START

DRIVER_INITIALIZE DriverEntry;

EVT_WDF_DEVICE_PREPARE_HARDWARE OnPrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE OnReleaseHardware;
EVT_WDF_DRIVER_DEVICE_ADD OnDeviceAdd;
EVT_WDF_DRIVER_UNLOAD OnDriverUnload;

EVT_WDF_OBJECT_CONTEXT_CLEANUP OnDriverContextCleanup;

EVT_WDF_IO_IN_CALLER_CONTEXT OnAnyIoInCallerContext;

EVT_WDF_DEVICE_FILE_CREATE OnFileCreate;
EVT_WDF_FILE_CLOSE OnFileClose;

EXTERN_C_END

//
// Helper functions
//
void FreeVpuBuffer(VpuAlloc *buffer);

NTSTATUS UnlockVpu(
    _In_ IMXVPU_DEVICE_CONTEXT *deviceContextPtr, _In_ ULONG CoreId, _In_ WDFFILEOBJECT File);

#endif