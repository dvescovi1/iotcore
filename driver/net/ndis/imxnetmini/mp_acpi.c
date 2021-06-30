/*
* Copyright 2019 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the disclaimer
* below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* * Neither the name of NXP nor the names of its contributors may be used to
* endorse or promote products derived from this software without specific prior
* written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS
* LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "precomp.h"

// {4EBBCB9D-4087-4791-80BD-8F8C08024206}
DEFINE_GUID(GUID_IMX_ENET_DSM,               0x4ebbcb9d, 0x4087, 0x4791, 0x80, 0xbd, 0x8f, 0x8c, 0x08, 0x02, 0x42, 0x06);
DEFINE_GUID(ACPI_DEVICE_PROPERTIES_DSD_GUID, 0xDAFFD814, 0x6EBA, 0x4D8C, 0x8A, 0x91, 0xBC, 0x9B, 0xBF, 0x4A, 0xA3, 0x01);


#define ACPI_DSM_STRING ((ULONG)'MSD_')
#define ACPI_DSD_STRING ((ULONG)'DSD_')

// _DSM Execution Timeout (in 100-ns units)
#define IMX_ENET_DSM_EXECUTION_TIMEOUT 30000    // 3 seconds

EXTERN_C_START

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS Acpi_EvaluateDsm(_In_ PMP_ADAPTER pAdapter, _In_ ULONG FunctionIndex, _In_opt_ ULONG OutputLength, _Outptr_opt_ PACPI_EVAL_OUTPUT_BUFFER* Output);

EXTERN_C_END

#pragma alloc_text(PAGE, Acpi_Init)
#pragma alloc_text(PAGE, Acpi_EvaluateDsm)
#pragma alloc_text(PAGE, Acpi_GetValue)

/*++
Routine Description:
    Initialises Acpi part of the driver.
Arguments:
    pAdapter                  - ENET device data structure address.
    pACPI_SupportedFunctions  - Address of variable to fill with "Supported functions" bitmask.
Return Value:
    STATUS_ACPI_INVALID_DATA or NDIS_STATUS_SUCCESS
--*/
_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS Acpi_Init(_In_ PMP_ADAPTER pAdapter, _Out_ ULONG *pACPI_SupportedFunctions)
{
    NTSTATUS                 Status;
    PACPI_EVAL_OUTPUT_BUFFER output;

    PAGED_CODE();
    DBG_ACPI_METHOD_BEG();
    NdisMGetDeviceProperty(pAdapter->AdapterHandle, &pAdapter->Pdo, &pAdapter->Fdo, &pAdapter->NextDeviceObject, NULL, NULL);
    output = NULL;
    *pACPI_SupportedFunctions = 0;
    do {
        Status = Acpi_EvaluateDsm(pAdapter, IMX_ENET_DSM_FUNCTION_SUPPORTED_FUNCTIONS_INDEX, sizeof(ULONG), &output);
        if (!NT_SUCCESS(Status)) {
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("Acpi_EvaluateDsm(SUPPORTED_FUNCTION) failed.");
            break;
        }
        if (output->Count != 1) {
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS_AND_PARAMS("Acpi_EvaluateDsm(SUPPORTED_FUNCTION) failed.", "Functions returned wrong number of args %lu", output->Count);
            break;
        }
        if (output->Argument[0].Type != ACPI_METHOD_ARGUMENT_BUFFER) {
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS_AND_PARAMS("Acpi_EvaluateDsm(SUPPORTED_FUNCTION) failed.", "Functions returned unexpected argtype %d", output->Argument[0].Type);
            break;
        }
        if ((ULONG)(output->Argument[0].Data[0]) & 1) {                               // If bit 0 is set
            *pACPI_SupportedFunctions = (ULONG)(output->Argument[0].Data[0]);         // then return bit mask contains valid data.
        } else {                                                                      // If bit 0 is not set
            *pACPI_SupportedFunctions = 0;                                            // then no functions should be invoked.
        }
    } while (0);
    if (output) {
        ExFreePoolWithTag(output, MP_TAG_ACPI);
    }
    DBG_ACPI_METHOD_END_WITH_STATUS(Status);
    return Status;
}

