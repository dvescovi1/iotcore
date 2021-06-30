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
#pragma alloc_text (PAGE, I2C_Initialize)
#pragma alloc_text (PAGE, I2C_Open)
#pragma alloc_text (PAGE, I2C_Close)
#pragma alloc_text (PAGE, I2C_ReadSynchronously)
#pragma alloc_text (PAGE, I2C_WriteSynchronously)
#pragma alloc_text (PAGE, I2C_PerformDeviceReset)
#pragma alloc_text (PAGE, I2C_ReadSynchronouslyMultiple)

#pragma alloc_text (PAGE, GPIO_Open)
#pragma alloc_text (PAGE, GPIO_Close)
#endif

void UpdateRegsInDevContext(_In_ PDEVICE_CONTEXT pDevContext, UINT8 RegisterAddress, _In_ PVOID pBuffer,_In_ ULONG RegSize) {
    /* Update registers values in device context */
    if (RegSize == 1) {
        ((UINT8*)(&pDevContext->TCPI_PhyRegs))[RegisterAddress] = *(UINT8*)pBuffer;
    } else if (RegSize == 2) {
        *(UINT16*)&(((UINT8*)(&pDevContext->TCPI_PhyRegs))[RegisterAddress]) = *(UINT16*)pBuffer;
    } 
}
/*++
Routine Description:
    Initialize the I2C resource that provides a communications channel to the port controller hardware.
Arguments:
    pDevContext         - Context for a framework device.
    ResourcesRaw        - A handle to a framework resource-list object that identifies the raw hardware resources that the Plug and Play manager has assigned to the device.
    ResourcesTranslated - A handle to a framework resource-list object that identifies the translated hardware resources that the Plug and Play manager has assigned to the device.
Return Value:  NTSTATUS
--*/
NTSTATUS I2C_Initialize(_In_ PDEVICE_CONTEXT pDevContext, _In_ WDFCMRESLIST hResourcesRaw, _In_ WDFCMRESLIST hResourcesTranslated) {
    NTSTATUS                        ntStatus = STATUS_SUCCESS;
    WDF_INTERRUPT_CONFIG            interruptConfig;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR descriptor = nullptr;
    ULONG                           interruptIndex = 0;
    BOOLEAN                         connFound = FALSE;
    BOOLEAN                         interruptFound = FALSE;
    ULONG                           resourceCount;
    int                             pinCount = 0;

    PAGED_CODE();
    UNREFERENCED_PARAMETER(hResourcesRaw);
    // Check for I2C and Interrupt resources from the resources that PnP manager has allocated to our device.
    resourceCount = WdfCmResourceListGetCount(hResourcesTranslated);
    for (ULONG i = 0; i < resourceCount; i++) {
        descriptor = WdfCmResourceListGetDescriptor(hResourcesTranslated, i);
        switch (descriptor->Type) {
            case CmResourceTypeConnection:  // Check for I2C resource            
                if (descriptor->u.Connection.Class == CM_RESOURCE_CONNECTION_CLASS_SERIAL && descriptor->u.Connection.Type == CM_RESOURCE_CONNECTION_TYPE_SERIAL_I2C) {
                    pDevContext->I2CConnectionId.LowPart  = descriptor->u.Connection.IdLowPart;
                    pDevContext->I2CConnectionId.HighPart = descriptor->u.Connection.IdHighPart;
                    connFound = TRUE;
                    DBG_PRINT_INFO("I2C resource found with connection id: 0x%llx", pDevContext->I2CConnectionId.QuadPart);
                }
                if ((descriptor->u.Connection.Class == CM_RESOURCE_CONNECTION_CLASS_GPIO) && (descriptor->u.Connection.Type == CM_RESOURCE_CONNECTION_TYPE_GPIO_IO)) {
                    if (++pinCount == 2) {
                        pDevContext->GPIOConnectionId.LowPart  = descriptor->u.Connection.IdLowPart;
                        pDevContext->GPIOConnectionId.HighPart = descriptor->u.Connection.IdHighPart;
                        DBG_PRINT_INFO("USB Type-C SuperSpeed switch pin resource found with connection id: 0x%llx", pDevContext->GPIOConnectionId.QuadPart);
                    }
                }
                break;
            case CmResourceTypeInterrupt:   // We've found an interrupt resource.           
                interruptFound = TRUE;
                interruptIndex = i;
                DBG_PRINT_INFO("Interrupt resource found at index: %lu", interruptIndex);
                break;
            default:            
                break;                       // We don't care about other descriptors.
        }
    }    
    do {
        if (!connFound) {                    // Fail if either connection or interrupt resource was not found.
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
            DBG_PRINT_ERROR("Failed finding required I2C resource. ntStatus: 0x%08X", ntStatus);
            break;
        }
        if (!interruptFound) {
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
            DBG_PRINT_ERROR("Failed finding required interrupt resource. ntStatus: 0x%08X", ntStatus);
            break;
        }
        // The alerts from the port controller hardware will be handled in a passive ISR.
        // The ISR performs hardware read and write operations which block until the hardware access is complete. Waiting is unacceptable at DIRQL, so we perform our ISR at PASSIVE_LEVEL.
        WDF_INTERRUPT_CONFIG_INIT(&interruptConfig, OnInterruptPassiveIsr, NULL);
        interruptConfig.PassiveHandling     = TRUE;
        interruptConfig.InterruptTranslated = WdfCmResourceListGetDescriptor(hResourcesTranslated, interruptIndex);
        interruptConfig.InterruptRaw        = WdfCmResourceListGetDescriptor(hResourcesRaw, interruptIndex);
        if (!NT_SUCCESS(ntStatus = WdfInterruptCreate(pDevContext->Device, &interruptConfig, WDF_NO_OBJECT_ATTRIBUTES, &pDevContext->AlertInterrupt))) {
            DBG_PRINT_ERROR("WdfInterruptCreate failed. ntStatus: 0x%08X", ntStatus);
            break;
        }
    } while (0);
    return ntStatus;
}

