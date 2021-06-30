#pragma once

/*------------------------------------------------------------------------------
--       Copyright (c) 2015-2017, VeriSilicon Inc. All rights reserved        --
--         Copyright (c) 2011-2014, Google Inc. All rights reserved.          --
--         Copyright (c) 2019, Microsoft Inc. All rights reserved.            --
--                                                                            --
-- This software is confidential and proprietary and may be used only as      --
--   expressly authorized by VeriSilicon in a written licensing agreement.    --
--                                                                            --
--         This entire notice must be reproduced on all copies                --
--                       and may not be removed.                              --
--                                                                            --
--------------------------------------------------------------------------------
-- Redistribution and use in source and binary forms, with or without         --
-- modification, are permitted provided that the following conditions are met:--
--   * Redistributions of source code must retain the above copyright notice, --
--       this list of conditions and the following disclaimer.                --
--   * Redistributions in binary form must reproduce the above copyright      --
--       notice, this list of conditions and the following disclaimer in the  --
--       documentation and/or other materials provided with the distribution. --
--   * Neither the names of Google nor the names of its contributors may be   --
--       used to endorse or promote products derived from this software       --
--       without specific prior written permission.                           --
--------------------------------------------------------------------------------
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"--
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE  --
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE --
-- ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE  --
-- LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR        --
-- CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF       --
-- SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS   --
-- INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN    --
-- CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)    --
-- ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE --
-- POSSIBILITY OF SUCH DAMAGE.                                                --
--------------------------------------------------------------------------------
------------------------------------------------------------------------------*/


#include "basetype.h"
#include "dwl_defs.h"
#include "dwl.h"
#include "dwl_activity_trace.h"
// #include "memalloc.h"
#include <assert.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/ioctl.h>
// #include <sys/mman.h>
#include <sys/stat.h>

#include <sys/timeb.h>
#include <sys/types.h>

#include <tracing.h>

#ifdef _DWL_DEBUG

#define DWL_DEBUG(fmt, ...) \
  TraceStringA(&g_traceContext, 4, __FILE__ ":%d:%s() " fmt, __LINE__, __func__, __VA_ARGS__)
#else
#define DWL_DEBUG(fmt, ...) \
  do {                          \
  } while (0); /* not debugging: nothing */
#endif

EXTERN_C_START

struct core_desc {
	u32 id; /* id of the Core */
	u32 *regs; /* pointer to user registers */
	u32 size; /* size of register space */
};

#define HANTRODECPP_REG_START 0x400
#define HANTRODEC_REG_START 0x4

#define HANTRODECPP_FUSE_CFG 99
#define HANTRODEC_FUSE_CFG 57

#define DWL_DECODER_INT \
  ((DWLReadReg(dec_dwl, HANTRODEC_REG_START) >> 11) & 0xFFU)
#define DWL_PP_INT      ((DWLReadReg(dec_dwl, HANTRODECPP_REG_START) >> 11) & 0xFFU)

#define DEC_IRQ_ABORT (1 << 11)
#define DEC_IRQ_RDY (1 << 12)
#define DEC_IRQ_BUS (1 << 13)
#define DEC_IRQ_BUFFER (1 << 14)
#define DEC_IRQ_ASO (1 << 15)
#define DEC_IRQ_ERROR (1 << 16)
#define DEC_IRQ_SLICE (1 << 17)
#define DEC_IRQ_TIMEOUT (1 << 18)

#define PP_IRQ_RDY             (1 << 12)
#define PP_IRQ_BUS             (1 << 13)

#define DWL_HW_ENABLE_BIT 0x000001 /* 0th bit */

#ifdef _DWL_HW_PERFORMANCE
/* signal that decoder/pp is enabled */
void DwlDecoderEnable(void);
#endif

struct MCListenerThreadParams {
	int fd;
	int b_stopped;
	unsigned int n_dec_cores;
	unsigned int n_ppcores;
	sem_t sc_dec_rdy_sem[MAX_ASIC_CORES];
	sem_t sc_pp_rdy_sem[MAX_ASIC_CORES];
	DWLIRQCallbackFn *callback[MAX_ASIC_CORES];
	void *callback_arg[MAX_ASIC_CORES];
};

/* wrapper information */
struct HX170DWL {
	u32 client_type;
	IDeviceIoControl * fd;          /* decoder device file */
	int fd_mem;      /* /dev/mem for mapping */
	IDeviceIoControl * fd_memalloc; /* linear memory allocator */
	u32 num_cores;
	u32 reg_size;         /* IO mem size */
	addr_t free_lin_mem;     /* Start address of free linear memory */
	addr_t free_ref_frm_mem; /* Start address of free reference frame memory */
	int semid;
	struct MCListenerThreadParams *sync_params;
	struct ActivityTrace activity;
	u32 b_ppreserved;
	u32 asic_id;
};

