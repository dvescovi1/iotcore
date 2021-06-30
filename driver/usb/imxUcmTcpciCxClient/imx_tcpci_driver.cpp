/*
* This file is derived from Microsoft example available on https://github.com/microsoft/Windows-driver-samples/blob/master/usb/UcmTcpciCxClientSample
* Original file licensing:
* Copyright (c) 2015 Microsoft
* SPDX-License-Identifier: MS-PL
* NXP modifications are licensed under the same license
* Copyright 2020 NXP
*
*/

#include "imx_tcpci_driver.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, EvtDeviceAdd)
#pragma alloc_text (PAGE, EvtDriverContextCleanup)
#endif

/*++
Routine Description:
    DriverEntry initializes the driver and is the first routine called by the system after the driver is loaded. DriverEntry specifies the other entry
    points in the function driver, such as EvtDevice and DriverUnload.
Parameters Description:
    DriverObject - represents the instance of the function driver that is loaded into memory. DriverEntry must initialize members of DriverObject before it
                   returns to the caller. DriverObject is allocated by the system before the driver is loaded, and it is released by the system after the system unloads
                   the function driver from memory.
    RegistryPath - represents the driver specific path in the Registry. The function driver can use the path to store driver related data between
                   reboots. The path does not store hardware instance specific data.
Return Value:
    NTSTATUS
--*/
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  pDriverObject, _In_ PUNICODE_STRING pRegistryPath) {
    NTSTATUS               ntStatus;
    WDF_DRIVER_CONFIG      Config;
    WDF_OBJECT_ATTRIBUTES  Attributes;

    // Register a cleanup callback so that we can call WPP_CLEANUP when the framework driver object is deleted during driver unload.
    WDF_OBJECT_ATTRIBUTES_INIT(&Attributes);
    Attributes.EvtCleanupCallback = EvtDriverContextCleanup;
    WDF_DRIVER_CONFIG_INIT(&Config, EvtDeviceAdd);

    DBG_PRINT_INFO("Driver: 0x%016p, '%S')", pDriverObject, ((PUNICODE_STRING)pRegistryPath)->Buffer);
    DBG_PRINT_INFO("***********************************************************************************");
    DBG_PRINT_INFO("*** NXP USB TCPC driver, date: %s %s                             ***", __DATE__, __TIME__);
    DBG_PRINT_INFO("***********************************************************************************");
    if (!NT_SUCCESS(ntStatus = WdfDriverCreate(pDriverObject, pRegistryPath, &Attributes, &Config, WDF_NO_HANDLE))) {
        DBG_PRINT_ERROR("WdfDriverCreate() failed. ntStatus: 0x%08X", ntStatus);
    }
    return ntStatus;
}

/*++
Routine Description:
    EvtDeviceAdd is called by the framework in response to AddDevice call from the PnP manager. We create and initialize a device object to represent a new instance of the device.
Arguments:
    Driver     - Handle to a framework driver object created in DriverEntry
    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.
Return Value:
    NTSTATUS
--*/
NTSTATUS EvtDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit) {
    UNREFERENCED_PARAMETER(Driver);
    PAGED_CODE();
    NTSTATUS ntStatus;

    ntStatus = EvtCreateDevice(DeviceInit);
    return ntStatus;
}

/*++
Routine Description:
    Free all the resources allocated in DriverEntry.
Arguments:
    DriverObject - handle to a WDF Driver object.
--*/
VOID EvtDriverContextCleanup(_In_ WDFOBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    _IRQL_limited_to_(PASSIVE_LEVEL);
    PAGED_CODE();
}
