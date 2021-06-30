/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License.

Module Name:

    driver.c

Abstract:

    This file contains the driver entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "imxvpu_driver.h"
#include "driver.tmh"

IMXVPU_INIT_SEGMENT_BEGIN

_Use_decl_annotations_
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:
    DriverEntry initializes the driver and is the first routine called by the
    system after the driver is loaded. DriverEntry specifies the other entry
    points in the function driver, such as EvtDevice and DriverUnload.

Parameters Description:

    DriverObject - represents the instance of the function driver that is loaded
    into memory. DriverEntry must initialize members of DriverObject before it
    returns to the caller. DriverObject is allocated by the system before the
    driver is loaded, and it is released by the system after the system unloads
    the function driver from memory.

    RegistryPath - represents the driver specific path in the Registry.
    The function driver can use the path to store driver related data between
    reboots. The path does not store hardware instance specific data.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.

--*/
{
    WDF_DRIVER_CONFIG config;
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;

    //
    // Initialize WPP Tracing
    //
    WPP_INIT_TRACING(DriverObject, RegistryPath);

    TraceEvents(TRACE_LEVEL_INFORMATION, IMXVPU_DRIVER_TRACE, "%!FUNC! iMX VPU KM Entry");

    //
    // Register a cleanup callback so that we can call WPP_CLEANUP when
    // the framework driver object is deleted during driver unload.
    //
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.EvtCleanupCallback = OnDriverContextCleanup;

    WDF_DRIVER_CONFIG_INIT(&config,
                           OnDeviceAdd
                           );

    //
    // Specify a pool tag for allocations WDF makes on our behalf
    //
    config.DriverPoolTag = (ULONG)VPU_KM_ALLOC_TAG_WDF;

    status = WdfDriverCreate(DriverObject,
                             RegistryPath,
                             &attributes,
                             &config,
                             WDF_NO_HANDLE
                             );

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, IMXVPU_DRIVER_TRACE, "WdfDriverCreate failed %!STATUS!", status);
        WPP_CLEANUP(DriverObject);
        return status;
    }

    TraceEvents(TRACE_LEVEL_INFORMATION, IMXVPU_DRIVER_TRACE, "%!FUNC! Exit");

    return status;
}

IMXVPU_INIT_SEGMENT_END

IMXVPU_PAGED_SEGMENT_BEGIN

//
// Routine Description:
//
//  Called when any I/O request is sent to the driver.  Called in the process
//  context of the caller that initiated the I/O request. 
//  Everything is passed on to the default queue currently.
//
// Arguments:
//
//     FxDevice - a handle to the framework device object
//     FxRequest - a handle to the framework request object
//
//  Return Value:
//
//     None.
//
_Use_decl_annotations_
VOID
OnAnyIoInCallerContext(
    WDFDEVICE   FxDevice,
    WDFREQUEST  FxRequest
)
{
    NTSTATUS status;
    WDF_REQUEST_PARAMETERS params;

    PAGED_CODE();

    //
    // This routine only handles Device I/O Control requests.
    //
    WDF_REQUEST_PARAMETERS_INIT(&params);
    WdfRequestGetParameters(FxRequest, &params);
    if (params.Type == WdfRequestTypeDeviceControl)
    {
        //
        // Determine what type of Device I/O Control request this is.
        //
        switch (params.Parameters.DeviceIoControl.IoControlCode)
        {
        default:
            break;
        }
    }

    status = WdfDeviceEnqueueRequest(FxDevice, FxRequest);
    if (!NT_SUCCESS(status))
    {
        TraceEvents(
            TRACE_LEVEL_ERROR,
            IMXVPU_QUEUE_TRACE,
            "WdfDeviceEnqueueRequest(...) failed. (status=%!STATUS!, FxDevice=%p, FxRequest=%p)",
            status,
            FxDevice,
            FxRequest);

        WdfRequestComplete(FxRequest, STATUS_INTERNAL_ERROR);
        return;
    }
}

_Use_decl_annotations_
VOID
OnFileCreate(
    WDFDEVICE FxDevice,
    WDFREQUEST FxRequest,
    WDFFILEOBJECT FxFile
)
{
    UNREFERENCED_PARAMETER(FxDevice);
    UNREFERENCED_PARAMETER(FxFile);

    PAGED_CODE();
    IMXVPU_ASSERT_MAX_IRQL(PASSIVE_LEVEL);

    WdfRequestComplete(FxRequest, STATUS_SUCCESS);
}

_Use_decl_annotations_
VOID
OnFileClose(
    WDFFILEOBJECT FxFile
)
{
    WDFDEVICE device;
    IMXVPU_DEVICE_CONTEXT *deviceContextPtr;
    VpuAlloc *ptr;
    VpuAlloc *prev;

    PAGED_CODE();
    IMXVPU_ASSERT_MAX_IRQL(PASSIVE_LEVEL);

    device = WdfFileObjectGetDevice(FxFile);
    deviceContextPtr = DeviceGetContext(device);

    // Release any VPU locks and release any memory
    ExAcquireFastMutex(&deviceContextPtr->Mutex);

    UnlockVpu(deviceContextPtr, 0, FxFile);
    UnlockVpu(deviceContextPtr, 1, FxFile);

    prev = &deviceContextPtr->memList;
    ptr = prev->next;
    while (ptr != NULL)
    {
        if (ptr->file == FxFile)
        {
            prev->next = ptr->next;
            FreeVpuBuffer(ptr);
            ptr = prev->next;
        }
        else
        {
            prev = ptr;
            ptr = ptr->next;
        }
    }
    ExReleaseFastMutex(&deviceContextPtr->Mutex);
}