/*++
Routine Description:
    Reads data returned by specified ACPI function to the user provided buffer.
    Note: Caller is expected to free the Output buffer.
Arguments:
    pAdapter     - ENET device data structure address.
    FnIndex      - ACPI function index.
    Output       - Address of user provided buffer.
    OutputLength - Buffer size.
Return Value:
    NDIS_STATUS_SUCCESS or STATUS_INSUFFICIENT_RESOURCES
--*/
_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS Acpi_EvaluateDsm(_In_ PMP_ADAPTER pAdapter, _In_ ULONG FunctionIndex, _In_opt_ ULONG OutputLength, _Outptr_opt_ PACPI_EVAL_OUTPUT_BUFFER* Output)
{
    PACPI_EVAL_INPUT_BUFFER_COMPLEX  inputBuffer;
    ULONG                            inputBufferSize;
    ULONG                            inputArgumentBufferSize;
    PACPI_METHOD_ARGUMENT            argument;
    PACPI_EVAL_OUTPUT_BUFFER         outputBuffer;
    ULONG                            outputBufferSize;
    ULONG                            outputArgumentBufferSize;
    NTSTATUS                         Status = STATUS_SUCCESS;
    PIRP                             irp = NULL;
    IO_STATUS_BLOCK                  ioStatus = { 0 };
    KEVENT                           event;
    LARGE_INTEGER                    timeout;
    ULONG                            outputDataLength;
    PDEVICE_OBJECT                   Device = pAdapter->Pdo;

    PAGED_CODE();
    KeInitializeEvent(&event, NotificationEvent, FALSE);
    inputBuffer = NULL;
    outputBuffer = NULL;
    inputArgumentBufferSize = ACPI_METHOD_ARGUMENT_LENGTH(sizeof(GUID)) +
                              ACPI_METHOD_ARGUMENT_LENGTH(sizeof(ULONG)) +
                              ACPI_METHOD_ARGUMENT_LENGTH(sizeof(ULONG)) +
                              ACPI_METHOD_ARGUMENT_LENGTH(0);
    do {
        inputBufferSize = FIELD_OFFSET(ACPI_EVAL_INPUT_BUFFER_COMPLEX, Argument) + inputArgumentBufferSize;
        if ((inputBuffer = (PACPI_EVAL_INPUT_BUFFER_COMPLEX)ExAllocatePoolWithTag(NonPagedPoolNx, inputBufferSize, MP_TAG_ACPI)) == NULL) {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS_AND_PARAMS("ExAllocatePoolWithTag(Input) failed.", "Requested bytes: %Iu", inputBufferSize);
            break;
        }
        RtlZeroMemory(inputBuffer, inputBufferSize);
        inputBuffer->Signature = ACPI_EVAL_INPUT_BUFFER_COMPLEX_SIGNATURE;
        inputBuffer->Size = inputArgumentBufferSize;
        inputBuffer->ArgumentCount = 4;
        inputBuffer->MethodNameAsUlong = ACPI_DSM_STRING;

        // Device Specific Method takes 4 args:
        argument = &(inputBuffer->Argument[0]);
        ACPI_METHOD_SET_ARGUMENT_BUFFER(argument, &GUID_IMX_ENET_DSM, sizeof(GUID_IMX_ENET_DSM));   // Arg0: Buffer containing a UUID[16 bytes]
        argument = ACPI_METHOD_NEXT_ARGUMENT(argument);
        ACPI_METHOD_SET_ARGUMENT_INTEGER(argument, IMX_ENET_DSM_REVISION);                          //  Arg1 : Integer containing the Revision ID
        argument = ACPI_METHOD_NEXT_ARGUMENT(argument);
        ACPI_METHOD_SET_ARGUMENT_INTEGER(argument, FunctionIndex);                                  //  Arg2 : Integer containing the Function Index
        argument = ACPI_METHOD_NEXT_ARGUMENT(argument);
        argument->Type = ACPI_METHOD_ARGUMENT_PACKAGE_EX;                                           //  Arg3 : Package that contains function-specific arguments
        argument->DataLength = 0;

        if (OutputLength) {
            outputDataLength = OutputLength;
        } else {
            outputDataLength = sizeof(ULONG);
        }
        outputArgumentBufferSize = ACPI_METHOD_ARGUMENT_LENGTH(outputDataLength);
        outputBufferSize = FIELD_OFFSET(ACPI_EVAL_OUTPUT_BUFFER, Argument) + outputArgumentBufferSize;
        if ((outputBuffer = (PACPI_EVAL_OUTPUT_BUFFER)ExAllocatePoolWithTag(NonPagedPoolNx, outputBufferSize, MP_TAG_ACPI)) == NULL) {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS_AND_PARAMS("ExAllocatePoolWithTag(Output) failed.", "Requested bytes: %Iu", outputBufferSize);
            break;
        }
        RtlZeroMemory(outputBuffer, outputBufferSize);
        if ((irp = IoBuildDeviceIoControlRequest(IOCTL_ACPI_EVAL_METHOD, Device, inputBuffer, inputBufferSize, outputBuffer, outputBufferSize,FALSE, &event, &ioStatus)) == NULL) {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("IoBuildDeviceIoControlRequest() failed.");
            break;
        }
        if ((Status = IoCallDriver(Device, irp)) == STATUS_PENDING) {
            timeout.QuadPart = IMX_ENET_DSM_EXECUTION_TIMEOUT;
            KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, &timeout);
            Status = ioStatus.Status;
        }
        if (!NT_SUCCESS(Status)) {
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("IOCTL_ACPI_EVAL_METHOD for _DSM failed.");
            break;
        }
        if (outputBuffer->Signature != ACPI_EVAL_OUTPUT_BUFFER_SIGNATURE) {
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("ACPI_EVAL_OUTPUT_BUFFER signature is incorrect.");
            break;
        }
    } while (0);
    if (inputBuffer) {
        ExFreePoolWithTag(inputBuffer, MP_TAG_ACPI);
    }
    if (!NT_SUCCESS(Status) || (Output == NULL)) {
        if (outputBuffer) {
            ExFreePoolWithTag(outputBuffer, MP_TAG_ACPI);
        }
    } else {
        *Output = outputBuffer;
    }
    return Status;
}

