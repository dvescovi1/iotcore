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
#pragma alloc_text (PAGE, EvtCreateDevice)
#pragma alloc_text (PAGE, EvtPrepareHardware)
#pragma alloc_text (PAGE, EvtDeviceD0Entry)
#pragma alloc_text (PAGE, EvtReleaseHardware)
#endif

#define RdRegMult(_Items) if (!NT_SUCCESS(ntStatus = I2C_ReadSynchronouslyMultiple(pDevContext, I2CRequestSourceClient, _Items, _countof(_Items)))) {break;}
#define RdReg(_Reg, _Buff) if (!NT_SUCCESS(ntStatus = I2C_ReadSynchronously(pDevContext, I2CRequestSourceClient, _Reg, &_Buff, sizeof(_Buff)))) {break;} 
#define WrReg(_Reg, _Buff) if (!NT_SUCCESS(ntStatus = I2C_WriteSynchronously(pDevContext, I2CRequestSourceClient, _Reg, &_Buff, sizeof(_Buff)))) {break;} 

/*++
Routine Description:
    Worker routine called to create a device and its software resources.
Arguments:
    DeviceInit - Pointer to an opaque init structure. Memory for this structure will be freed by the framework when the WdfDeviceCreate succeeds. Don't access the structure after that point.
Return Value:
    NTSTATUS
--*/
NTSTATUS EvtCreateDevice(_Inout_ PWDFDEVICE_INIT pDeviceInit) {
    PAGED_CODE();

    NTSTATUS                     ntStatus;
    WDFDEVICE                    hDevice;
    PDEVICE_CONTEXT              pDevContext;
    UCMTCPCI_DEVICE_CONFIG       Config;
    WDF_PNPPOWER_EVENT_CALLBACKS PnpPowerCallbacks;
    WDF_OBJECT_ATTRIBUTES        Attributes;
    
    do {
        WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&Attributes, DEVICE_CONTEXT);
        WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&PnpPowerCallbacks);
        PnpPowerCallbacks.EvtDevicePrepareHardware = EvtPrepareHardware;
        PnpPowerCallbacks.EvtDeviceReleaseHardware = EvtReleaseHardware;
        PnpPowerCallbacks.EvtDeviceD0Entry         = EvtDeviceD0Entry;
        WdfDeviceInitSetPnpPowerEventCallbacks(pDeviceInit, &PnpPowerCallbacks);
        if (!NT_SUCCESS(ntStatus = UcmTcpciDeviceInitInitialize(pDeviceInit))) {
            DBG_PRINT_ERROR("[PWDFDEVICE_INIT: 0x%p] UcmTcpciDeviceInitInitialize failed - ntStatus: 0x%08X", pDeviceInit, ntStatus);
            break;
        }
        if (!NT_SUCCESS(ntStatus = WdfDeviceCreate(&pDeviceInit, &Attributes, &hDevice))) {
            DBG_PRINT_ERROR("[PWDFDEVICE_INIT: 0x%p] WdfDeviceCreate failed - ntStatus: 0x%08X", pDeviceInit, ntStatus);
            break;
        }
        pDevContext = DeviceGetContext(hDevice);
        pDevContext->Device                  = hDevice;            // Save the device in the context so we can access it later.
        pDevContext->ResetAttempts           = 0;                  // Initialize platform-level device reset.
        RtlZeroMemory(&pDevContext->ResetInterface, sizeof(pDevContext->ResetInterface));
        pDevContext->ResetInterface.Size     = sizeof(pDevContext->ResetInterface);
        pDevContext->ResetInterface.Version  = 1;
        if (!NT_SUCCESS(ntStatus = WdfFdoQueryForInterface(pDevContext->Device, &GUID_DEVICE_RESET_INTERFACE_STANDARD, (PINTERFACE)&pDevContext->ResetInterface, sizeof(pDevContext->ResetInterface), 1, NULL))) {
            // The reset interface may not exist on certain environments.
            // In this case, we will fall back to using a different reset method.
            // Zero the reset interface and ignore the error.
            DBG_PRINT_ERROR("[WDFDEVICE: 0x%p] WdfFdoQueryForInterface for GUID_DEVICE_RESET_INTERFACE_STANDARD failed. ntStatus: 0x%08X", hDevice, ntStatus);
            RtlZeroMemory(&pDevContext->ResetInterface, sizeof(pDevContext->ResetInterface));
            ntStatus = STATUS_SUCCESS;
        } else {
            DBG_PRINT_ERROR("[WDFDEVICE: 0x%p] Successfully initialized platform-level device reset.", pDevContext->Device);
        }
        // Register our device with UcmTcpciCx.
        UCMTCPCI_DEVICE_CONFIG_INIT(&Config);
        if (!NT_SUCCESS(ntStatus = UcmTcpciDeviceInitialize(hDevice, &Config))) {
            DBG_PRINT_ERROR("[WDFDEVICE: 0x%p] UcmTcpciDeviceInitialize failed - ntStatus: 0x%08X", hDevice, ntStatus);
            break;
        }
    } while (0);
    return ntStatus;
}

