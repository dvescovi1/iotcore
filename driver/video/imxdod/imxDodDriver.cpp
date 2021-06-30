// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "precomp.h"

#include "imxDodLogging.h"
#include "imxDodDriver.tmh"

#include "imxDodCommon.h"
#include "imxDodDevice.h"
#include "imxDodDriver.h"

IMXDOD_NONPAGED_SEGMENT_BEGIN; //==============================================

//
// Placement new and delete operators
//

_Use_decl_annotations_
void* operator new ( size_t, void* Ptr ) throw ()
{
    return Ptr;
}

void operator delete ( void*, void* ) throw ()
{}

_Use_decl_annotations_
void* operator new[] ( size_t, void* Ptr ) throw ()
{
    return Ptr;
}

void operator delete[] ( void*, void* ) throw ()
{}

IMXDOD_NONPAGED_SEGMENT_END; //================================================
IMXDOD_PAGED_SEGMENT_BEGIN; //=================================================

namespace { // static

    // This must be stored as a global variable since DxgkDdiUnload does not
    // supply a driver object pointer or context pointer.
    DRIVER_OBJECT* mx6GlobalDriverObjectPtr;

} // namespace "static"

_Use_decl_annotations_
void IMXDodDdiUnload ()
{
    PAGED_CODE();
    IMXDOD_ASSERT_MAX_IRQL(PASSIVE_LEVEL);

    NT_ASSERT(mx6GlobalDriverObjectPtr);
    WPP_CLEANUP(mx6GlobalDriverObjectPtr);
    mx6GlobalDriverObjectPtr = nullptr;
}

IMXDOD_PAGED_SEGMENT_END; //=====================================================
IMXDOD_INIT_SEGMENT_BEGIN; //====================================================

_Use_decl_annotations_
NTSTATUS
DriverEntry (
    DRIVER_OBJECT* DriverObjectPtr,
    UNICODE_STRING* RegistryPathPtr
    )
{
    PAGED_CODE();
     
    //
    // Initialize logging
    //
    {
        WPP_INIT_TRACING(DriverObjectPtr, RegistryPathPtr);
        RECORDER_CONFIGURE_PARAMS recorderConfigureParams;
        RECORDER_CONFIGURE_PARAMS_INIT(&recorderConfigureParams);
        WppRecorderConfigure(&recorderConfigureParams);
#if DBG
        WPP_RECORDER_LEVEL_FILTER(IMXDOD_TRACING_DEFAULT) = TRUE;
#endif // DBG
    }

    IMXDOD_LOG_INFORMATION(
        "(DriverObjectPtr = 0x%p, RegistryPathPtr = 0x%p)",
        DriverObjectPtr,
        RegistryPathPtr);
    
    // Initialize DDI function pointers and dxgkrnl
    auto dodInit = KMDDOD_INITIALIZATION_DATA();
    dodInit.Version = DXGKDDI_INTERFACE_VERSION;

    dodInit.DxgkDdiUnload = IMXDodDdiUnload;

    dodInit.DxgkDdiAddDevice = IMXDOD_DEVICE::DdiAddDevice;
    dodInit.DxgkDdiStartDevice = IMXDOD_DEVICE::DdiStartDevice;
    dodInit.DxgkDdiStopDevice = IMXDOD_DEVICE::DdiStopDevice;
    dodInit.DxgkDdiResetDevice = IMXDOD_DEVICE::DdiResetDevice;
    dodInit.DxgkDdiRemoveDevice = IMXDOD_DEVICE::DdiRemoveDevice;
    dodInit.DxgkDdiDispatchIoRequest = IMXDOD_DEVICE::DdiDispatchIoRequest;

    dodInit.DxgkDdiQueryChildRelations = IMXDOD_DEVICE::DdiQueryChildRelations;
    dodInit.DxgkDdiQueryChildStatus = IMXDOD_DEVICE::DdiQueryChildStatus;
    dodInit.DxgkDdiQueryDeviceDescriptor = IMXDOD_DEVICE::DdiQueryDeviceDescriptor;
    dodInit.DxgkDdiSetPowerState = IMXDOD_DEVICE::DdiSetPowerState;

    dodInit.DxgkDdiQueryAdapterInfo = IMXDOD_DEVICE::DdiQueryAdapterInfo;
    dodInit.DxgkDdiSetPointerPosition = IMXDOD_DEVICE::DdiSetPointerPosition;
    dodInit.DxgkDdiSetPointerShape = IMXDOD_DEVICE::DdiSetPointerShape;

    dodInit.DxgkDdiIsSupportedVidPn = IMXDOD_DEVICE::DdiIsSupportedVidPn;
    dodInit.DxgkDdiRecommendFunctionalVidPn = IMXDOD_DEVICE::DdiRecommendFunctionalVidPn;
    dodInit.DxgkDdiEnumVidPnCofuncModality = IMXDOD_DEVICE::DdiEnumVidPnCofuncModality;
    dodInit.DxgkDdiSetVidPnSourceVisibility = IMXDOD_DEVICE::DdiSetVidPnSourceVisibility;
    dodInit.DxgkDdiCommitVidPn = IMXDOD_DEVICE::DdiCommitVidPn;
    dodInit.DxgkDdiUpdateActiveVidPnPresentPath = IMXDOD_DEVICE::DdiUpdateActiveVidPnPresentPath;

    dodInit.DxgkDdiRecommendMonitorModes = IMXDOD_DEVICE::DdiRecommendMonitorModes;
    dodInit.DxgkDdiQueryVidPnHWCapability = IMXDOD_DEVICE::DdiQueryVidPnHWCapability;
    dodInit.DxgkDdiPresentDisplayOnly = IMXDOD_DEVICE::DdiPresentDisplayOnly;
    dodInit.DxgkDdiSetPowerComponentFState = IMXDOD_DEVICE::DdiSetPowerComponentFState;
    dodInit.DxgkDdiStopDeviceAndReleasePostDisplayOwnership = IMXDOD_DEVICE::DdiStopDeviceAndReleasePostDisplayOwnership;
    dodInit.DxgkDdiSystemDisplayEnable = IMXDOD_DEVICE::DdiSystemDisplayEnable;
    dodInit.DxgkDdiSystemDisplayWrite = IMXDOD_DEVICE::DdiSystemDisplayWrite;
    dodInit.DxgkDdiPowerRuntimeControlRequest = IMXDOD_DEVICE::DdiPowerRuntimeControlRequest;

    NTSTATUS status = DxgkInitializeDisplayOnlyDriver(
            DriverObjectPtr,
            RegistryPathPtr,
            &dodInit);
            
    if (!NT_SUCCESS(status)) {
        IMXDOD_LOG_ERROR(
            "DxgkInitializeDisplayOnlyDriver failed. (status = %!STATUS!, DriverObjectPtr = %p)",
            status,
            DriverObjectPtr);
            
        return status;
    }

    NT_ASSERT(mx6GlobalDriverObjectPtr == nullptr);
    mx6GlobalDriverObjectPtr = DriverObjectPtr;
    NT_ASSERT(status == STATUS_SUCCESS);
    return status;
} // DriverEntry (...)

IMXDOD_INIT_SEGMENT_END; //====================================================