/*++
Routine Description:
    Copies data returned by specified ACPI function to the user provided buffer.
Arguments:
    pAdapter   - ENET device data structure address.
    FnIndex    - ACPI function index.
    pBuffer    - Address of user provided buffer.
    BufferSize - Buffer size.
Return Value:
    NDIS_STATUS_SUCCESS, STATUS_ACPI_INVALID_DATA or STATUS_INSUFFICIENT_RESOURCES
--*/
_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS Acpi_GetValue(_In_ PMP_ADAPTER pAdapter, _In_ ULONG FnIndex, _Out_ PVOID pBuffer, _In_ ULONG BufferSize)
{
    NTSTATUS                 Status;
    PACPI_EVAL_OUTPUT_BUFFER output = NULL;

    PAGED_CODE();
    DBG_ACPI_METHOD_BEG_WITH_PARAMS("Function: %s", Dbg_GetACPIFunctionName(FnIndex));
    do {
        if ((pAdapter->ENETDev_bmACPISupportedFunctions & (1 << FnIndex)) == 0) {
            Status = STATUS_NOT_IMPLEMENTED;
            DBG_ACPI_PRINT_INFO("Function %s is not supported in ACPI.", Dbg_GetACPIFunctionName(FnIndex));
            break;
        }
        if (!NT_SUCCESS(Status = Acpi_EvaluateDsm(pAdapter, FnIndex, BufferSize, &output))) {
            DBG_ACPI_PRINT_ERROR_WITH_STATUS_AND_PARAMS("Acpi_EvaluateDsm(%s) failed.","", Dbg_GetACPIFunctionName(FnIndex));
            break;
        }
        if (output->Count != 1) {                              // Check count
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS_AND_PARAMS("Acpi_EvaluateDsm(%s) failed.", "Returned wrong number of args %lu, expected only one argument", Dbg_GetACPIFunctionName(FnIndex), output->Count);
            break;
        }
        if (output->Argument[0].DataLength != BufferSize) {    // Check length
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS_AND_PARAMS("Acpi_EvaluateDsm(%s) failed.", "incorrect data length %lu, expected %lu bytes", Dbg_GetACPIFunctionName(FnIndex), output->Argument[0].DataLength, BufferSize);
            break;
        }
        RtlCopyMemory(pBuffer, &output->Argument[0].Data, BufferSize);
        Status = STATUS_SUCCESS;
    } while(0);
    if (output) {
        ExFreePoolWithTag(output, MP_TAG_ACPI);
    }
    DBG_ACPI_METHOD_END_WITH_STATUS(Status);
    return Status;
}