NTSTATUS GPIO_Open(_In_ PDEVICE_CONTEXT pDevContext) {
    NTSTATUS                        ntStatus;
    WDF_IO_TARGET_OPEN_PARAMS       OpenParams;
    WDF_OBJECT_ATTRIBUTES           Attributes;
    DECLARE_UNICODE_STRING_SIZE(usDevicePath, RESOURCE_HUB_PATH_SIZE);      // Create the device path using the connection ID.

    PAGED_CODE();
    do {
        if (pDevContext->GPIOConnectionId.QuadPart == 0) {
            ntStatus = STATUS_SUCCESS;
            break;
        }
        RESOURCE_HUB_CREATE_PATH_FROM_ID(&usDevicePath, pDevContext->GPIOConnectionId.LowPart, pDevContext->GPIOConnectionId.HighPart);
        if (!NT_SUCCESS(ntStatus = WdfIoTargetCreate(pDevContext->Device, WDF_NO_OBJECT_ATTRIBUTES, &pDevContext->hGPIOIoTarget))) {
            DBG_PRINT_ERROR("GPIO - WdfIoTargetCreate() failed - STATUS = 0x%08X", ntStatus);
            break;
        }
        // Open a handle to the controller.
        WDF_IO_TARGET_OPEN_PARAMS_INIT_OPEN_BY_NAME(&OpenParams, &usDevicePath, GENERIC_WRITE);
        if (!NT_SUCCESS(ntStatus = WdfIoTargetOpen(pDevContext->hGPIOIoTarget, &OpenParams))) {
            DBG_PRINT_ERROR("GPIO - WdfIoTargetOpen() failed - STATUS = 0x%08X", ntStatus);
            break;
        }
        WDF_OBJECT_ATTRIBUTES_INIT(&Attributes);
        Attributes.ParentObject = pDevContext->hGPIOIoTarget;
        if (!NT_SUCCESS(ntStatus = WdfRequestCreate(&Attributes, pDevContext->hGPIOIoTarget, &pDevContext->hGPIOIoctlRequest))) {
            DBG_PRINT_ERROR("GPIO - WdfRequestCreate() failed - STATUS = 0x%08X", ntStatus);
            break;
        }
        WDF_OBJECT_ATTRIBUTES_INIT(&Attributes);
        Attributes.ParentObject = pDevContext->hGPIOIoctlRequest;
        if (!NT_SUCCESS(ntStatus = WdfMemoryCreatePreallocated(&Attributes, &pDevContext->GPIOIoctlData, sizeof(pDevContext->GPIOIoctlData), &pDevContext->hGPIOWdfMemory))) {
            DBG_PRINT_ERROR("GPIO - WdfMemoryCreatePreallocated() failed - STATUS = 0x%08X", ntStatus);
            break;
        }
    } while (0);
    return ntStatus;
}

