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
#pragma alloc_text (PAGE, HardwareRequestQueueInitialize)
#endif

#define RdRegMult(_Items) if (!NT_SUCCESS(ntStatus = I2C_ReadSynchronouslyMultiple(pDevContext, I2CRequestSourceClient, _Items, _countof(_Items)))) {break;}
#define RdReg(_Reg, _Buff) if (!NT_SUCCESS(ntStatus = I2C_ReadSynchronously(pDevContext, I2CRequestSourceClient, _Reg, &_Buff, sizeof(_Buff)))) {break;} 
#define WrReg(_Reg, _Buff) if (!NT_SUCCESS(ntStatus = I2C_WriteSynchronously(pDevContext, I2CRequestSourceClient, _Reg, &_Buff, sizeof(_Buff)))) {break;} 

/*++
Routine Description:
    Because EvtIoDeviceControl was called at dispatch-level, it cannot send I/O and then wait synchronously;
    instead, the work has to be postponed to a passive-level workitem.
Arguments:
    Request  - Handle to a framework hRequest object.
    Device   - Handle to a framework device object.
    WorkItem - Handle to a framework workitem object
--*/
void PostponeToWorkitem(_In_ WDFREQUEST hRequest, _In_ WDFDEVICE hDevice, _In_ WDFWORKITEM hWorkItem) {
    PWORKITEM_CONTEXT pWorkItemContext;

    pWorkItemContext = WorkitemGetContext(hWorkItem);
    pWorkItemContext->Device = hDevice;
    pWorkItemContext->Request = hRequest;
    WdfWorkItemEnqueue(hWorkItem);
}

/*++
Routine Description:
    This routine handles IOCTL_UCMTCPCI_PORT_CONTROLLER_GET_STATUS.
    Read the contents of the CC ntStatus, Fault ntStatus, and Power ntStatus registers from the device and complete the WDFREQUEST.
Arguments:
    WorkItem - Handle to a framework workitem object.
--*/
void Evt_WorkItem_GetStatus(_In_ WDFWORKITEM hWorkItem) {

    PUCMTCPCI_PORT_CONTROLLER_GET_STATUS_OUT_PARAMS pGetStatusOutParams;
    PWORKITEM_CONTEXT                               pWorkItemContext;
    PDEVICE_CONTEXT                                 pDevContext;
    WDFREQUEST                                      hRequest;
    NTSTATUS                                        ntStatus;

    pWorkItemContext = WorkitemGetContext(hWorkItem);
    pDevContext      = DeviceGetContext(pWorkItemContext->Device);
    hRequest         = pWorkItemContext->Request;
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveOutputBuffer(hRequest, sizeof(*pGetStatusOutParams), reinterpret_cast<PVOID*>(&pGetStatusOutParams), NULL))) {
            DBG_PRINT_ERROR("WdfRequestRetrieveOutputBuffer failed. ntStatus: 0x%08X", ntStatus);
            break;
        }
        REGISTER_ITEM items[] = {
            GEN_REGISTER_ITEM(TCPC_PHY_CC_STATUS,    pGetStatusOutParams->CCStatus),
            GEN_REGISTER_ITEM(TCPC_PHY_POWER_STATUS, pGetStatusOutParams->PowerStatus),
            GEN_REGISTER_ITEM(TCPC_PHY_FAULT_STATUS, pGetStatusOutParams->FaultStatus),
        };
        if (!NT_SUCCESS(ntStatus = I2C_ReadSynchronouslyMultiple(pDevContext, I2CRequestSourceClient, items, _countof(items)))) {
            break;
        }
        WdfRequestSetInformation(hRequest, sizeof(*pGetStatusOutParams));
    } while (0);
    WdfRequestComplete(hRequest, ntStatus);
}

