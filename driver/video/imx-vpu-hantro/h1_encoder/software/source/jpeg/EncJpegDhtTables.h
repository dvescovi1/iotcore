/*------------------------------------------------------------------------------
--       Copyright (c) 2015-2017, VeriSilicon Inc. All rights reserved        --
--         Copyright (c) 2011-2014, Google Inc. All rights reserved.          --
--         Copyright (c) 2007-2010, Hantro OY. All rights reserved.           --
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
--                                                                            --
--  Abstract  : 
--
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------

    Table of contents 
   
    1. Include headers
    2. Module defines
    3. Data types
    4. Function prototypes

------------------------------------------------------------------------------*/
#ifndef __ENC_JPEG_DHT_TABLES_H__
#define __ENC_JPEG_DHT_TABLES_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "basetype.h"

/*------------------------------------------------------------------------------
    2. Module defines
------------------------------------------------------------------------------*/

/*------------------ DC Huffman codes for DHT header --------------------*/

typedef struct JpegEncDcDhtTables_t
{
    u32 DcLumLi;
    u32 DcChromLi;

} JpegEncDcDhtTables;

/* Table for Luminance & Chrominace DC coefficient to DHT */

static const JpegEncDcDhtTables Dc_Li[16] = {
    {0x00, 0x00},
    {0x01, 0x03},
    {0x05, 0x01},
    {0x01, 0x01},
    {0x01, 0x01},
    {0x01, 0x01},
    {0x01, 0x01},
    {0x01, 0x01},
    {0x01, 0x01},
    {0x00, 0x01},
    {0x00, 0x01},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00}
};

/* Table for Luminance & Chrominace DC coefficient value associated to Li */

typedef struct JpegEncDcVijTables_t
{
    u32 DcLumVij;
    u32 DcChromVij;

} JpegEncDcVijTables;

static const JpegEncDcVijTables Vij_Dc[12] = {
    {0x00, 0x00},
    {0x01, 0x01},
    {0x02, 0x02},
    {0x03, 0x03},
    {0x04, 0x04},
    {0x05, 0x05},
    {0x06, 0x06},
    {0x07, 0x07},
    {0x08, 0x08},
    {0x09, 0x09},
    {0x0A, 0x0A},
    {0x0B, 0x0B},
};

/* AC Huffman codes for DHT header */

/*------------------ AC Huffman codes for DHT header --------------------*/

typedef struct JpegEncAcDhtTables_t
{
    u32 AcLumLi;
    u32 AcChromLi;

} JpegEncAcDhtTables;

/* Table for Luminance & Chrominace AC coefficient to DHT */

static const JpegEncAcDhtTables Ac_Li[16] = {
    {0x00, 0x00},
    {0x02, 0x02},
    {0x01, 0x01},
    {0x03, 0x02},
    {0x03, 0x04},
    {0x02, 0x04},
    {0x04, 0x03},
    {0x03, 0x04},
    {0x05, 0x07},
    {0x05, 0x05},
    {0x04, 0x04},
    {0x04, 0x04},
    {0x00, 0x00},
    {0x00, 0x01},
    {0x01, 0x02},
    {0x7D, 0x77}
};

/* Table for Luminance & Chrominace AC coefficient value associated to Li */

typedef struct JpegEncAcVijTables_t
{
    u32 AcLumVij;
    u32 AcChromVij;

} JpegEncAcVijTables;