extern i32 DWLWaitDecHwReady(const void *instance, i32 core_id, u32 timeout);
extern u32 *DWLMapRegisters(void * mem_dev, unsigned long * base, unsigned int reg_size,
	u32 write);
extern void DWLUnmapRegisters(const void *io, unsigned int reg_size);

#define NEXT_MULTIPLE(value, n) (((value) + (n) - 1) & ~((n) - 1))
#define MAP_FAILED ((u32 *)0x80000000LL)

/*
#define _IO(magic, op) 0
#define _IOR(magic, op, value) 0
#define _IOW(magic, op, value) 0
#define _IOWR(magic, op, value) 0
 */
#define _IOWR(x,y,t)     (IOC_OUT | IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))


/* Use 'k' as magic number */
#define HANTRODEC_IOC_MAGIC  'k'

/*
 * S means "Set" through a ptr,
 * T means "Tell" directly with the argument value
 * G means "Get": reply by setting through a pointer
 * Q means "Query": response is on the return value
 * X means "eXchange": G and S atomically
 * H means "sHift": T and Q atomically
 */

#define HANTRODEC_PP_INSTANCE       _IO(HANTRODEC_IOC_MAGIC, 1)
#define HANTRODEC_HW_PERFORMANCE    _IO(HANTRODEC_IOC_MAGIC, 2)

#define HANTRODEC_IOCGHWOFFSET      _IOWR(HANTRODEC_IOC_MAGIC,  3, addr_t)
#define HANTRODEC_IOCGHWIOSIZE      _IOWR(HANTRODEC_IOC_MAGIC,  4, unsigned int)

#define HANTRODEC_IOC_CLI           _IO(HANTRODEC_IOC_MAGIC,  5)
#define HANTRODEC_IOC_STI           _IO(HANTRODEC_IOC_MAGIC,  6)

#define HANTRODEC_IOC_MC_OFFSETS    _IOR(HANTRODEC_IOC_MAGIC, 7, unsigned long)
#define HANTRODEC_IOC_MC_CORES      _IOR(HANTRODEC_IOC_MAGIC, 8, unsigned int)

#define HANTRODEC_IOCS_DEC_PUSH_REG  _IOW(HANTRODEC_IOC_MAGIC, 9, struct core_desc)
#define HANTRODEC_IOCS_PP_PUSH_REG   _IOW(HANTRODEC_IOC_MAGIC, 10, struct core_desc)

#define HANTRODEC_IOCH_DEC_RESERVE   _IOWR(HANTRODEC_IOC_MAGIC, 11, unsigned int)
#define HANTRODEC_IOCT_DEC_RELEASE   _IOW(HANTRODEC_IOC_MAGIC, 12, unsigned int)
#define HANTRODEC_IOCQ_PP_RESERVE    _IO(HANTRODEC_IOC_MAGIC, 13)
#define HANTRODEC_IOCT_PP_RELEASE    _IO(HANTRODEC_IOC_MAGIC, 14)

#define HANTRODEC_IOCX_DEC_WAIT      _IOWR(HANTRODEC_IOC_MAGIC, 15, struct core_desc)
#define HANTRODEC_IOCX_PP_WAIT       _IOWR(HANTRODEC_IOC_MAGIC, 16, struct core_desc)

#define HANTRODEC_IOCS_DEC_PULL_REG  _IOWR(HANTRODEC_IOC_MAGIC, 17, struct core_desc)
#define HANTRODEC_IOCS_PP_PULL_REG   _IOWR(HANTRODEC_IOC_MAGIC, 18, struct core_desc)

#define HANTRODEC_IOCG_CORE_WAIT     _IOR(HANTRODEC_IOC_MAGIC, 19, int)

#define HANTRODEC_IOX_ASIC_ID        _IOWR(HANTRODEC_IOC_MAGIC, 20, unsigned int)

#define HANTRODEC_IOCG_CORE_ID       _IOWR(HANTRODEC_IOC_MAGIC, 21, int)		

#define HANTRODEC_DEBUG_STATUS       _IO(HANTRODEC_IOC_MAGIC, 29)

#define HANTRODEC_IOC_MAXNR 29

#define MEMALLOC_IOC_MAGIC  'k'

#define MEMALLOC_IOCXGETBUFFER		_IOWR(MEMALLOC_IOC_MAGIC, 100, IMXVPU_MEMORY)
#define MEMALLOC_IOCSFREEBUFFER		_IOW(MEMALLOC_IOC_MAGIC, 101, IMXVPU_MEMORY)

#define MEMALLOC_IOCSINVALIDATEBUFFER _IOW(MEMALLOC_IOC_MAGIC, 102, addr_t)
#define MEMALLOC_IOCSFLUSHBUFFER	  _IOW(MEMALLOC_IOC_MAGIC, 103, addr_t)

EXTERN_C_END