/*++
Routine Description:
    This routine handles IOCTL_UCMTCPCI_PORT_CONTROLLER_GET_CONTROL.
    Read the contents of the TCPC Control, Role Control, Fault Control, and Power Control registers from the device and complete the WDFREQUEST.
Arguments:
    WorkItem - Handle to a framework workitem object.
--*/
void Evt_WorkItem_GetControl(_In_ WDFWORKITEM hWorkItem) {
    PUCMTCPCI_PORT_CONTROLLER_GET_CONTROL_OUT_PARAMS pGetControlOutParams;
    PWORKITEM_CONTEXT                                pWorkItemContext;
    PDEVICE_CONTEXT                                  pDevContext;
    WDFREQUEST                                       hRequest;
    NTSTATUS                                         ntStatus;

    pWorkItemContext = WorkitemGetContext(hWorkItem);
    pDevContext      = DeviceGetContext(pWorkItemContext->Device);
    hRequest         = pWorkItemContext->Request;
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveOutputBuffer(hRequest, sizeof(*pGetControlOutParams), reinterpret_cast<PVOID*>(&pGetControlOutParams), NULL))) {
            DBG_PRINT_ERROR( "WdfRequestRetrieveOutputBuffer failed. ntStatus: 0x%08X", ntStatus);
            break;
        }
        REGISTER_ITEM items[] = {
            GEN_REGISTER_ITEM(TCPC_PHY_TCPC_CONTROL,  pGetControlOutParams->TCPCControl),
            GEN_REGISTER_ITEM(TCPC_PHY_ROLE_CONTROL,  pGetControlOutParams->RoleControl),
            GEN_REGISTER_ITEM(TCPC_PHY_FAULT_CONTROL, pGetControlOutParams->FaultControl),
            GEN_REGISTER_ITEM(TCPC_PHY_POWER_CONTROL, pGetControlOutParams->PowerControl),
        };
        if (!NT_SUCCESS(ntStatus = I2C_ReadSynchronouslyMultiple(pDevContext, I2CRequestSourceClient, items, _countof(items)))) {
            break;
        }
        WdfRequestSetInformation(hRequest, sizeof(*pGetControlOutParams));
    } while (0);
    WdfRequestComplete(hRequest, ntStatus);
}

/*++
Routine Description:
    Set the contents of the TCPC Control, Role Control, Fault Control, or Power Control registers on the device and complete the WDFREQUEST.
Arguments:
    WorkItem - Handle to a framework workitem object.
--*/
void Evt_WorkItem_SetControl(_In_ WDFWORKITEM hWorkItem) {
    PWORKITEM_CONTEXT                                pWorkItemContext;
    PDEVICE_CONTEXT                                  pDevContext;
    WDFREQUEST                                       hRequest;
    NTSTATUS                                         ntStatus;
    PUCMTCPCI_PORT_CONTROLLER_SET_CONTROL_IN_PARAMS  pSetControlInParams;
    TCPC_PHY_t                                      *pPhyRegs;

    pWorkItemContext = WorkitemGetContext(hWorkItem);
    pDevContext      = DeviceGetContext(pWorkItemContext->Device);
    hRequest         = pWorkItemContext->Request;

    pPhyRegs = &pDevContext->TCPI_PhyRegs;
    if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_COMMAND_IN_PARAMS), reinterpret_cast<PVOID*>(&pSetControlInParams), NULL))) {
        DBG_PRINT_ERROR("WdfRequestRetrieveInputBuffer failed. ntStatus: 0x%08X", ntStatus);
    } else {
        switch (pSetControlInParams->ControlType) {
        case UcmTcpciPortControllerTcpcControl:    // TCPC_CONTROL - First write 
            if (pDevContext->hGPIOIoTarget != WDF_NO_HANDLE) {
                GPIO_PlugOrientation_Set(pDevContext, *(TCPC_PHY_TCPC_CONTROL_t *)(&pSetControlInParams->TCPCControl));
            }
            WrReg(TCPC_PHY_TCPC_CONTROL, pSetControlInParams->TCPCControl);
            break;
        case UcmTcpciPortControllerRoleControl:    // ROLE_CONTROL - Second write 
            WrReg(TCPC_PHY_ROLE_CONTROL, pSetControlInParams->RoleControl);
            break;
        case UcmTcpciPortControllerPowerControl: { // POWER_CONTROL - Third write
                                                   // PNT5110 bug fix
            if (pSetControlInParams->PowerControl.EnableVconn) {
                if (pPhyRegs->TCPC_CONTROL.B.PLUG_ORIENTATION) { // PC.VCON = 1 & TC.PlugOrient = 1 (VCON to CC1)
                    if (pPhyRegs->ROLE_CONTROL.B.CC1 != 0x03) {  // PC.VCON = 1 & TC.PlugOrient = 1 &RC.CC1 != Open
                        DBG_PRINT_ERROR("PC.VCON = 1 & TC.PlugOrient = 1 &RC.CC1 != Open, Setting RC.CC1 = Open(0x03)");
                        pPhyRegs->ROLE_CONTROL.B.CC1 = 0x03;     // RC.CC1 = Open
                        WrReg(TCPC_PHY_ROLE_CONTROL, pPhyRegs->ROLE_CONTROL);
                    }   
                } else {                                         // PC.VCON = 1 & TC.PlugOrient = 0 (VCON to CC2)
                    if (pPhyRegs->ROLE_CONTROL.B.CC2 != 0x03) {  // PC.VCON = 1 & TC.PlugOrient = 0 &RC.CC2 != Open
                        pPhyRegs->ROLE_CONTROL.B.CC2 = 0x03;     // RC.CC1 = Open
                        DBG_PRINT_ERROR("PC.VCON = 1 & TC.PlugOrient =  &RC.CC2 != Open, Setting RC.CC2 = Open(0x03)");
                        WrReg(TCPC_PHY_ROLE_CONTROL, pPhyRegs->ROLE_CONTROL);
                    }   
                }
            }
            WrReg(TCPC_PHY_POWER_CONTROL, pSetControlInParams->PowerControl);
            break;
        }
        case UcmTcpciPortControllerFaultControl:
            WrReg(TCPC_PHY_FAULT_CONTROL, pSetControlInParams->FaultControl);
            break;
        default:
            DBG_PRINT_ERROR("Invalid control register type.");
            ntStatus = STATUS_INVALID_DEVICE_REQUEST;
        }
    }
    WdfRequestComplete(hRequest, ntStatus);
}