/*++
Routine Description:
    Returns next argumet of the output buffer.
Arguments:
    pOutputBuffer     - Pointer to the ouptut buffer.
    ppArgument        - Pointer to pointer that will contain next argument in the output buffer.
    ArgumentType      - Argunent type.
Return Value:
    NDIS_STATUS_SUCCESS, STATUS_NO_MORE_ENTRIES or STATUS_ACPI_INVALID_ARGTYPE
--*/
_Use_decl_annotations_
NTSTATUS AcpiOuputBuffer_GetNextArgument(ACPI_EVAL_OUTPUT_BUFFER UNALIGNED* pOutputBuffer, ACPI_METHOD_ARGUMENT UNALIGNED **ppArgument, USHORT ArgumentType) 
{ 
    ACPI_METHOD_ARGUMENT UNALIGNED *pNewArgument;

    // A null value indicates start enumeration from first argument
    if (*ppArgument == NULL) {
        pNewArgument = pOutputBuffer->Argument;
    } else {
        pNewArgument = ACPI_METHOD_NEXT_ARGUMENT(*ppArgument);
    }
    // Check end of buffer
    if ((pNewArgument >= ACPI_EVAL_OUTPUT_BUFFER_ARGUMENTS_END(pOutputBuffer))) {
        return STATUS_NO_MORE_ENTRIES;
    }
    if (pNewArgument->Type != ArgumentType) {
        return STATUS_ACPI_INVALID_ARGTYPE;
    }
    *ppArgument = pNewArgument;
    return STATUS_SUCCESS;        
}

/*++
Routine Description:
    Returns next argumet of the package.
Arguments:
    pOutputBuffer     - Pointer to the package.
    ppArgument        - Pointer to pointer that will contain next argument in the package.
    ArgumentType      - Argunent type.
Return Value:
    NDIS_STATUS_SUCCESS, STATUS_NO_MORE_ENTRIES or STATUS_ACPI_INVALID_ARGTYPE
--*/
_Use_decl_annotations_
NTSTATUS AcpiPackage_GetNextArgument(ACPI_METHOD_ARGUMENT UNALIGNED* pPackage, ACPI_METHOD_ARGUMENT UNALIGNED **ppArgument, USHORT ArgumentType) 
{ 
    ACPI_METHOD_ARGUMENT UNALIGNED *pNewArgument;

    // Package is either empty or incomplete
    if (pPackage->DataLength < sizeof(ACPI_METHOD_ARGUMENT)) {
        return STATUS_NO_MORE_ENTRIES;
    }
    // A null value indicates start enumeration from first argument
    if (*ppArgument == NULL) {
        pNewArgument = (PACPI_METHOD_ARGUMENT)&(pPackage->Data);
    } else {
        pNewArgument =  ACPI_METHOD_NEXT_ARGUMENT(*ppArgument);
    }
    // Check end of package
    if (pNewArgument >= (PACPI_METHOD_ARGUMENT)(pPackage->Data + pPackage->DataLength)) { 
        return STATUS_NO_MORE_ENTRIES;
    }
    if (pNewArgument->Type != ArgumentType) {
        return STATUS_ACPI_INVALID_ARGTYPE;
    }
    *ppArgument = pNewArgument;
    return STATUS_SUCCESS;        
}

