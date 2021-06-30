/*
* Copyright 2020 NXP
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

#ifndef _MP_DBG_H
#define _MP_DBG_H

#ifdef DBG

#include "imx_tcpci_phy_io_map.h"

#pragma warning(disable:4189)   // local variable is initialized but not used
#pragma warning(disable:4152)   // non standard extension, function/data ptr conversion in expression used

// DO NOT COMMENT OUT ERROR MACROS in debug build
#define DBG_PRINT_ERROR(_format_str_,...) DbgPrintEx(DPFLTR_IHVDRIVER_ID,0xFFFFFFFE,"%08d, C%d D%d %s:%s ERROR !!!"_format_str_"\n",KeGetCurrentProcessorNumber(),KeGetCurrentIrql(),"TCPCIDrv",__FUNCTION__,__VA_ARGS__)

// Uncomment next line for debug message printing
#define DBG_MESSAGE_PRINTING
#ifdef DBG_MESSAGE_PRINTING

#define I2C_READ  0
#define I2C_WRITE 1

typedef struct _DEVICE_CONTEXT DEVICE_CONTEXT, *PDEVICE_CONTEXT;
EXTERN_C_START
const char* Dbg_GetIOCTLName(ULONG i);
void TCPC_PHY_DumpReg(TCPC_PHY_t *pRegs, UINT32 RegAddress, UINT32 params, ULONG Length);
void TCPC_PHY_ReadAllRegs(PDEVICE_CONTEXT pDevContext);
EXTERN_C_END

// Print IO to TCPCI controller registers
#define DBG_PRINT_REG(_format_str_,...) DbgPrintEx(DPFLTR_IHVDRIVER_ID,0xFFFFFFFE,"C%d D%d %s:%-30s "_format_str_"\n",KeGetCurrentProcessorNumber(),KeGetCurrentIrql(),"TCPCIDrv",__FUNCTION__,__VA_ARGS__)
#define DBG_PRINT_INFO(_format_str_,...) DbgPrintEx(DPFLTR_IHVDRIVER_ID,0xFFFFFFFE,"C%d D%d %s:%s "_format_str_"\n",KeGetCurrentProcessorNumber(),KeGetCurrentIrql(),"TCPCIDrv",__FUNCTION__,__VA_ARGS__)

// TCPCI driver specific macros - uncomment next line for message printing

#else // DBG_MESSAGE_PRINTING

#define TCPC_PHY_DumpReg(...)
#define TCPC_PHY_ReadAllRegs(...)

#endif // DBG_MESSAGE_PRINTING

#else // DBG

#define TCPC_PHY_DumpReg(...)
#define TCPC_PHY_ReadAllRegs(...)

#endif // DBG

#ifndef DBG_PRINT_REG
#define DBG_PRINT_REG(...)
#endif
#ifndef DBG_PRINT_INFO
#define DBG_PRINT_INFO(...)
#endif
#ifndef DBG_PRINT_ERROR
#define DBG_PRINT_ERROR(...)
#endif

#endif // _MP_DBG_H
