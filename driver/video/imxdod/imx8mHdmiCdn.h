/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _IMX8MHDMICDN_H_
#define _IMX8MHDMICDN_H_

#define HDMI_CDN_IMX8M_REGISTERS_LENGTH         0x100000

// HDMI cadence mailbox registers
#define IMX_HDMICDN_MAILBOX_FULL                0x0008
#define IMX_HDMICDN_MAILBOX_EMPTY               0x000C
#define IMX_HDMICDN_MAILBOX0_WR_DATA            0x0010
#define IMX_HDMICDN_MAILBOX0_RD_DATA            0x0014

#define IMX_HDMICDN_MAILBOX_FULL_MAILBOX_FULL   (0x01 << 0)
#define IMX_HDMICDN_MAILBOX_EMPTY_MAILBOX_EMPTY (0x01 << 0)

// HDMI cadence OpCodes
#define IMX_HDMICDN_GENERAL_READ_REGISTER       0x07
#define IMX_HDMICDN_GENERAL_WRITE_REGISTER      0x05

// HDMI cadence Module ID
#define IMX_HDMICDN_MB_MODULE_ID_DP_TX          0x01
#define IMX_HDMICDN_MB_MODULE_ID_DP_RX          0x02
#define IMX_HDMICDN_MB_MODULE_ID_HDMI_TX        0x03
#define IMX_HDMICDN_MB_MODULE_ID_HDMI_RX        0x04
#define IMX_HDMICDN_MB_MODULE_ID_MHL_TX         0x05
#define IMX_HDMICDN_MB_MODULE_ID_MHL_RX         0x06
#define IMX_HDMICDN_MB_MODULE_ID_HDCP_TX        0x07
#define IMX_HDMICDN_MB_MODULE_ID_HDCP_RX        0x08
#define IMX_HDMICDN_MB_MODULE_ID_HDCP_GENERAL   0x09
#define IMX_HDMICDN_MB_MODULE_ID_GENERAL        0x0A
#define IMX_HDMICDN_MB_MODULE_ID                1

/* register HDTX_CONTROLLER */
#define IMX_HDMICDN_ADDR_SOURCE_MHL_HD          0x01000
#define IMX_HDMICDN_HDTX_CONTROLLER             0x18
#define IMX_HDMICDN_HDTX_CONTROLLER_F_DATA_EN   (0x01 << 15)

/* APB read/write operation macros */
#define IMX_HDMICDN_APB_WRITE(value)                                     \
    do {                                                                 \
        while (this->readHdmiCdnRegister(IMX_HDMICDN_MAILBOX_FULL) &     \
              IMX_HDMICDN_MAILBOX_FULL_MAILBOX_FULL) {}                  \
        this->writeHdmiCdnRegister(IMX_HDMICDN_MAILBOX0_WR_DATA, value); \
    } while (0)

#define IMX_HDMICDN_APB_READ(value)                                      \
    do {                                                                 \
        while (this->readHdmiCdnRegister(IMX_HDMICDN_MAILBOX_EMPTY) &    \
              IMX_HDMICDN_MAILBOX_EMPTY_MAILBOX_EMPTY) {}                \
        value = this->readHdmiCdnRegister(IMX_HDMICDN_MAILBOX0_RD_DATA); \
    } while (0)

#define IMX_HDMICDN_APB_READ8(value)                                     \
    do {                                                                 \
        IMX_HDMICDN_APB_READ(value);                                     \
    } while (0)

#define IMX_HDMICDN_APB_READ16(value)                                    \
    do {                                                                 \
        UINT32 rdValue;                                                  \
        IMX_HDMICDN_APB_READ(rdValue);                                   \
        value = (rdValue << 8);                                          \
        IMX_HDMICDN_APB_READ(rdValue);                                   \
        value |= (rdValue << 0);                                         \
    } while (0)

#define IMX_HDMICDN_APB_READ32(value)                                    \
    do {                                                                 \
        UINT32 rdValue;                                                  \
        IMX_HDMICDN_APB_READ(rdValue);                                   \
        value = (rdValue << 24);                                         \
        IMX_HDMICDN_APB_READ(rdValue);                                   \
        value |= (rdValue << 16);                                        \
        IMX_HDMICDN_APB_READ(rdValue);                                   \
        value = (rdValue << 8);                                          \
        IMX_HDMICDN_APB_READ(rdValue);                                   \
        value |= (rdValue << 0);                                         \
    } while (0)

#define IMX_HDMICDN_APB_WRITE8(value)                                    \
    do {                                                                 \
        IMX_HDMICDN_APB_WRITE(value);                                    \
    } while (0)

#define IMX_HDMICDN_APB_WRITE16(value)                                   \
    do {                                                                 \
        IMX_HDMICDN_APB_WRITE((value >> 8) & 0xFF);                      \
        IMX_HDMICDN_APB_WRITE((value >> 0) & 0xFF);                      \
    } while (0)

#define IMX_HDMICDN_APB_WRITE32(value)                                   \
    do {                                                                 \
        IMX_HDMICDN_APB_WRITE((value >> 24) & 0xFF);                     \
        IMX_HDMICDN_APB_WRITE((value >> 16) & 0xFF);                     \
        IMX_HDMICDN_APB_WRITE((value >> 8) & 0xFF);                      \
        IMX_HDMICDN_APB_WRITE((value >> 0) & 0xFF);                      \
    } while (0)

#endif