/*++
Routine Description:
    This routine opens a handle to the I2C controller.
Arguments:
    pDevContext - a pointer to the device context
Return Value:
    NTSTATUS
--*/
NTSTATUS I2C_Open(_In_ PDEVICE_CONTEXT pDevContext) {
    NTSTATUS                  ntStatus;
    WDF_IO_TARGET_OPEN_PARAMS OpenParams;
    WDF_OBJECT_ATTRIBUTES     RequestAttributes;
    WDF_OBJECT_ATTRIBUTES     WorkitemAttributes;
    WDF_WORKITEM_CONFIG       WorkitemConfig;

    PAGED_CODE();
    // Create the device path using the connection ID.
    DECLARE_UNICODE_STRING_SIZE(usDevicePath, RESOURCE_HUB_PATH_SIZE);
    do {
        RESOURCE_HUB_CREATE_PATH_FROM_ID(&usDevicePath, pDevContext->I2CConnectionId.LowPart, pDevContext->I2CConnectionId.HighPart);
        if (!NT_SUCCESS(ntStatus = WdfIoTargetCreate(pDevContext->Device, WDF_NO_OBJECT_ATTRIBUTES, &pDevContext->hI2CIoTarget))) {
            DBG_PRINT_ERROR("WdfIoTargetCreate failed - STATUS = 0x%08X", ntStatus);
            break;
        }
        // Open a handle to the I2C controller.
        WDF_IO_TARGET_OPEN_PARAMS_INIT_OPEN_BY_NAME(&OpenParams, &usDevicePath, (GENERIC_READ | GENERIC_WRITE));
        OpenParams.ShareAccess       = 0;
        OpenParams.CreateDisposition = FILE_OPEN;
        OpenParams.FileAttributes    = FILE_ATTRIBUTE_NORMAL;
        if (!NT_SUCCESS(ntStatus = WdfIoTargetOpen(pDevContext->hI2CIoTarget, &OpenParams))) {
            DBG_PRINT_ERROR("Failed to open I2C I/O target - STATUS = 0x%08X", ntStatus);
            break;
        }
        // Create a WDFMEMORY object. Do call WdfMemoryAssignBuffer before use it,
        if (!NT_SUCCESS(ntStatus = WdfMemoryCreatePreallocated(WDF_NO_OBJECT_ATTRIBUTES, static_cast<PVOID>(&ntStatus), sizeof(ntStatus), &pDevContext->I2CMemory))) {
            DBG_PRINT_ERROR("WdfMemoryCreatePreallocated failed - STATUS = 0x%08X", ntStatus);
            break;
        }
        /* Create I2C WDF requests */
        WDF_OBJECT_ATTRIBUTES_INIT(&RequestAttributes);
        RequestAttributes.ParentObject = pDevContext->hI2CIoTarget;
        for (ULONG i = 0; i < I2CRequestSourceMax; i++) {
            if (!NT_SUCCESS(ntStatus = WdfRequestCreate(&RequestAttributes, pDevContext->hI2CIoTarget, &pDevContext->OutgoingRequests[i]))) {
                DBG_PRINT_ERROR("WdfRequestCreate failed - STATUS = 0x%08X", ntStatus);
                break;
            }
        }
        WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&WorkitemAttributes, WORKITEM_CONTEXT);
        WorkitemAttributes.ParentObject = pDevContext->hI2CIoTarget;
        WDF_WORKITEM_CONFIG_INIT(&WorkitemConfig, Evt_WorkItem_GetStatus);
        if (!NT_SUCCESS(ntStatus = WdfWorkItemCreate(&WorkitemConfig, &WorkitemAttributes, &pDevContext->I2CWorkItemGetStatus))) {
            DBG_PRINT_ERROR("WdfWorkItemCreate failed - STATUS = 0x%08X", ntStatus);
            break;
        }
        WDF_WORKITEM_CONFIG_INIT(&WorkitemConfig, Evt_WorkItem_GetControl);
        if (!NT_SUCCESS(ntStatus = WdfWorkItemCreate(&WorkitemConfig, &WorkitemAttributes, &pDevContext->I2CWorkItemGetControl))) {
            DBG_PRINT_ERROR("WdfWorkItemCreate failed - STATUS = 0x%08X", ntStatus);
            break;
        }
        WDF_WORKITEM_CONFIG_INIT(&WorkitemConfig, Evt_WorkItem_SetControl);
        if (!NT_SUCCESS(ntStatus = WdfWorkItemCreate(&WorkitemConfig, &WorkitemAttributes, &pDevContext->I2CWorkItemSetControl))) {
            DBG_PRINT_ERROR("WdfWorkItemCreate failed - STATUS = 0x%08X", ntStatus);
            break;
        }
    } while (0);
    return ntStatus;
}

