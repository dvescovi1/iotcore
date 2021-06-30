/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License.

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_imxvpukm,
    0xada9253b,0x628c,0x40ce,0xb2,0xc1,0x19,0xf4,0x89,0xa0,0xf3,0xda);
// {ada9253b-628c-40ce-b2c1-19f489a0f3da}

// read # of Cores : out Buffer=(ULONG)
#define IOCTL_VPU_MC_CORES     CTL_CODE(FILE_DEVICE_DEVAPI, 0x00, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define IOCTL_VPU_MC_OFFSETS   CTL_CODE(FILE_DEVICE_DEVAPI, 0x01, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Get core ID for media type : in/out buffer=(ULONG),  input = DWL client type, output = core ID
#define IOCTL_VPU_CORE_ID     CTL_CODE(FILE_DEVICE_DEVAPI, 0x02, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Get ASIC id for core ID : in/out buffer=(ULONG)  input = core_id, output=asic_id
#define IOCTL_VPU_ASIC_ID     CTL_CODE(FILE_DEVICE_DEVAPI, 0x03, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Map VPU HW memory into user space : in buffer = (ULONG) core_id, out buffer=(void *) virtual address
#define IOCTL_VPU_HW_ADDR     CTL_CODE(FILE_DEVICE_DEVAPI, 0x04, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Return HW reg size : in/out buffer=(ULONG) input = core_id, output=core size
#define IOCTL_VPU_HW_SIZE     CTL_CODE(FILE_DEVICE_DEVAPI, 0x05, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Allocate ram block for VPU use : in/out buffer=(IMXVPU_MEMORY) input = size, output=memory block
#define IOCTL_VPU_ALLOC_MEM   CTL_CODE(FILE_DEVICE_DEVAPI, 0x06, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Free ram block for VPU use : in buffer=(IMXVPU_MEMORY) input = memory block
#define IOCTL_VPU_FREE_MEM   CTL_CODE(FILE_DEVICE_DEVAPI, 0x07, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Pull codec registers from device : in/out buffer=(IMX_VPUREGISTERS)
#define IOCTL_VPU_DEC_PULL_REG   CTL_CODE(FILE_DEVICE_DEVAPI, 0x08, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Push codec registers to device : in buffer=(IMX_VPUREGISTERS)
#define IOCTL_VPU_DEC_PUSH_REG   CTL_CODE(FILE_DEVICE_DEVAPI, 0x09, METHOD_BUFFERED, FILE_ANY_ACCESS)

// reserve HW decoder : in/out buffer=(ULONG), input = DWL client type output = core ID
#define IOCTL_VPU_DEC_RESERVE   CTL_CODE(FILE_DEVICE_DEVAPI, 0x0A, METHOD_BUFFERED, FILE_ANY_ACCESS)

// release HW decoder : in buffer=(ULONG), input = core ID
#define IOCTL_VPU_DEC_RELEASE   CTL_CODE(FILE_DEVICE_DEVAPI, 0x0B, METHOD_BUFFERED, FILE_ANY_ACCESS)

// flush data cache to VPU ram block : in buffer=(IMXVPU_MEMORY), input = memory block
#define IOCTL_VPU_CACHE_FLUSH   CTL_CODE(FILE_DEVICE_DEVAPI, 0x0C, METHOD_BUFFERED, FILE_ANY_ACCESS)

// invalidate data cache for VPU ram block : in buffer=(IMXVPU_MEMORY), input = memory block
#define IOCTL_VPU_CACHE_INVALIDATE   CTL_CODE(FILE_DEVICE_DEVAPI, 0x0D, METHOD_BUFFERED, FILE_ANY_ACCESS)

EXTERN_C_START

// TOTAL_X170_ORIGIN_REGS from deccfg.h
#define MAX_REGS_G1 119

// DEC_X170_REGISTERS from deccfg.h
#define MAX_REGS_G2 265

typedef struct {
    ULONG coreid;
    ULONG regSize;
    ULONG regs[MAX_REGS_G2];
} IMXVPU_REGISTERS;

typedef struct {
    void * virtAddress;
    long long physAddress;
    ULONG size;
    ULONG flags;
} IMXVPU_MEMORY;

#define VPU_MEM_UNCACHED        0
#define VPU_MEM_WRITE_COMBINED  1
#define VPU_MEM_CACHED          2

EXTERN_C_END