/*++
Routine Description:
    Allocates the output buffer and finds the "Property package" in _DSD.
    User must dealocate output buffer as soon as it is not needed.
Arguments:
    pAdapter   - Pointer to the ENET device data structure.
Return Value:
    NDIS_STATUS_SUCCESS or STATUS_ACPI_INVALID_ARGTYPE
--*/
_Use_decl_annotations_
NTSTATUS AcpiDevice_GetDevicePropertiesPackage(PMP_ADAPTER pAdapter) {
    NTSTATUS                              Status;
    PIRP                                  pIrp = NULL;
    IO_STATUS_BLOCK                       IoStatus = { 0 };
    KEVENT                                Event;
    LARGE_INTEGER                         Timeout;
    ACPI_EVAL_OUTPUT_BUFFER              *pOutputBuffer;
    ACPI_EVAL_INPUT_BUFFER                InputBuffer;
    int                                   retries = 2;
    ULONG                                 OutputBufferLength = sizeof(ACPI_EVAL_OUTPUT_BUFFER);
    ACPI_METHOD_ARGUMENT UNALIGNED       *pArgument = NULL;

    DBG_ACPI_METHOD_BEG();
    do {
        // Get _DSD from ACPI (alocate buffer for data)
        do {
            KeInitializeEvent(&Event, NotificationEvent, FALSE);
            if ((pOutputBuffer = (ACPI_EVAL_OUTPUT_BUFFER *)(ExAllocatePoolWithTag(NonPagedPoolNx, OutputBufferLength, MP_TAG_ACPI))) == NULL) {
                Status = STATUS_INSUFFICIENT_RESOURCES;
                break;
            }
            RtlZeroMemory(pOutputBuffer, OutputBufferLength);
            InputBuffer.Signature         = ACPI_EVAL_INPUT_BUFFER_SIGNATURE;
            InputBuffer.MethodNameAsUlong = ACPI_DSD_STRING;
            if ((pIrp = IoBuildDeviceIoControlRequest(IOCTL_ACPI_EVAL_METHOD, pAdapter->Pdo, &InputBuffer, sizeof(ACPI_EVAL_INPUT_BUFFER), pOutputBuffer, OutputBufferLength, FALSE, &Event, &IoStatus)) == NULL) {
                Status = STATUS_INSUFFICIENT_RESOURCES;
                DBG_ACPI_PRINT_ERROR_WITH_STATUS("IoBuildDeviceIoControlRequest() failed.");
                break;
            }
            if ((Status = IoCallDriver(pAdapter->Pdo, pIrp)) == STATUS_PENDING) {
                Timeout.QuadPart = IMX_ENET_DSM_EXECUTION_TIMEOUT;
                KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, &Timeout);
                Status = IoStatus.Status;
            }
            if (Status == STATUS_BUFFER_OVERFLOW) {
                ExFreePoolWithTag(pOutputBuffer, MP_TAG_ACPI);
                OutputBufferLength = pOutputBuffer->Length;
                pOutputBuffer = NULL;
            }
            retries--;
        } while ((Status == STATUS_BUFFER_OVERFLOW) && (retries > 0));
        if (!NT_SUCCESS(Status)) {
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("IOCTL_ACPI_EVAL_METHOD for _DSD failed.");
            break;
        }
        // Check output buffer
        if (pOutputBuffer->Signature != ACPI_EVAL_OUTPUT_BUFFER_SIGNATURE) {
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("pOutputBuffer->Signature != ACPI_EVAL_OUTPUT_BUFFER_SIGNATURE.");
            break;
        }
        if (pOutputBuffer->Count != 2) {
            Status = STATUS_ACPI_INCORRECT_ARGUMENT_COUNT;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("pOutputBuffer->Count != 2.");
        }
        // First argument of the _DSD must be a GUID
        if (!NT_SUCCESS(Status = AcpiOuputBuffer_GetNextArgument(pOutputBuffer, &pArgument, ACPI_METHOD_ARGUMENT_BUFFER))) {
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("GUID argument is missing or its type is not BUFFER.");
            break;
        }
        if (pArgument->DataLength !=  sizeof(GUID)) {
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("GUID argument size is not valid.");
            break;                
        }
        // Check GUID value
        if (!(((unsigned long *)&pArgument->Data)[0] == ((unsigned long *) &ACPI_DEVICE_PROPERTIES_DSD_GUID)[0] &&
              ((unsigned long *)&pArgument->Data)[1] == ((unsigned long *) &ACPI_DEVICE_PROPERTIES_DSD_GUID)[1] &&
              ((unsigned long *)&pArgument->Data)[2] == ((unsigned long *) &ACPI_DEVICE_PROPERTIES_DSD_GUID)[2] &&
              ((unsigned long *)&pArgument->Data)[3] == ((unsigned long *) &ACPI_DEVICE_PROPERTIES_DSD_GUID)[3])) {
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("GUID argument value is not ACPI_DEVICE_PROPERTIES_DSD_GUID.");
            break;
        }        
        // Next argument must be Package of two items packages - Package () { Package(2) {"Property 1 name", Property 1 value}, {"Property 2 name", Property 2 value} }
        if (!NT_SUCCESS(Status = AcpiOuputBuffer_GetNextArgument(pOutputBuffer, &pArgument, ACPI_METHOD_ARGUMENT_PACKAGE))) {
            DBG_ACPI_PRINT_ERROR_WITH_STATUS("No PACKAGE argument found after GUID argument.");
            break;
        }
    } while (0);
    if (NT_SUCCESS(Status)) {
        pAdapter->pOutputBuffer = pOutputBuffer;
        pAdapter->pPropertiesPackage = pArgument;
    } else {
        if (pOutputBuffer != NULL) {
            ExFreePoolWithTag(pOutputBuffer, MP_TAG_ACPI);
        }
    }
    DBG_ACPI_METHOD_END_WITH_STATUS(Status);
    return Status;
}