/*++
Routine Description:
    This routine closes a handle to the I2C controller.
Arguments:
    pDevContext - a pointer to the device context.
--*/
void I2C_Close(_In_ PDEVICE_CONTEXT pDevContext) {
    PAGED_CODE();
    if (pDevContext->hI2CIoTarget != WDF_NO_HANDLE) {
        WdfIoTargetClose(pDevContext->hI2CIoTarget);
    }
}

/*++
Routine Description:
    This routine closes a handle to the I2C controller.
Arguments:
    pDevContext - a pointer to the device context.
--*/
void GPIO_Close(_In_ PDEVICE_CONTEXT pDevContext) {
    PAGED_CODE();
    if (pDevContext->hGPIOIoTarget != WDF_NO_HANDLE) {
        WdfIoTargetClose(pDevContext->hGPIOIoTarget);
    }
}

/*++
Routine Description:
    This routine closes a handle to the I2C controller.
Arguments:
    pDevContext - a pointer to the device context.
--*/
NTSTATUS GPIO_PlugOrientation_Set(_In_ PDEVICE_CONTEXT pDevContext, TCPC_PHY_TCPC_CONTROL_t TCPC_CONTROL) {
    NTSTATUS                 ntStatus = STATUS_SUCCESS;

    do {
        if (TCPC_CONTROL.B.PLUG_ORIENTATION) {
            pDevContext->GPIOIoctlData = 0x00;
        } else {
            pDevContext->GPIOIoctlData = 0x01;
        }
        DBG_PRINT_REG("GPIO_PlugOrientation_Set","Setting Plug Orientation GPIO pin, value = %d", pDevContext->GPIOIoctlData);
        if (!NT_SUCCESS(ntStatus = WdfIoTargetFormatRequestForIoctl(pDevContext->hGPIOIoTarget, pDevContext->hGPIOIoctlRequest, IOCTL_GPIO_WRITE_PINS, pDevContext->hGPIOWdfMemory, 0, pDevContext->hGPIOWdfMemory, 0))) {
            DBG_PRINT_ERROR("GPIO - WdfIoTargetFormatRequestForIoctl() failed - STATUS = 0x%08X", ntStatus);
            break;
        }
        // Send the request to the GPIO Target.
        if (WdfRequestSend(pDevContext->hGPIOIoctlRequest, pDevContext->hGPIOIoTarget, NULL) == FALSE) {
            ntStatus = WdfRequestGetStatus(pDevContext->hGPIOIoctlRequest);
            DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] WdfRequestSend for GPIO write failed with ntStatus 0x%08X", pDevContext->hGPIOIoctlRequest, ntStatus);
            break;
        }
    } while (0);
    return ntStatus;
}