/*++
Routine Description: 
    Set the contents of the command register on the device and complete the WDFREQUEST.
Arguments:
    Request - Handle to a framework hRequest object.
    Device  - Handle to a framework device object.
--*/
void Evt_SetCommand(_In_ WDFREQUEST hRequest, _In_ WDFDEVICE hDevice) {
    NTSTATUS                                        ntStatus;
    PDEVICE_CONTEXT                                 pDevContext;
    PUCMTCPCI_PORT_CONTROLLER_SET_COMMAND_IN_PARAMS inParams;
    UINT8                                           Cmd;

    pDevContext = DeviceGetContext(hDevice);
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_COMMAND_IN_PARAMS), reinterpret_cast<PVOID*>(&inParams), NULL))) {
            DBG_PRINT_ERROR("WdfRequestRetrieveInputBuffer failed. ntStatus: 0x%08X", ntStatus);
            break;
        }
        // UCMTCPCI_PORT_CONTROLLER_COMMAND (from inParams->Command) is defined as an enum.
        // Thus sizeof() typically returns 4 bytes, instead of 1 byte as required by TCPCI spec.
        // The workaround is to copy it to a local 1-byte variable before writing it down.
        Cmd = static_cast<UINT8>(inParams->Command);
        if (!NT_SUCCESS(ntStatus = I2C_WriteAsynchronously(pDevContext, TCPC_PHY_COMMAND, &Cmd, sizeof(Cmd)))) {
            break;
        }
    } while (0);
}

/*++
Routine Description:
    Set the contents of the config standard output register on the device and complete the WDFREQUEST.
Arguments:
    Request - Handle to a framework hRequest object.
    Device  - Handle to a framework device object.
--*/
void Evt_SetConfigStandardOutput(_In_ WDFREQUEST hRequest, _In_ WDFDEVICE hDevice) {
    NTSTATUS                                                       ntStatus;
    PDEVICE_CONTEXT                                                pDevContext;
    PUCMTCPCI_PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT_IN_PARAMS inParams;

    pDevContext = DeviceGetContext(hDevice);
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT_IN_PARAMS), reinterpret_cast<PVOID*>(&inParams), NULL))) {
            DBG_PRINT_ERROR("WdfRequestRetrieveInputBuffer failed. ntStatus: 0x%08X", ntStatus);
            break;
        }
        if (!NT_SUCCESS(ntStatus = I2C_WriteAsynchronously(pDevContext, TCPC_PHY_CONFIG_STANDARD_OUTPUT, &inParams->ConfigStandardOutput, sizeof(inParams->ConfigStandardOutput)))) {
            break;
        }
    } while (0);
}

/*++
Routine Description:
    Set the contents of the message header info register on the device and complete the WDFREQUEST.
Arguments:
    Request - Handle to a framework hRequest object.
    Device  - Handle to a framework device object.
--*/
void Evt_SetMessageHeaderInfo(_In_ WDFREQUEST hRequest, _In_ WDFDEVICE hDevice) {
    NTSTATUS                                                    ntStatus;
    PDEVICE_CONTEXT                                             pDevContext;
    PUCMTCPCI_PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO_IN_PARAMS inParams;

    pDevContext = DeviceGetContext(hDevice);
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO_IN_PARAMS), reinterpret_cast<PVOID*>(&inParams), NULL))) {
            DBG_PRINT_ERROR("WdfRequestRetrieveInputBuffer failed. ntStatus: 0x%08X", ntStatus);
            break;
        }
        if (!NT_SUCCESS(ntStatus = I2C_WriteAsynchronously(pDevContext, TCPC_PHY_MESSAGE_HEADER_INFO, &inParams->MessageHeaderInfo, sizeof(inParams->MessageHeaderInfo)))) {
            break;
        }
    } while (0);
}

