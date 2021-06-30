/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License.

Module Name:

    imxomxg1.cpp - decoder creation entry points

Abstract:

   This file contains entry points for decoders available on the Hantro G1
   hardware, but not enabled in this software version.
*/

#include <string.h>
#include "OSAL.h"
#include "codec_avs.h"
#include "codec_jpeg.h"
#include "codec_mpeg2.h"
#include "codec_mpeg4.h"
#include "codec_vc1.h"
#include "codec_vp6.h"
#include "codec_vp8.h"
#include "codec_webp.h"

CODEC_PROTOTYPE *HantroHwDecOmx_decoder_create_avs(const void *DWLInstance,
    OMX_VIDEO_PARAM_G1CONFIGTYPE *g1Conf)
{
    return NULL;
}

CODEC_PROTOTYPE *HantroHwDecOmx_decoder_create_jpeg(OMX_BOOL motion_jpeg)
{
    return NULL;
}

CODEC_PROTOTYPE *HantroHwDecOmx_decoder_create_mpeg2(const void *DWLInstance,
    OMX_VIDEO_PARAM_G1CONFIGTYPE *g1Conf)
{
    return NULL;
}

CODEC_PROTOTYPE *HantroHwDecOmx_decoder_create_mpeg4(const void *DWLInstance,
    OMX_BOOL enable_deblocking,
    MPEG4_FORMAT format,
    OMX_VIDEO_PARAM_G1CONFIGTYPE *g1Conf)
{
    return NULL;
}

CODEC_PROTOTYPE *HantroHwDecOmx_decoder_create_vc1(const void *DWLInstance,
    OMX_VIDEO_PARAM_G1CONFIGTYPE *g1Conf)
{
    return NULL;
}

CODEC_PROTOTYPE *HantroHwDecOmx_decoder_create_vp6(const void *DWLInstance,
    OMX_VIDEO_PARAM_G1CONFIGTYPE *g1Conf)
{
    return NULL;
}

CODEC_PROTOTYPE *HantroHwDecOmx_decoder_create_webp(const void *DWLInstance)
{
    return NULL;
}