/*++
Routine Description:
    Sends data to the I2C controller to write to the specified register on the port controller hardware.
    Before calling, pDevContext->IncomingRequest should be set to a valid value. The IncomingRequest will be completed automatically when writing is finished.
Arguments:
    pDevContext     - Context information about the device.
    RegisterAddress - The I2C register address to write data to.
    Data            - Pointer to the data to write.
    Length          - Length of the data to write.
Return Value:
    NTSTATUS
--*/
NTSTATUS I2C_WriteAsynchronously(_In_ PDEVICE_CONTEXT pDevContext, _In_ UINT8 RegisterAddress, _In_reads_bytes_(Length) PVOID Data, _In_ ULONG Length) {
    NTSTATUS                 ntStatus;
    WDF_REQUEST_REUSE_PARAMS reuseParams;

    do {
        if (Length == 0) {
            DBG_PRINT_ERROR("Parameter 'Length' cannot be 0.");
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
        }
        if (REGISTER_ADDR_SIZE + Length > sizeof(pDevContext->I2CAsyncBuffer)) {
            DBG_PRINT_ERROR("Unexpected value of data length. Length: %lu. Size of buffer: %lu", Length, sizeof(pDevContext->I2CAsyncBuffer));
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
        }
        // Reuse the preallocated WDFREQUEST for I2C.
        WDF_REQUEST_REUSE_PARAMS_INIT(&reuseParams, WDF_REQUEST_REUSE_NO_FLAGS, STATUS_SUCCESS);
        if (!NT_SUCCESS(ntStatus = WdfRequestReuse(pDevContext->I2CAsyncRequest, &reuseParams))) {
            DBG_PRINT_ERROR("WdfRequestReuse failed with ntStatus 0x%08X", ntStatus);
            break;
        }
        UpdateRegsInDevContext(pDevContext, RegisterAddress, Data, Length); /* Update registers values in device context */
        TCPC_PHY_DumpReg(&pDevContext->TCPI_PhyRegs, RegisterAddress, I2C_WRITE, Length);
        WdfRequestSetCompletionRoutine(pDevContext->I2CAsyncRequest, I2C_OnWriteCompletion, pDevContext);
        // Combine register address and user data to write into a single buffer.
        pDevContext->I2CAsyncBuffer[0] = RegisterAddress;
        RtlCopyMemory(&pDevContext->I2CAsyncBuffer[REGISTER_ADDR_SIZE], Data, Length);
        if (!NT_SUCCESS(ntStatus = WdfMemoryAssignBuffer(pDevContext->I2CMemory, static_cast<PVOID>(pDevContext->I2CAsyncBuffer), REGISTER_ADDR_SIZE + Length))) {
            DBG_PRINT_ERROR("WdfMemoryAssignBuffer failed with ntStatus 0x%08X", ntStatus);
        }
        if (!NT_SUCCESS(ntStatus = WdfIoTargetFormatRequestForWrite(pDevContext->hI2CIoTarget, pDevContext->I2CAsyncRequest, pDevContext->I2CMemory, NULL,  NULL))) {
            DBG_PRINT_ERROR("WdfIoTargetFormatRequestForWrite failed with ntStatus 0x%08X", ntStatus);
            break;
        }
        // Send the request to the I2C I/O Target.
        if (WdfRequestSend(pDevContext->I2CAsyncRequest, pDevContext->hI2CIoTarget, NULL) == FALSE) {
            ntStatus = WdfRequestGetStatus(pDevContext->I2CAsyncRequest);
            DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] WdfRequestSend for I2C write failed with ntStatus 0x%08X", pDevContext->I2CAsyncRequest, ntStatus);
            break;
        }
    } while (0);
    return ntStatus;
}
/*++
Routine Description:
    Synchronously reads data from the port controller's registers over the I2C controller for a request originating from the client driver.
Arguments:
    pDevContext   - Context information for the port controller device.
    RequestSource   - Identify the caller so the correct WDFREQUEST can be re-used
    RegisterAddress - The I2C register address from which to read data.
    Length          - Length of the data to read.
    Data            - The data read from the registers.
Return Value:  NTSTATUS
--*/
NTSTATUS I2C_ReadSynchronously(_In_ PDEVICE_CONTEXT pDevContext, _In_ I2C_REQUEST_SOURCE RequestSource, _In_ UINT8 RegisterAddress, _Out_writes_bytes_(Length) PVOID Data, _In_ ULONG Length) {
    NTSTATUS                 ntStatus;
    WDF_REQUEST_SEND_OPTIONS requestOptions;
    WDF_MEMORY_DESCRIPTOR    memoryDescriptor;
    WDF_REQUEST_REUSE_PARAMS reuseParams;
    ULONG_PTR                bytesTransferred = 0;
    UINT8                    transferBuffer[I2C_BUFFER_SIZE];
    TCPC_PHY_t               *pPhyRegs = &pDevContext->TCPI_PhyRegs;

    PAGED_CODE();
    // Static analysis cannot figure out the SPB_TRANSFER_LIST_ENTRY size but using an index variable quiets the warning.
    ULONG      index = 0;
    WDFREQUEST hRequest = pDevContext->OutgoingRequests[RequestSource];
    WDF_REQUEST_REUSE_PARAMS_INIT(&reuseParams, WDF_REQUEST_REUSE_NO_FLAGS, STATUS_SUCCESS);
    do {
        if (!NT_SUCCESS(ntStatus = WdfRequestReuse(hRequest, &reuseParams))) {
            DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] WdfRequestReuse for I2CSyncRequest failed with ntStatus 0x%08X", hRequest, ntStatus);
            break;
        }
        if (Length == 0) {
            DBG_PRINT_ERROR("Parameter 'Length' cannot be 0.");
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
        }
        if (Length > sizeof(transferBuffer)) {
            DBG_PRINT_ERROR("Unexpected value of data length. Length: %lu. Size of buffer: %lu", Length, sizeof(transferBuffer));
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
        }
        // Prepare the I2C transfer.
        SPB_TRANSFER_LIST_AND_ENTRIES(I2C_TRANSFER_COUNT) transferList;
        SPB_TRANSFER_LIST_INIT(&(transferList.List), I2C_TRANSFER_COUNT);
        // Static analysis can't figure out the relationship between the transfer array size and the transfer count.
        _Analysis_assume_(ARRAYSIZE(transferList.List.Transfers) == I2C_TRANSFER_COUNT);
        transferList.List.Transfers[index]     = SPB_TRANSFER_LIST_ENTRY_INIT_SIMPLE(SpbTransferDirectionToDevice,   0, &RegisterAddress, REGISTER_ADDR_SIZE);
        transferList.List.Transfers[index + 1] = SPB_TRANSFER_LIST_ENTRY_INIT_SIMPLE(SpbTransferDirectionFromDevice, 0, transferBuffer,   Length);
        // Initialize the memory descriptor with the transfer list.
        WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&memoryDescriptor, &transferList, sizeof(transferList));
        WDF_REQUEST_SEND_OPTIONS_INIT(&requestOptions, WDF_REQUEST_SEND_OPTION_TIMEOUT);
        requestOptions.Timeout = WDF_REL_TIMEOUT_IN_MS(I2C_SYNCHRONOUS_TIMEOUT);
        // Send the hRequest to the I2C I/O Target.
        if (!NT_SUCCESS(ntStatus = WdfIoTargetSendIoctlSynchronously(pDevContext->hI2CIoTarget, hRequest, IOCTL_SPB_EXECUTE_SEQUENCE, &memoryDescriptor, NULL, &requestOptions, &bytesTransferred))) {
            DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] WdfIoTargetSendIoctlSynchronously failed with ntStatus 0x%08X", hRequest, ntStatus);
            // A synchronous I2C hRequest failing is a good indicator that I2C is unresponsive. Attempt to reset the device.
            I2C_PerformDeviceReset(pDevContext);
            break;
        }
        if (bytesTransferred != REGISTER_ADDR_SIZE + Length) {
            DBG_PRINT_ERROR("Unexpected number of bytes transferred. Expected %lu, transferred %Iu.", REGISTER_ADDR_SIZE + Length, bytesTransferred);
            ntStatus = STATUS_INFO_LENGTH_MISMATCH;
            break;
        }        
        UpdateRegsInDevContext(pDevContext, RegisterAddress, transferBuffer, Length); /* Update registers values in device context */
        if (RegisterAddress == TCPC_PHY_CC_STATUS) { /* Bug fix */
            if ((pPhyRegs->CC_STATUS.B.LOOKING4CONNECTION || (pPhyRegs->POWER_CONTROL.B.ENABLE_VCONN & pPhyRegs->TCPC_CONTROL.B.PLUG_ORIENTATION)) & pPhyRegs->CC_STATUS.B.CC2_STATE) {
                 DBG_PRINT_ERROR("(L4C=1 || (VCON_EN=1 & PLUG_ORIENT=1) & CC2_STATE !=0, setting pPhyRegs->CC_STATUS.B.CC2_STATE 0x%02X", pPhyRegs->CC_STATUS.B);
                 pPhyRegs->CC_STATUS.B.CC2_STATE = 0;                                        
            }
            if ((pPhyRegs->CC_STATUS.B.LOOKING4CONNECTION || (pPhyRegs->POWER_CONTROL.B.ENABLE_VCONN & !pPhyRegs->TCPC_CONTROL.B.PLUG_ORIENTATION)) & pPhyRegs->CC_STATUS.B.CC1_STATE) {
                DBG_PRINT_ERROR("(L4C=1 || (VCON_EN=1 & PLUG_ORIENT=0) & CC1_STATE !=0, setting pPhyRegs->CC_STATUS.B.CC1_STATE 0x%02X",pPhyRegs->CC_STATUS.B);
                pPhyRegs->CC_STATUS.B.CC2_STATE = 0;                                        
            }
            *(UINT8*)transferBuffer = pPhyRegs->CC_STATUS.R;
            UpdateRegsInDevContext(pDevContext, RegisterAddress, transferBuffer, Length); /* Update registers values in device context */
        }
        TCPC_PHY_DumpReg(&pDevContext->TCPI_PhyRegs, RegisterAddress, I2C_READ, Length);
        RtlCopyMemory(Data, transferBuffer, Length);    // Get the returned data out of the buffer.
    } while (0);
    return ntStatus;
}

