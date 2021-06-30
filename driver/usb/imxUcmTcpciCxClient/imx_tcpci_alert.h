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

#define MAX_ALERTS 12
#define MAX_ALERTS_TO_PROCESS 10

EXTERN_C_START

EVT_WDF_INTERRUPT_ISR OnInterruptPassiveIsr;

EXTERN_C_END