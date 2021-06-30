/*
* This file is derived from Microsoft example available on https://github.com/microsoft/Windows-driver-samples/blob/master/usb/UcmTcpciCxClientSample
* Original file licensing:
* Copyright (c) 2015 Microsoft
* SPDX-License-Identifier: MS-PL
* NXP modifications are licensed under the same license
* Copyright 2020 NXP
*
*/

#pragma once

// Number of platform-level device resets to attempt.
#define MAX_DEVICE_RESET_ATTEMPTS 3

// Transfers required for an I2C read or write operation.
// The first transfer in the sequence writes a one-byte register address to the device.
// The second transfer reads from or writes to the selected register.
#define I2C_TRANSFER_COUNT 2

#define REGISTER_ADDR_SIZE 1

// Timeout in milliseconds for synchronous I2C reads/writes.
// The I2C specification does not specify a timeout. 300 ms was chosen arbitrarily.
#define I2C_SYNCHRONOUS_TIMEOUT 300

// Size used to initialize the I2C read and write buffers.
#define I2C_BUFFER_SIZE 50

typedef struct _DEVICE_CONTEXT DEVICE_CONTEXT, *PDEVICE_CONTEXT;

enum I2C_REQUEST_SOURCE;

typedef struct _REGISTER_ITEM {
    UINT8                             RegisterAddress;
    _Out_writes_bytes_(Length) PVOID  Data;
    ULONG                             Length;
} REGISTER_ITEM, *PREGISTER_ITEM;

// Helper macro to generate an array item.
#define GEN_REGISTER_ITEM(RegisterAddress, Variable)  { (RegisterAddress), &(Variable), sizeof((Variable)) }

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

EXTERN_C_START

NTSTATUS I2C_Initialize (_In_ PDEVICE_CONTEXT pDevContext, _In_ WDFCMRESLIST hResourcesRaw, _In_ WDFCMRESLIST hResourcesTranslated);
void     I2C_PerformDeviceReset (_In_ PDEVICE_CONTEXT pDevContext);
NTSTATUS I2C_Open(_In_ PDEVICE_CONTEXT pDevContext);
void     I2C_Close(_In_ PDEVICE_CONTEXT pDevContext);
NTSTATUS I2C_WriteAsynchronously(_In_ PDEVICE_CONTEXT pDevContext, _In_ UINT8 RegisterAddress, _In_reads_bytes_(Length) PVOID Data, _In_ ULONG Length);
NTSTATUS I2C_ReadSynchronously(_In_ PDEVICE_CONTEXT pDevContext, _In_ I2C_REQUEST_SOURCE requestSource, _In_ UINT8 RegisterAddress, _Out_writes_bytes_(Length) PVOID Data, _In_ ULONG Length);
NTSTATUS I2C_WriteSynchronously(_In_ PDEVICE_CONTEXT pDevContext, _In_ I2C_REQUEST_SOURCE requestSource, _In_ UINT8 RegisterAddress, _In_reads_(Length) PVOID Data, _In_ ULONG Length);
NTSTATUS I2C_ReadSynchronouslyMultiple(_In_ PDEVICE_CONTEXT pDevContext, _In_ I2C_REQUEST_SOURCE requestSource, _Inout_updates_(Count) REGISTER_ITEM* Items, _In_ ULONG Count);
EVT_WDF_REQUEST_COMPLETION_ROUTINE I2C_OnWriteCompletion;
NTSTATUS GPIO_Open(_In_ PDEVICE_CONTEXT pDevContext);
void     GPIO_Close(_In_ PDEVICE_CONTEXT pDevContext);
NTSTATUS GPIO_PlugOrientation_Set(_In_ PDEVICE_CONTEXT pDevContext, TCPC_PHY_TCPC_CONTROL_t TCPC_CONTROL);

EXTERN_C_END