/*++
Routine Description:
    Synchronously reads data from the port controller's registers over the I2C controller for a request originating from the client driver.
Arguments:
    pDevContext - Context information for the port controller device.
    RequestSource - Identify the caller so the correct WDFREQUEST can be re-used
    Items         - Array of (register, data, length) triplets
    Count         - Count of elements from the Items array above
Return Value:  NTSTATUS
--*/
NTSTATUS I2C_ReadSynchronouslyMultiple(_In_ PDEVICE_CONTEXT pDevContext, _In_ I2C_REQUEST_SOURCE requestSource, _Inout_updates_(Count) REGISTER_ITEM* Items, _In_ ULONG Count) {
    NTSTATUS       ntStatus = STATUS_SUCCESS;
    PREGISTER_ITEM item;

    PAGED_CODE();
    for (ULONG i = 0; i < Count; i++) {
        item = &Items[i];
        if (!NT_SUCCESS(ntStatus = I2C_ReadSynchronously(pDevContext, requestSource, item->RegisterAddress, item->Data, item->Length))) {
            break;
        }
    }
    return ntStatus;
}

/*++
Routine Description:
    Synchronously writes data from the port controller's registers over the I2C controller for a request originating from the client driver.
Arguments:
    pDevContext   - Context information for the port controller device.
    RequestSource   - Identify the caller so the correct WDFREQUEST can be re-used
    RegisterAddress - The I2C register address from which to write data.
    Data            - The data to write.
    Length          - Length of the data to write.
Return Value:  NTSTATUS
--*/
NTSTATUS I2C_WriteSynchronously(_In_ PDEVICE_CONTEXT pDevContext, _In_ I2C_REQUEST_SOURCE RequestSource, _In_ UINT8 RegisterAddress, _In_reads_(Length) PVOID Data, _In_ ULONG Length) {    
    NTSTATUS                 ntStatus;
    WDF_REQUEST_SEND_OPTIONS requestOptions;
    WDF_MEMORY_DESCRIPTOR    memoryDescriptor;
    WDF_REQUEST_REUSE_PARAMS reuseParams;
    ULONG_PTR                bytesTransferred = 0;
    UINT8                    transferBuffer[I2C_BUFFER_SIZE];

    PAGED_CODE();
    WDFREQUEST request = pDevContext->OutgoingRequests[RequestSource];
    do {
        if (Length == 0) {
            DBG_PRINT_ERROR("Parameter 'Length' cannot be 0.");
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
        }
        // Reuse the preallocated WDFREQUEST for internal requests.
        WDF_REQUEST_REUSE_PARAMS_INIT(&reuseParams, WDF_REQUEST_REUSE_NO_FLAGS, STATUS_SUCCESS);
        if (!NT_SUCCESS(ntStatus = WdfRequestReuse(request, &reuseParams))) {
            DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] WdfRequestReuse failed with ntStatus 0x%08X", request, ntStatus);
            break;
        }
        transferBuffer[0] = RegisterAddress;
        if (REGISTER_ADDR_SIZE + Length > sizeof(transferBuffer)) {
            DBG_PRINT_ERROR("Unexpected value of length. Length: %lu. Size of buffer: %lu", Length, sizeof(transferBuffer));
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
        }
        UpdateRegsInDevContext(pDevContext, RegisterAddress, Data, Length); /* Update registers values in device context */
        TCPC_PHY_DumpReg(&pDevContext->TCPI_PhyRegs, RegisterAddress, I2C_WRITE, Length);
        RtlCopyMemory(&transferBuffer[REGISTER_ADDR_SIZE], Data, Length);
        // Initialize the memory descriptor with the write buffer.
        WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&memoryDescriptor, &transferBuffer, REGISTER_ADDR_SIZE + Length);
        WDF_REQUEST_SEND_OPTIONS_INIT(&requestOptions, WDF_REQUEST_SEND_OPTION_TIMEOUT);
        requestOptions.Timeout = WDF_REL_TIMEOUT_IN_MS(I2C_SYNCHRONOUS_TIMEOUT);
        // Send the write request to the I2C I/O Target.
        if (!NT_SUCCESS(ntStatus = WdfIoTargetSendWriteSynchronously(pDevContext->hI2CIoTarget, request, &memoryDescriptor, NULL, &requestOptions, &bytesTransferred))) {
            DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] WdfIoTargetSendWriteSynchronously failed with ntStatus 0x%08X", request, ntStatus);
            // A synchronous I2C request failing is a good indicator that I2C is unresponsive. Attempt to reset the device.
            I2C_PerformDeviceReset(pDevContext);
            break;
        }
        if (bytesTransferred != REGISTER_ADDR_SIZE + Length) {
            DBG_PRINT_ERROR("Unexpected number of bytes transferred. Expected %lu, transferred %Iu.", REGISTER_ADDR_SIZE + Length, bytesTransferred);
            ntStatus = STATUS_INFO_LENGTH_MISMATCH;
            break;
        }
    } while (0);
    return ntStatus;
}