/*++
Routine Description:
    Returns the property value argument.
Arguments:
    pAdapter                 - Pointer to the ENET device data structure.
    pPropertName             - Property name.
    ppPropertyValueArgument  - Pointer to pointer to the requested peroperty value argument.
    ArgumentType             - Argunent type.
Return Value:
    NDIS_STATUS_SUCCESS, STATUS_NO_MORE_ENTRIES or STATUS_ACPI_INVALID_ARGTYPE
--*/
_Use_decl_annotations_
NTSTATUS AcpiDevice_GetDevicePropertyValueArgument(PMP_ADAPTER pAdapter, const CHAR* pPropertName, ACPI_METHOD_ARGUMENT UNALIGNED **ppPropertyValueArgument, USHORT ArgumentType)
 {
    NTSTATUS                        Status;
    ACPI_METHOD_ARGUMENT UNALIGNED *pCurrentPropertyPackage = NULL;
    ANSI_STRING                     sKeyName;
    __analysis_assume_nullterminated(pPropertName);

    *ppPropertyValueArgument = NULL;
    if (!NT_SUCCESS(Status = RtlInitAnsiStringEx(&sKeyName, pPropertName))) {
        return Status;
    }
    // Example Device Properties
    // Package () {
    //   Package (2) { "Key1", Value1 },
    //   Package (2) { "Key2", Package () {Value2, Value3, ..} },
    //   Package (2) { "Key3", ValueX }
    // }
    for (;;) {
        // Each element in the Device Properties package is a non-empty package key/value pair
        if (!NT_SUCCESS(Status = AcpiPackage_GetNextArgument(pAdapter->pPropertiesPackage, &pCurrentPropertyPackage, ACPI_METHOD_ARGUMENT_PACKAGE))) {
            return Status;
        }        
        ACPI_METHOD_ARGUMENT UNALIGNED* pPropertyName = NULL;
        // Key should be string
        if (!NT_SUCCESS(Status = AcpiPackage_GetNextArgument(pCurrentPropertyPackage, &pPropertyName, ACPI_METHOD_ARGUMENT_STRING))) {
            return Status;
        }
        ANSI_STRING currentKeyNameStr;
        _Analysis_assume_nullterminated_(pPropertyName->Data);
        RtlInitAnsiStringEx(&currentKeyNameStr, (PCSZ)(pPropertyName->Data));
        if (!RtlEqualString(&currentKeyNameStr, &sKeyName, FALSE)) {
            continue;
        }
        if (!NT_SUCCESS(Status = AcpiPackage_GetNextArgument(pCurrentPropertyPackage, &pPropertyName, ArgumentType))) {
            return Status;
        }
        *ppPropertyValueArgument = pPropertyName;
        break;
    }
    return STATUS_SUCCESS;
}
    