static const JpegEncAcVijTables Vij_Ac[162] = {
    {0x01, 0x00},
    {0x02, 0x01},
    {0x03, 0x02},
    {0x00, 0x03},
    {0x04, 0x11},
    {0x11, 0x04},
    {0x05, 0x05},
    {0x12, 0x21},
    {0x21, 0x31},
    {0x31, 0x06},
    {0x41, 0x12},
    {0x06, 0x41},
    {0x13, 0x51},
    {0x51, 0x07},
    {0x61, 0x61},
    {0x07, 0x71},
    {0x22, 0x13},
    {0x71, 0x22},
    {0x14, 0x32},
    {0x32, 0x81},
    {0x81, 0x08},
    {0x91, 0x14},
    {0xA1, 0x42},
    {0x08, 0x91},
    {0x23, 0xA1},
    {0x42, 0xB1},
    {0xB1, 0xC1},
    {0xC1, 0x09},
    {0x15, 0x23},
    {0x52, 0x33},
    {0xD1, 0x52},
    {0xF0, 0xF0},
    {0x24, 0x15},
    {0x33, 0x62},
    {0x62, 0x72},
    {0x72, 0xD1},
    {0x82, 0x0A},
    {0x09, 0x16},
    {0x0A, 0x24},
    {0x16, 0x34},
    {0x17, 0xE1},
    {0x18, 0x25},
    {0x19, 0xF1},
    {0x1A, 0x17},
    {0x25, 0x18},
    {0x26, 0x19},
    {0x27, 0x1A},
    {0x28, 0x26},
    {0x29, 0x27},
    {0x2A, 0x28},
    {0x34, 0x29},
    {0x35, 0x2A},
    {0x36, 0x35},
    {0x37, 0x36},
    {0x38, 0x37},
    {0x39, 0x38},
    {0x3A, 0x39},
    {0x43, 0x3A},
    {0x44, 0x43},
    {0x45, 0x44},
    {0x46, 0x45},
    {0x47, 0x46},
    {0x48, 0x47},
    {0x49, 0x48},
    {0x4A, 0x49},
    {0x53, 0x4A},
    {0x54, 0x53},
    {0x55, 0x54},
    {0x56, 0x55},
    {0x57, 0x56},
    {0x58, 0x57},
    {0x59, 0x58},
    {0x5A, 0x59},
    {0x63, 0x5A},
    {0x64, 0x63},
    {0x65, 0x64},
    {0x66, 0x65},
    {0x67, 0x66},
    {0x68, 0x67},
    {0x69, 0x68},
    {0x6A, 0x69},
    {0x73, 0x6A},
    {0x74, 0x73},
    {0x75, 0x74},
    {0x76, 0x75},
    {0x77, 0x76},
    {0x78, 0x77},
    {0x79, 0x78},
    {0x7A, 0x79},
    {0x83, 0x7A},
    {0x84, 0x82},
    {0x85, 0x83},
    {0x86, 0x84},
    {0x87, 0x85},
    {0x88, 0x86},
    {0x89, 0x87},
    {0x8A, 0x88},
    {0x92, 0x89},
    {0x93, 0x8A},
    {0x94, 0x92},
    {0x95, 0x93},
    {0x96, 0x94},
    {0x97, 0x95},
    {0x98, 0x96},
    {0x99, 0x97},
    {0x9A, 0x98},
    {0xA2, 0x99},
    {0xA3, 0x9A},
    {0xA4, 0xA2},
    {0xA5, 0xA3},
    {0xA6, 0xA4},
    {0xA7, 0xA5},
    {0xA8, 0xA6},
    {0xA9, 0xA7},
    {0xAA, 0xA8},
    {0xB2, 0xA9},
    {0xB3, 0xAA},
    {0xB4, 0xB2},
    {0xB5, 0xB3},
    {0xB6, 0xB4},
    {0xB7, 0xB5},
    {0xB8, 0xB6},
    {0xB9, 0xB7},
    {0xBA, 0xB8},
    {0xC2, 0xB9},
    {0xC3, 0xBA},
    {0xC4, 0xC2},
    {0xC5, 0xC3},
    {0xC6, 0xC4},
    {0xC7, 0xC5},
    {0xC8, 0xC6},
    {0xC9, 0xC7},
    {0xCA, 0xC8},
    {0xD2, 0xC9},
    {0xD3, 0xCA},
    {0xD4, 0xD2},
    {0xD5, 0xD3},
    {0xD6, 0xD4},
    {0xD7, 0xD5},
    {0xD8, 0xD6},
    {0xD9, 0xD7},
    {0xDA, 0xD8},
    {0xE1, 0xD9},
    {0xE2, 0xDA},
    {0xE3, 0xE2},
    {0xE4, 0xE3},
    {0xE5, 0xE4},
    {0xE6, 0xE5},
    {0xE7, 0xE6},
    {0xE8, 0xE7},
    {0xE9, 0xE8},
    {0xEA, 0xE9},
    {0xF1, 0xEA},
    {0xF2, 0xF2},
    {0xF3, 0xF3},
    {0xF4, 0xF4},
    {0xF5, 0xF5},
    {0xF6, 0xF6},
    {0xF7, 0xF7},
    {0xF8, 0xF8},
    {0xF9, 0xF9},
    {0xFA, 0xFA}
};

/*------------------------------------------------------------------------------
    3. Data types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    4. Function prototypes
------------------------------------------------------------------------------*/

#endif