/*++
Routine Description:
    Completion routine for hardware access after a write. Completes the WDFREQUEST from UcmTcpciCx.
Arguments:
    Request - A handle to a framework request object that represents the completed I/O request.
    Target  - A handle to an I/O target object that represents the I/O target that completed the request.
    Params  - A pointer to a WDF_REQUEST_COMPLETION_PARAMS structure that contains information about the completed request.
    Context - Driver-supplied context information, which the driver specified in a previous call to WdfRequestSetCompletionRoutine. In this case, it is of type PDEVICE_CONTEXT.
--*/
VOID I2C_OnWriteCompletion(_In_ WDFREQUEST Request, _In_ WDFIOTARGET Target, _In_ PWDF_REQUEST_COMPLETION_PARAMS Params, _In_ WDFCONTEXT Context) {
    NTSTATUS        ntStatus;
    PDEVICE_CONTEXT pDevContext;

    UNREFERENCED_PARAMETER(Target);
    UNREFERENCED_PARAMETER(Request);
    pDevContext = (PDEVICE_CONTEXT)Context;
    if (!NT_SUCCESS(ntStatus = Params->IoStatus.Status)) {
        DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] I2C_OnWriteCompletion - WDFREQUEST completed with failure ntStatus: 0x%08X", Request, ntStatus);
    }
    // Complete the WDFREQUEST from UcmTcpciCx.
    WdfRequestComplete(pDevContext->IncomingRequest, ntStatus);
}