/*++
Routine Description: 
    Set the contents of the receive detect register on the device and complete the WDFREQUEST.
Arguments: 
    Request - Handle to a framework hRequest object.
    Device  - Handle to a framework device object.
--*/
void Evt_SetReceiveDetect(_In_ WDFREQUEST hRequest, _In_ WDFDEVICE hDevice) {
    NTSTATUS                                               ntStatus;
    PDEVICE_CONTEXT                                        pDevContext;
    PUCMTCPCI_PORT_CONTROLLER_SET_RECEIVE_DETECT_IN_PARAMS inParams;

    pDevContext = DeviceGetContext(hDevice);
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_RECEIVE_DETECT_IN_PARAMS), reinterpret_cast<PVOID*>(&inParams), NULL))) {
        DBG_PRINT_ERROR("WdfRequestRetrieveInputBuffer failed. ntStatus: 0x%08X", ntStatus);
        break;
        }
        if (!NT_SUCCESS(ntStatus = I2C_WriteAsynchronously(pDevContext, TCPC_PHY_RECEIVE_DETECT, &inParams->ReceiveDetect, sizeof(inParams->ReceiveDetect)))) {
            break;
        }
    } while (0);
}

/*++
Routine Description: 
    Set the contents of the transmit register on the device and complete the WDFREQUEST.
Arguments: 
    Request - Handle to a framework hRequest object.
    Device  - Handle to a framework device object.
--*/
void Evt_SetTransmit(_In_ WDFREQUEST hRequest, _In_ WDFDEVICE hDevice) {
    NTSTATUS                                         ntStatus;
    PDEVICE_CONTEXT                                  pDevContext;
    PUCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_IN_PARAMS inParams;

    pDevContext = DeviceGetContext(hDevice);
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(PUCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_IN_PARAMS), reinterpret_cast<PVOID*>(&inParams), NULL))) {
        DBG_PRINT_ERROR("WdfRequestRetrieveInputBuffer failed. ntStatus: 0x%08X", ntStatus);
        break;
        }
        if (!NT_SUCCESS(ntStatus = I2C_WriteAsynchronously(pDevContext, TCPC_PHY_TRANSMIT, &inParams->Transmit, sizeof(inParams->Transmit)))) {
            break;
        }
    } while (0);
}

/*++
Routine Description: 
    Set the contents of the transmit buffer register on the device and complete the WDFREQUEST.
Arguments:
    Request - Handle to a framework hRequest object.
    Device  - Handle to a framework device object.
--*/
void Evt_SetTransmitBuffer(_In_ WDFREQUEST hRequest, _In_ WDFDEVICE hDevice) {
    NTSTATUS                                                ntStatus;
    PDEVICE_CONTEXT                                         pDevContext;
    PUCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_BUFFER_IN_PARAMS inParams;

    pDevContext = DeviceGetContext(hDevice);
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_BUFFER_IN_PARAMS), reinterpret_cast<PVOID*>(&inParams), NULL))) {
        DBG_PRINT_ERROR("WdfRequestRetrieveInputBuffer failed. ntStatus: 0x%08X", ntStatus);
        break;
        }
        if (!NT_SUCCESS(ntStatus = I2C_WriteAsynchronously(pDevContext, TCPC_PHY_TX_BUF_BYTE_X, &inParams->TransmitBuffer, inParams->TransmitBuffer.TransmitByteCount + sizeof(inParams->TransmitBuffer.TransmitByteCount)))) {
            break;
        }
    } while (0);
}