NTSTATUS
OnDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++
Routine Description:

    EvtDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager. We create and initialize a device object to
    represent a new instance of the device.

Arguments:

    Driver - Handle to a framework driver object created in DriverEntry

    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.

Return Value:

    NTSTATUS

--*/
{
    WDFDEVICE device;
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    WDF_OBJECT_ATTRIBUTES fileAttributes;
    IMXVPU_DEVICE_CONTEXT* deviceContext;
    WDF_FILEOBJECT_CONFIG fileConfig;
    NTSTATUS status;

    UNREFERENCED_PARAMETER(Driver);

    PAGED_CODE();

    TraceEvents(TRACE_LEVEL_INFORMATION, IMXVPU_DRIVER_TRACE, "%!FUNC! Entry");

    //
    // Setup PNP/Power callbacks.
    //
    {
        WDF_PNPPOWER_EVENT_CALLBACKS pnpCallbacks;
        WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpCallbacks);

        pnpCallbacks.EvtDevicePrepareHardware =
            OnPrepareHardware;
        pnpCallbacks.EvtDeviceReleaseHardware =
            OnReleaseHardware;

        WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpCallbacks);
    } // // Setup PNP/Power callbacks
    
    // Register the callback that gets all I/O requests (in caller context)
    // before they are queued.
    WdfDeviceInitSetIoInCallerContextCallback(DeviceInit, OnAnyIoInCallerContext);

    //
    // Initialize file object handling
    //
    WDF_OBJECT_ATTRIBUTES_INIT(&fileAttributes);
//  WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&fileAttributes, FILE_CONTEXT);
    fileAttributes.ExecutionLevel = WdfExecutionLevelPassive;

    WDF_FILEOBJECT_CONFIG_INIT(
        &fileConfig,
        OnFileCreate,
        OnFileClose,
        WDF_NO_EVENT_CALLBACK);     // OnFileCleanup

    WdfDeviceInitSetFileObjectConfig(DeviceInit, &fileConfig, &fileAttributes);

    WdfDeviceInitSetCharacteristics(
        DeviceInit,
        FILE_AUTOGENERATED_DEVICE_NAME,
        TRUE);                              // OR with existing values

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, IMXVPU_DEVICE_CONTEXT);

    // Grant world+dog access
    status = WdfDeviceInitAssignSDDLString(
        DeviceInit,
        &SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RW_RES_R);
    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, IMXVPU_DRIVER_TRACE, 
            "WdfDeviceInitAssignSDDLString failed. (status=%!STATUS!, DeviceInit=%p)",
            status,
            DeviceInit);

        goto end;
    }

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, IMXVPU_DRIVER_TRACE,
            "WdfDeviceCreate failed. (status=%!STATUS!, DeviceInit=%p)",
            status,
            DeviceInit);

        goto end;
    }

    //
    // Get a pointer to the device context structure that we just associated
    // with the device object. We define this structure in the device.h
    // header file. DeviceGetContext is an inline function generated by
    // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
    // This function will do the type checking and return the device context.
    // If you pass a wrong object handle it will return NULL and assert if
    // run under framework verifier mode.
    //
    deviceContext = DeviceGetContext(device);

    //
    // Initialize the context.
    //
    memset(deviceContext, 0, sizeof(IMXVPU_DEVICE_CONTEXT));
    deviceContext->WdfDevice = device;
    ExInitializeFastMutex(&deviceContext->Mutex);

    WDF_OBJECT_ATTRIBUTES attributes;
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.ParentObject = device;
    status = WdfWaitLockCreate(
        &attributes,
        &deviceContext->ControllerLock
    );
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, IMXVPU_DRIVER_TRACE,
            "WdfWaitLockCreate failed. (status=%!STATUS!)",
            status);

        goto end;
    }

    WDF_IO_QUEUE_CONFIG queueConfig;
    WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);
    status = WdfIoQueueCreate(device, &queueConfig, &attributes, &deviceContext->waitQueue[0]);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, IMXVPU_DRIVER_TRACE,
            "WdfIoQueueCreate[0] failed. (status=%!STATUS!)",
            status);

        goto end;
    }
    status = WdfIoQueueCreate(device, &queueConfig, &attributes, &deviceContext->waitQueue[1]);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, IMXVPU_DRIVER_TRACE,
            "WdfIoQueueCreate[1] failed. (status=%!STATUS!)",
            status);

        goto end;
    }

    //
    // Create a device interface so that applications can find and talk
    // to us.
    //
    status = WdfDeviceCreateDeviceInterface(
        device,
        &GUID_DEVINTERFACE_imxvpukm,
        NULL // ReferenceString
    );

    if (NT_SUCCESS(status)) {
        //
        // Initialize the I/O Package and any Queues
        //
        status = OnQueueInitialize(device);
    }

end:

    TraceEvents(TRACE_LEVEL_INFORMATION, IMXVPU_DRIVER_TRACE, "%!FUNC! Exit");

    return status;
}

VOID
OnDriverContextCleanup(
    _In_ WDFOBJECT DriverObject
    )
/*++
Routine Description:

    Free all the resources allocated in DriverEntry.

Arguments:

    DriverObject - handle to a WDF Driver object.

Return Value:

    VOID.

--*/
{
    UNREFERENCED_PARAMETER(DriverObject);

    PAGED_CODE();

    TraceEvents(TRACE_LEVEL_INFORMATION, IMXVPU_DRIVER_TRACE, "%!FUNC! Entry");

    //
    // Stop WPP Tracing
    //
    WPP_CLEANUP(WdfDriverWdmGetDriverObject((WDFDRIVER)DriverObject));
}

IMXVPU_PAGED_SEGMENT_END