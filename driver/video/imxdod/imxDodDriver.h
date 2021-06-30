// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
//
// Module Name:
//
//  imxDodDriver.h
//
// Abstract:
//
//    This is IMXDOD driver initialization
//
// Environment:
//
//    Kernel mode only.
//

#ifndef _IMXDODDRIVER_HPP_
#define  _IMXDODDRIVER_HPP_ 1

extern "C" DRIVER_INITIALIZE DriverEntry;

DXGKDDI_UNLOAD IMXDodDdiUnload;

#endif // _IMXDODDRIVER_HPP_