/*++
Routine Description:
     The I/O dispatch callbacks for the frameworks hDevice object are configured in this function.
     A single I/O Queue is configured for sequential request processing, and a driver context memory allocation is created to hold our structure QUEUE_CONTEXT.
Arguments:
     Device - Handle to a framework hDevice object.
Returns:
    NTSTATUS
--*/
NTSTATUS HardwareRequestQueueInitialize(_In_ WDFDEVICE hDevice) {
    NTSTATUS            ntStatus;
    WDFQUEUE            hQueue;
    WDF_IO_QUEUE_CONFIG QueueConfig;
    PDEVICE_CONTEXT     pDevContext;

    PAGED_CODE();
    pDevContext = DeviceGetContext(hDevice);
    WDF_IO_QUEUE_CONFIG_INIT(&QueueConfig, WdfIoQueueDispatchSequential);
    QueueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
    QueueConfig.EvtIoStop          = EvtIoStop;
    do {
        if (!NT_SUCCESS(ntStatus = WdfIoQueueCreate(hDevice, &QueueConfig, WDF_NO_OBJECT_ATTRIBUTES, &hQueue))) {  // Create the hardware request queue.
            DBG_PRINT_ERROR("WdfIoQueueCreate failed STATUS = 0x%08X", ntStatus);
            break;
        }
    } while (0);
    UcmTcpciPortControllerSetHardwareRequestQueue(pDevContext->PortController, hQueue);                         // Set this queue as the one to which UcmTcpciCx will forward its hardware requests.
    return ntStatus;
}

/*++
Routine Description:
    This event is invoked when the framework receives IRP_MJ_DEVICE_CONTROL request. Called at dispatch-level.
Arguments:
    Queue              - Handle to the framework queue object that is associated with the I/O request.
    Request            - Handle to a framework request object.
    OutputBufferLength - Size of the output buffer in bytes
    InputBufferLength  - Size of the input buffer in bytes
    IoControlCode      - I/O control code.
--*/
VOID EvtIoDeviceControl(_In_ WDFQUEUE hQueue, _In_ WDFREQUEST hRequest, _In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode) {
    WDFDEVICE       hDevice;
    PDEVICE_CONTEXT pDevContext;

    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(OutputBufferLength);

    hDevice                      = WdfIoQueueGetDevice(hQueue);
    pDevContext                  = DeviceGetContext(hDevice);
    pDevContext->IncomingRequest = hRequest;
    switch (IoControlCode) {
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_GET_STATUS:
            PostponeToWorkitem(hRequest, hDevice, pDevContext->I2CWorkItemGetStatus);
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_GET_CONTROL:
            PostponeToWorkitem(hRequest, hDevice, pDevContext->I2CWorkItemGetControl);
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_CONTROL:
            PostponeToWorkitem(hRequest, hDevice, pDevContext->I2CWorkItemSetControl);
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT:
            Evt_SetTransmit(hRequest, hDevice);
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_BUFFER:
            Evt_SetTransmitBuffer(hRequest, hDevice);
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_RECEIVE_DETECT:
            Evt_SetReceiveDetect(hRequest, hDevice);
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT:
            Evt_SetConfigStandardOutput(hRequest, hDevice);
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_COMMAND:
            Evt_SetCommand(hRequest, hDevice);
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO:
            Evt_SetMessageHeaderInfo(hRequest, hDevice);
            break;
        default:
            DBG_PRINT_ERROR("Received unexpected IoControlCode 0x%08X", IoControlCode);
            pDevContext->IncomingRequest = WDF_NO_HANDLE;      
            WdfRequestComplete(hRequest, STATUS_NOT_SUPPORTED);  // If we don't recognize the IOCTL, complete the WDFREQUEST here.
    }
}

/*++
Routine Description:
    This event is invoked for a power-managed queue before the hDevice leaves the working state (D0).
Arguments:
    Queue       - Handle to the framework queue object that is associated with the I/O request.
    Request     - Handle to a framework request object.
    ActionFlags - A bitwise OR of one or more WDF_REQUEST_STOP_ACTION_FLAGS-typed flags that identify the reason that the callback function is being called and whether the request is cancelable.
--*/
VOID EvtIoStop(_In_ WDFQUEUE hQueue, _In_ WDFREQUEST hRequest, _In_ ULONG ActionFlags) {
    UNREFERENCED_PARAMETER(ActionFlags);
    UNREFERENCED_PARAMETER(hRequest);

    WDFDEVICE       hDevice;
    PDEVICE_CONTEXT pDevContext;

    hDevice = WdfIoQueueGetDevice(hQueue);
    pDevContext = DeviceGetContext(hDevice);

    // Attempt to cancel the I2C WDFREQUESTs.
    if (pDevContext->I2CAsyncRequest != WDF_NO_HANDLE) {
        DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] Attempting to cancel.", pDevContext->I2CAsyncRequest);
        WdfRequestCancelSentRequest(pDevContext->I2CAsyncRequest);
    }
}