/*++
Routine Description:
    A driver's EvtDevicePrepareHardware event callback function performs any operations that are needed to make a device accessible to the driver.
Arguments:
    hDevice              - A handle to a framework device object.
    ResourcesRaw        - A handle to a framework resource-list object that identifies the raw hardware resources that the Plug and Play manager has assigned to the device.
    ResourcesTranslated - A handle to a framework resource-list object that identifies the translated hardware resources that the Plug and Play manager has assigned to the device.
Return Value:
    NTSTATUS
--*/
NTSTATUS EvtPrepareHardware(_In_ WDFDEVICE hDevice, _In_ WDFCMRESLIST hResourcesRaw, _In_ WDFCMRESLIST hResourcesTranslated) {
    PAGED_CODE();

    NTSTATUS ntStatus;
    PDEVICE_CONTEXT pDevContext;

    pDevContext = DeviceGetContext(hDevice);
    do {
        if (!NT_SUCCESS(ntStatus = I2C_Initialize(pDevContext, hResourcesRaw, hResourcesTranslated))) {  // Initialize the I2C communication channel to read from/write to the hardware.
            break;
        }
        if (!NT_SUCCESS(ntStatus = I2C_Open(pDevContext))) {
            break;
        }
        if (!NT_SUCCESS(ntStatus = GPIO_Open(pDevContext))) {
            break;
        }
    } while (0);
    return ntStatus;
}

