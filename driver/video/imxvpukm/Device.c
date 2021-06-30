/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License.

Module Name:

    device.c - Device handling events for example driver.

Abstract:

   This file contains the device entry points and callbacks.
    
Environment:

    Kernel-mode Driver Framework

*/

#include "imxvpu_driver.h"
#include "Device.tmh"

IMXVPU_PAGED_SEGMENT_BEGIN; //==================================================

_Use_decl_annotations_
NTSTATUS
OnPrepareHardware(
    WDFDEVICE WdfDevice,
    WDFCMRESLIST ResourcesRaw,
    WDFCMRESLIST ResourcesTranslated
)
{
    PAGED_CODE();
    IMXVPU_ASSERT_MAX_IRQL(PASSIVE_LEVEL);
    UNREFERENCED_PARAMETER(ResourcesRaw);
    const CM_PARTIAL_RESOURCE_DESCRIPTOR* memResourcePtr = NULL;
    ULONG interruptResourceCount = 0;

    //
    // Look for single memory and interrupt resource.
    //
    const ULONG resourceCount = WdfCmResourceListGetCount(ResourcesTranslated);
    for (ULONG i = 0; i < resourceCount; ++i) {
        const CM_PARTIAL_RESOURCE_DESCRIPTOR* resourcePtr =
            WdfCmResourceListGetDescriptor(ResourcesTranslated, i);

        switch (resourcePtr->Type) {
        case CmResourceTypeMemory:
            if (memResourcePtr != NULL) {
                IMXVPU_LOG_ERROR(
                    "Received unexpected memory resource. (resourcePtr = 0x%p)",
                    resourcePtr);

                return STATUS_DEVICE_CONFIGURATION_ERROR;
            }

            memResourcePtr = resourcePtr;
            break;

        case CmResourceTypeInterrupt:
            if (interruptResourceCount > 3) {
                IMXVPU_LOG_ERROR(
                    "Received unexpected interrupt resource. "
                    "(interruptResourceCount = %lu, resourcePtr = 0x%p)",
                    interruptResourceCount,
                    resourcePtr);

                return STATUS_DEVICE_CONFIGURATION_ERROR;
            }

            ++interruptResourceCount;
            break;
        }
    }

    if ((memResourcePtr == NULL) || (interruptResourceCount != 3)) {
        IMXVPU_LOG_ERROR(
            "Did not receive required memory resource and interrupt resource. "
            "(memResourcePtr = 0x%p, interruptResourceCount = %lu)",
            memResourcePtr,
            interruptResourceCount);

        return STATUS_DEVICE_CONFIGURATION_ERROR;
    }

    if (memResourcePtr->u.Memory.Length < sizeof(IMXVPU_REGISTERS)) {
        IMXVPU_LOG_ERROR(
            "Memory resource is too small. "
            "(memResourcePtr->u.Memory.Length = %lu, "
            "sizeof(IMXVPU_REGISTERS) = %lu)",
            memResourcePtr->u.Memory.Length,
            sizeof(IMXVPU_REGISTERS));

        return STATUS_DEVICE_CONFIGURATION_ERROR;
    }

    //
    // ReleaseHardware is ALWAYS called, even if PrepareHardware fails, so
    // the cleanup of registersPtr is handled there
    //

    IMXVPU_DEVICE_CONTEXT* deviceContextPtr =
        DeviceGetContext(WdfDevice);

    // pad vpuMemory length to ULONG boundary
    NT_ASSERT(memResourcePtr->Type == CmResourceTypeMemory);
    ULONG vpuLength = ((memResourcePtr->u.Memory.Length + sizeof(ULONG) - 1) / sizeof(ULONG)) * sizeof(ULONG);

    deviceContextPtr->RegistersPtr = (IMXVPU_REGISTERS*)
        MmMapIoSpaceEx(
            memResourcePtr->u.Memory.Start,
            vpuLength,
            PAGE_READWRITE | PAGE_NOCACHE);

    if (deviceContextPtr->RegistersPtr == NULL) {
        IMXVPU_LOG_LOW_MEMORY(
            "MmMapIoSpaceEx(...) failed. "
            "(memResourcePtr->u.Memory.Start = 0x%llx, "
            "length = %lu)",
            memResourcePtr->u.Memory.Start.QuadPart,
            vpuLength);

        return STATUS_INSUFFICIENT_RESOURCES;
    }
    deviceContextPtr->RegistersLen = vpuLength;

    deviceContextPtr->MdlRegisters = IoAllocateMdl(deviceContextPtr->RegistersPtr, vpuLength, FALSE, FALSE, NULL);
    if (deviceContextPtr->MdlRegisters == NULL) {
        IMXVPU_LOG_LOW_MEMORY(
            "IoAllocateMdl(...) failed. "
            "(RegistersPtr = 0x%p)",
            deviceContextPtr->RegistersPtr);

        return STATUS_INSUFFICIENT_RESOURCES;
    }
    MmBuildMdlForNonPagedPool(deviceContextPtr->MdlRegisters);
       
    return STATUS_SUCCESS;
}

_Use_decl_annotations_
NTSTATUS
OnReleaseHardware(
    WDFDEVICE WdfDevice,
    WDFCMRESLIST ResourcesTranslated
)
{
    PAGED_CODE();
    IMXVPU_ASSERT_MAX_IRQL(PASSIVE_LEVEL);

    UNREFERENCED_PARAMETER(ResourcesTranslated);
    IMXVPU_DEVICE_CONTEXT* deviceContextPtr =
        DeviceGetContext(WdfDevice);

    if (deviceContextPtr->MdlRegisters != NULL) {
        IoFreeMdl(deviceContextPtr->MdlRegisters);
        deviceContextPtr->MdlRegisters = NULL;
    }

    if (deviceContextPtr->RegistersPtr != NULL) {
        MmUnmapIoSpace(
            deviceContextPtr->RegistersPtr,
            deviceContextPtr->RegistersLen);

        deviceContextPtr->RegistersLen = 0;
        deviceContextPtr->RegistersPtr = NULL;
    }

    return STATUS_SUCCESS;
}


NTSTATUS
OnCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    UNREFERENCED_PARAMETER(DeviceInit);

    return STATUS_SUCCESS;
}