/*++
Routine Description:
    Returns the integer property value.
Arguments:
    pAdapter       - Pointer to the ENET device data structure.
    pPropertName   - Property name.
    pBuffer        - Pointer to pointer to the data buffer.
Return Value:
    NDIS_STATUS_SUCCESS, STATUS_NO_MORE_ENTRIES or STATUS_ACPI_INVALID_ARGTYPE
    --*/
_Use_decl_annotations_
void AcpiDevice_GetIntegerPropertyValue(PMP_ADAPTER pAdapter, const CHAR* pPropertName, ULONG *pBuffer) 
{
    NTSTATUS                        Status;
    ACPI_METHOD_ARGUMENT UNALIGNED *pPropertyValue;

    if (!NT_SUCCESS(Status = AcpiDevice_GetDevicePropertyValueArgument(pAdapter, pPropertName, &pPropertyValue, ACPI_METHOD_ARGUMENT_INTEGER))) {
        DBG_ACPI_PRINT_INFO("Propery %s not found", pPropertName);
    } else {
        *pBuffer = pPropertyValue->Argument;
    }
    return;
}

/*++
Routine Description:
    Alloactes buffer for ENET_PHY_CMD and fills it with data from given proeprty.
Arguments:
    pAdapter       - Pointer to the ENET device data structure.
    pPropertName   - Property name.
    ppCmd          - Pointer to pointer to the ENET_PHY_CMD buffer. User mus free this buffer.
Return Value:
    NDIS_STATUS_SUCCESS, STATUS_NO_MORE_ENTRIES or STATUS_ACPI_INVALID_ARGTYPE
    --*/
_Use_decl_annotations_

void AcpiDevice_GetCommad(PMP_ADAPTER pAdapter, const CHAR* pPropertName, ENET_PHY_CMD **ppCmd)
 {
    NTSTATUS                         Status;
    ACPI_METHOD_ARGUMENT UNALIGNED  *pCurrentPackage;
    ENET_PHY_CMD                    *pTmpCmd;

    *ppCmd = NULL;
    if (!NT_SUCCESS(Status = AcpiDevice_GetDevicePropertyValueArgument(pAdapter, pPropertName, &pCurrentPackage, ACPI_METHOD_ARGUMENT_PACKAGE))) {
        DBG_ACPI_PRINT_INFO("Propery %s not found", pPropertName);
    } else {
        UINT32 CmdCount = pCurrentPackage->DataLength / sizeof(ACPI_METHOD_ARGUMENT);
        if (CmdCount > 1) {
            if ((pTmpCmd = (ENET_PHY_CMD *)(ExAllocatePoolWithTag(NonPagedPoolNx, (CmdCount + 1)* sizeof(ENET_PHY_CMD), MP_TAG_ACPI))) == NULL) {
                return;
            }
            *ppCmd = pTmpCmd;
            ACPI_METHOD_ARGUMENT UNALIGNED  *pPropertyValue = NULL;
            while (CmdCount) {
                if (!NT_SUCCESS(Status = AcpiPackage_GetNextArgument(pCurrentPackage, &pPropertyValue, ACPI_METHOD_ARGUMENT_INTEGER))) {
                    break;
                }
                pTmpCmd->MIIData = (UINT32)pPropertyValue->Argument;
                pTmpCmd->MIIFunct = NULL;
                pTmpCmd++;
                CmdCount--;
            }
            pTmpCmd->MIIData = 0;
        }
    }
    return;
}