NTSTATUS EvtDeviceD0Entry(_In_ WDFDEVICE hDevice, _In_ WDF_POWER_DEVICE_STATE PreviousState) {
    NTSTATUS                                ntStatus;
    PDEVICE_CONTEXT                         pDevContext;
    UCMTCPCIPORTCONTROLLER                  hPortController = WDF_NO_HANDLE;
    UCMTCPCI_PORT_CONTROLLER_CONFIG         Config;
    UCMTCPCI_PORT_CONTROLLER_IDENTIFICATION Ident;
    UCMTCPCI_PORT_CONTROLLER_CAPABILITIES   Capabilities;
    TCPC_PHY_FAULT_STATUS_t                 FaultStatus;
    TCPC_PHY_ALERT_t                        Alert;
    TCPC_PHY_EXT_FAULT_CONFIG_t             ExtFaultCfg = {0x0F};

    UNREFERENCED_PARAMETER(PreviousState);
    PAGED_CODE();

    pDevContext = DeviceGetContext(hDevice);
    UCMTCPCI_PORT_CONTROLLER_IDENTIFICATION_INIT(&Ident);
    UCMTCPCI_PORT_CONTROLLER_CAPABILITIES_INIT(&Capabilities);

    // Read device identification and Capabilities from the registers.
    REGISTER_ITEM items[] = {
        GEN_REGISTER_ITEM(TCPC_PHY_VENDOR_ID,                    Ident.VendorId),
        GEN_REGISTER_ITEM(TCPC_PHY_PRODUCT_ID,                   Ident.ProductId),
        GEN_REGISTER_ITEM(TCPC_PHY_DEVICE_ID,                    Ident.DeviceId),
        GEN_REGISTER_ITEM(TCPC_PHY_USBTYPEC_REV,                 Ident.TypeCRevisionInBcd),
        GEN_REGISTER_ITEM(TCPC_PHY_USBPD_REV_VER,                Ident.PDRevisionAndVersionInBcd),
        GEN_REGISTER_ITEM(TCPC_PHY_PD_INTERFACE_REV,             Ident.PDInterfaceRevisionAndVersionInBcd),
        GEN_REGISTER_ITEM(TCPC_PHY_DEVICE_CAPABILITIES_1,        Capabilities.DeviceCapabilities1),
        GEN_REGISTER_ITEM(TCPC_PHY_DEVICE_CAPABILITIES_2,        Capabilities.DeviceCapabilities2),
        GEN_REGISTER_ITEM(TCPC_PHY_STANDARD_INPUT_CAPABILITIES,  Capabilities.StandardInputCapabilities),
        GEN_REGISTER_ITEM(TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES, Capabilities.StandardOutputCapabilities),
    };

    do {
        RdReg(TCPC_PHY_ALERT, Alert);
        if (Alert.B.FAULT) {
            RdReg(TCPC_PHY_FAULT_STATUS, FaultStatus);
            if (FaultStatus.B.ALL_REGISTERS_RESET_TO_DEFAULT) {
                /* After Power-on reset the FAULT_STATUS.ALL_REGISTERS_RESET_TO_DEFAULT is set */
                FaultStatus.R = TCPC_PHY_FAULT_STATUS_MASK_ALLREGISTERSRESETTODEFAULT_INT_MASK_MASK;
                WrReg(TCPC_PHY_FAULT_STATUS, FaultStatus);   /* Clear Register reset to defulut bit */
                Alert.R = TCPC_PHY_ALERT_FAULT_MASK;
                WrReg(TCPC_PHY_ALERT, Alert);                /* Clear fault bit */
            }
        }
        Alert.R = 0x0DFF;
        WrReg(TCPC_PHY_ALERT_MASK, Alert);
        TCPC_PHY_ReadAllRegs(pDevContext);
        RdRegMult(items);
        Capabilities.IsPowerDeliveryCapable = FALSE;
        UCMTCPCI_PORT_CONTROLLER_CONFIG_INIT(&Config, &Ident, &Capabilities);
        // Create a UCMTCPCIPORTCONTROLLER framework object.
        if (!NT_SUCCESS(ntStatus = UcmTcpciPortControllerCreate(hDevice, &Config, WDF_NO_OBJECT_ATTRIBUTES, &hPortController))) {
            DBG_PRINT_ERROR("[WDFDEVICE: 0x%p] UcmTcpciPortControllerCreate failed  ntStatus: 0x%.8X", hDevice, ntStatus);
            break;
        }
        pDevContext = DeviceGetContext(hDevice);                            // Save the UCMTCPCIPORTCONTROLLER in our device context.
        pDevContext->PortController = hPortController;    
        if (!NT_SUCCESS(ntStatus = HardwareRequestQueueInitialize(hDevice))) {  // Set the hardware request queue for our device.
            break;
        }
        // Direct UcmTcpciCx to start the port controller. At this point, UcmTcpciCx will assume control of USB Type-C and Power Delivery.
        // After the port controller is started, UcmTcpciCx may start putting requests into the hardware request queue.
        if (!NT_SUCCESS(ntStatus = UcmTcpciPortControllerStart(hPortController))) {
            DBG_PRINT_ERROR("[UCMTCPCIPORTCONTROLLER: 0x%p] UcmTcpciPortControllerStart failed  ntStatus: 0x%.8X", hPortController, ntStatus);
            break;
        }
    } while (0);
    if (!NT_SUCCESS(ntStatus) && (hPortController != WDF_NO_HANDLE)) {
        WdfObjectDelete(hPortController);
        pDevContext->PortController = WDF_NO_HANDLE;
    }
    return ntStatus;
}

/*++
Routine Description:
    A driver's EvtDeviceReleaseHardware event callback function performs operations that are needed when a device is no longer accessible.
Arguments:
    hDevice              - A handle to a framework device object.
    ResourcesTranslated - A handle to a resource list object that identifies the translated hardware resources that the Plug and Play manager has assigned to the device.
Return Value:
    NTSTATUS
--*/
NTSTATUS EvtReleaseHardware(_In_ WDFDEVICE hDevice, _In_ WDFCMRESLIST ResourcesTranslated) {
    UNREFERENCED_PARAMETER(ResourcesTranslated);
    PAGED_CODE();

    NTSTATUS        ntStatus = STATUS_SUCCESS;
    PDEVICE_CONTEXT pDevContext;

    pDevContext = DeviceGetContext(hDevice);
    if (pDevContext->PortController != WDF_NO_HANDLE)  {        
        UcmTcpciPortControllerStop(pDevContext->PortController);   // Direct UcmTcpciCx to stop the port controller and then delete the backing object.
        WdfObjectDelete(pDevContext->PortController);
        pDevContext->PortController = WDF_NO_HANDLE;
    }    
    I2C_Close(pDevContext);                                         // Close the I2C controller.
    GPIO_Close(pDevContext);                                        // Close the GPIO controller.
    return ntStatus;
}