/*++
Routine Description:
    Recovery mechanism for a malfunctioning I2C bus. Attempt a platform-level device reset.
    If unsuccessful or we have exceeded the maximum number of reset attempts, call WdfDeviceSetFailed.
Arguments:
    pDevContext - Context information for the port controller device.
--*/
void I2C_PerformDeviceReset(_In_ PDEVICE_CONTEXT pDevContext) {
    PAGED_CODE();
    NTSTATUS ntStatus;
    if (pDevContext->ResetInterface.DeviceReset != NULL && pDevContext->ResetAttempts <= MAX_DEVICE_RESET_ATTEMPTS) {
        // Attempt a platform-level device reset (PLDR) to recover from an I2C error. This will disconnect the device from the power rail and reconnect it.
        ++pDevContext->ResetAttempts;
        if (!NT_SUCCESS(ntStatus = pDevContext->ResetInterface.DeviceReset(pDevContext->ResetInterface.Context, PlatformLevelDeviceReset, 0, NULL))) {
            DBG_PRINT_ERROR("[WDFDEVICE: 0x%p] PlatformLevelDeviceReset failed with ntStatus: 0x%08X", pDevContext->Device, ntStatus);
            // If PLDR fails, perform WdfDeviceSetFailed.
            WdfDeviceSetFailed(pDevContext->Device, WdfDeviceFailedAttemptRestart);
        }
    } else {
        // Either platform-level device reset failed or DEVICE_RESET_INTERFACE_STANDARD was not supported by the bus driver. Use WdfDeviceSetFailed and attempt to restart the device.
        // When the driver is reloaded, it will reinitialize I2C.
        // If several consecutive restart attempts fail (because the restarted driver again reports an error), the framework stops trying to restart the device.
        WdfDeviceSetFailed(pDevContext->Device, WdfDeviceFailedAttemptRestart);
    }
}
