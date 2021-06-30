/*
* This file is derived from Microsoft example available on https://github.com/microsoft/Windows-driver-samples/blob/master/usb/UcmTcpciCxClientSample
* Original file licensing:
* Copyright (c) 2015 Microsoft
* SPDX-License-Identifier: MS-PL
* NXP modifications are licensed under the same license
* Copyright 2020 NXP
*
*/

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>
#include <wdmguid.h>
#define RESHUB_USE_HELPER_ROUTINES
#include <reshub.h>
#include <spb.h>
#include <UcmTcpciCx.h>
#include <gpio.h>

#include "imx_tcpci_dbg.h"
#include "imx_tcpci_phy_io_map.h"
#include "imx_tcpci_i2c.h"
#include "imx_tcpci_device.h"
#include "imx_tcpci_alert.h"
#include "imx_tcpci_ioctl.h"

EXTERN_C_START

DRIVER_INITIALIZE              DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD      EvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP EvtDriverContextCleanup;

EXTERN_C_END
