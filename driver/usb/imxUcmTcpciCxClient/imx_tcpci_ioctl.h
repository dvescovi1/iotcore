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

EVT_WDF_WORKITEM Evt_WorkItem_GetStatus;
EVT_WDF_WORKITEM Evt_WorkItem_GetControl;
EVT_WDF_WORKITEM Evt_WorkItem_SetControl;

void PostponeToWorkitem(_In_ WDFREQUEST Request, _In_ WDFDEVICE Device, _In_ WDFWORKITEM Workitem);
void Evt_SetCommand(_In_ WDFREQUEST Request, _In_ WDFDEVICE Device);
void Evt_SetConfigStandardOutput(_In_ WDFREQUEST Request, _In_ WDFDEVICE Device);
void Evt_SetMessageHeaderInfo(_In_ WDFREQUEST Request, _In_ WDFDEVICE Device);
void Evt_SetReceiveDetect(_In_ WDFREQUEST Request, _In_ WDFDEVICE Device);
void Evt_SetTransmit(_In_ WDFREQUEST Request, _In_ WDFDEVICE Device);
void Evt_SetTransmitBuffer(_In_ WDFREQUEST Request, _In_ WDFDEVICE Device);

EXTERN_C_START

NTSTATUS                           HardwareRequestQueueInitialize(_In_ WDFDEVICE Device);
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL EvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP           EvtIoStop;

EXTERN_C_END