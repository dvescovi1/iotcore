// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// Module Name:
//
//   iMX8M.inc
//
// Abstract:
//
//  This module contains iMX8M definitions
//
// Environment:
//
//  Kernel mode only
//

IMX_NONPAGED_SEGMENT_BEGIN; //====================================================

namespace {

    // i.MX8 SOC specific pin data
    // Default values are updated to values found in HW at IMX_GPIO::PrepareController
    //
    // IMX8<
    const IMX_PIN_DATA Imx8MGpioPinDataSparseMap[] = {
        //--------------------------------------------------
        // Pin Absolute Number | Ctl Reg Offset | Default Value | Mux Reg Offset | Mux Default
        //--------------------------------------------------
        { IMX_MAKE_PIN_1(1, 0), 0x290, 0x00000014, 0x28, 0x00 }, // GPIO1_IO00 GPIO1_IO00
        { IMX_MAKE_PIN_1(1, 1), 0x294, 0x00000016, 0x2C, 0x00 }, // GPIO1_IO01 GPIO1_IO01
        { IMX_MAKE_PIN_1(1, 2), 0x298, 0x00000056, 0x30, 0x00 }, // GPIO1_IO02 GPIO1_IO02
        { IMX_MAKE_PIN_1(1, 3), 0x29C, 0x00000016, 0x34, 0x00 }, // GPIO1_IO03 GPIO1_IO03
        { IMX_MAKE_PIN_1(1, 4), 0x2A0, 0x00000016, 0x38, 0x00 }, // GPIO1_IO04 GPIO1_IO04
        { IMX_MAKE_PIN_1(1, 5), 0x2A4, 0x00000056, 0x3C, 0x00 }, // GPIO1_IO05 GPIO1_IO05
        { IMX_MAKE_PIN_1(1, 6), 0x2A8, 0x00000016, 0x40, 0x00 }, // GPIO1_IO06 GPIO1_IO06
        { IMX_MAKE_PIN_1(1, 7), 0x2AC, 0x00001816, 0x44, 0x00 }, // GPIO1_IO07 GPIO1_IO07
        { IMX_MAKE_PIN_1(1, 8), 0x2B0, 0x00000016, 0x48, 0x00 }, // GPIO1_IO08 GPIO1_IO08
        { IMX_MAKE_PIN_1(1, 9), 0x2B4, 0x00000016, 0x4C, 0x00 }, // GPIO1_IO09 GPIO1_IO09
        { IMX_MAKE_PIN_1(1, 10), 0x2B8, 0x00000016, 0x50, 0x00 }, // GPIO1_IO10 GPIO1_IO10
        { IMX_MAKE_PIN_1(1, 11), 0x2BC, 0x00000016, 0x54, 0x00 }, // GPIO1_IO11 GPIO1_IO11
        { IMX_MAKE_PIN_1(1, 12), 0x2C0, 0x00000016, 0x58, 0x00 }, // GPIO1_IO12 GPIO1_IO12
        { IMX_MAKE_PIN_1(1, 13), 0x2C4, 0x00000016, 0x5C, 0x00 }, // GPIO1_IO13 GPIO1_IO13
        { IMX_MAKE_PIN_1(1, 14), 0x2C8, 0x00000016, 0x60, 0x00 }, // GPIO1_IO14 GPIO1_IO14
        { IMX_MAKE_PIN_1(1, 15), 0x2CC, 0x00000016, 0x64, 0x00 }, // GPIO1_IO15 GPIO1_IO15
        { IMX_MAKE_PIN_1(1, 16), 0x2D0, 0x00000016, 0x68, 0x05 }, // GPIO1_IO16 ENET_MDC
        { IMX_MAKE_PIN_1(1, 17), 0x2D4, 0x00000016, 0x6C, 0x05 }, // GPIO1_IO17 ENET_MDIO
        { IMX_MAKE_PIN_1(1, 18), 0x2D8, 0x00000016, 0x70, 0x05 }, // GPIO1_IO18 ENET_TD3
        { IMX_MAKE_PIN_1(1, 19), 0x2DC, 0x00000016, 0x74, 0x05 }, // GPIO1_IO19 ENET_TD2
        { IMX_MAKE_PIN_1(1, 20), 0x2E0, 0x00000016, 0x78, 0x05 }, // GPIO1_IO20 ENET_TD1
        { IMX_MAKE_PIN_1(1, 21), 0x2E4, 0x00000016, 0x7C, 0x05 }, // GPIO1_IO21 ENET_TD0
        { IMX_MAKE_PIN_1(1, 22), 0x2E8, 0x00001816, 0x80, 0x05 }, // GPIO1_IO22 ENET_TX_CTL
        { IMX_MAKE_PIN_1(1, 23), 0x2EC, 0x00000016, 0x84, 0x05 }, // GPIO1_IO23 ENET_TXC
        { IMX_MAKE_PIN_1(1, 24), 0x2F0, 0x00000016, 0x88, 0x05 }, // GPIO1_IO24 ENET_RX_CTL
        { IMX_MAKE_PIN_1(1, 25), 0x2F4, 0x00000016, 0x8C, 0x05 }, // GPIO1_IO25 ENET_RXC
        { IMX_MAKE_PIN_1(1, 26), 0x2F8, 0x00000016, 0x90, 0x05 }, // GPIO1_IO26 ENET_RD0
        { IMX_MAKE_PIN_1(1, 27), 0x2FC, 0x00000016, 0x94, 0x05 }, // GPIO1_IO27 ENET_RD1
        { IMX_MAKE_PIN_1(1, 28), 0x300, 0x00000016, 0x98, 0x05 }, // GPIO1_IO28 ENET_RD2
        { IMX_MAKE_PIN_1(1, 29), 0x304, 0x00000016, 0x9C, 0x05 }, // GPIO1_IO29 ENET_RD3
        { IMX_MAKE_PIN_1(2, 0), 0x308, 0x00000016, 0xA0, 0x05 }, // GPIO2_IO00 SD1_CLK
        { IMX_MAKE_PIN_1(2, 1), 0x30C, 0x00000016, 0xA4, 0x05 }, // GPIO2_IO01 SD1_CMD
        { IMX_MAKE_PIN_1(2, 2), 0x310, 0x00000016, 0xA8, 0x05 }, // GPIO2_IO02 SD1_DATA0
        { IMX_MAKE_PIN_1(2, 3), 0x314, 0x00000016, 0xAC, 0x05 }, // GPIO2_IO03 SD1_DATA1
        { IMX_MAKE_PIN_1(2, 4), 0x318, 0x00000016, 0xB0, 0x05 }, // GPIO2_IO04 SD1_DATA2
        { IMX_MAKE_PIN_1(2, 5), 0x31C, 0x00001816, 0xB4, 0x05 }, // GPIO2_IO05 SD1_DATA3
        { IMX_MAKE_PIN_1(2, 6), 0x320, 0x00000016, 0xB8, 0x05 }, // GPIO2_IO06 SD1_DATA4
        { IMX_MAKE_PIN_1(2, 7), 0x324, 0x00000016, 0xBC, 0x05 }, // GPIO2_IO07 SD1_DATA5
        { IMX_MAKE_PIN_1(2, 8), 0x328, 0x00000016, 0xC0, 0x05 }, // GPIO2_IO08 SD1_DATA6
        { IMX_MAKE_PIN_1(2, 9), 0x32C, 0x00000016, 0xC4, 0x05 }, // GPIO2_IO09 SD1_DATA7
        { IMX_MAKE_PIN_1(2, 10), 0x330, 0x00000016, 0xC8, 0x05 }, // GPIO2_IO10 SD1_RESET_B
        { IMX_MAKE_PIN_1(2, 11), 0x334, 0x00000016, 0xCC, 0x05 }, // GPIO2_IO11 SD1_STROBE
        { IMX_MAKE_PIN_1(2, 12), 0x338, 0x00000016, 0xD0, 0x05 }, // GPIO2_IO12 SD2_CD_B
        { IMX_MAKE_PIN_1(2, 13), 0x33C, 0x00000016, 0xD4, 0x05 }, // GPIO2_IO13 SD2_CLK
        { IMX_MAKE_PIN_1(2, 14), 0x340, 0x00000016, 0xD8, 0x05 }, // GPIO2_IO14 SD2_CMD
        { IMX_MAKE_PIN_1(2, 15), 0x344, 0x00001816, 0xDC, 0x05 }, // GPIO2_IO15 SD2_DATA0
        { IMX_MAKE_PIN_1(2, 16), 0x348, 0x00000016, 0xE0, 0x05 }, // GPIO2_IO16 SD2_DATA1
        { IMX_MAKE_PIN_1(2, 17), 0x34C, 0x00000016, 0xE4, 0x05 }, // GPIO2_IO17 SD2_DATA2
        { IMX_MAKE_PIN_1(2, 18), 0x350, 0x00000016, 0xE8, 0x05 }, // GPIO2_IO18 SD2_DATA3
        { IMX_MAKE_PIN_1(2, 19), 0x354, 0x00000016, 0xEC, 0x05 }, // GPIO2_IO19 SD2_RESET_B
        { IMX_MAKE_PIN_1(2, 20), 0x358, 0x00000016, 0xF0, 0x05 }, // GPIO2_IO20 SD2_WP
        { IMX_MAKE_PIN_1(3, 0), 0x35C, 0x00000016, 0xF4, 0x05 }, // GPIO3_IO00 NAND_ALE
        { IMX_MAKE_PIN_1(3, 1), 0x360, 0x00000016, 0xF8, 0x05 }, // GPIO3_IO01 NAND_CE0_B
        { IMX_MAKE_PIN_1(3, 2), 0x364, 0x00000016, 0xFC, 0x05 }, // GPIO3_IO02 NAND_CE1_B
        { IMX_MAKE_PIN_1(3, 3), 0x368, 0x00000016, 0x100, 0x05 }, // GPIO3_IO03 NAND_CE2_B
        { IMX_MAKE_PIN_1(3, 4), 0x36C, 0x00000016, 0x104, 0x05 }, // GPIO3_IO04 NAND_CE3_B
        { IMX_MAKE_PIN_1(3, 5), 0x370, 0x00000016, 0x108, 0x05 }, // GPIO3_IO05 NAND_CLE
        { IMX_MAKE_PIN_1(3, 6), 0x374, 0x00000016, 0x10C, 0x05 }, // GPIO3_IO06 NAND_DATA00
        { IMX_MAKE_PIN_1(3, 7), 0x378, 0x00000016, 0x110, 0x05 }, // GPIO3_IO07 NAND_DATA01
        { IMX_MAKE_PIN_1(3, 8), 0x37C, 0x00000016, 0x114, 0x05 }, // GPIO3_IO08 NAND_DATA02
        { IMX_MAKE_PIN_1(3, 9), 0x380, 0x00001816, 0x118, 0x05 }, // GPIO3_IO09 NAND_DATA03
        { IMX_MAKE_PIN_1(3, 10), 0x384, 0x00000016, 0x11C, 0x05 }, // GPIO3_IO10 NAND_DATA04
        { IMX_MAKE_PIN_1(3, 11), 0x388, 0x00000016, 0x120, 0x05 }, // GPIO3_IO11 NAND_DATA05
        { IMX_MAKE_PIN_1(3, 12), 0x38C, 0x00000016, 0x124, 0x05 }, // GPIO3_IO12 NAND_DATA06
        { IMX_MAKE_PIN_1(3, 13), 0x390, 0x00000016, 0x128, 0x05 }, // GPIO3_IO13 NAND_DATA07
        { IMX_MAKE_PIN_1(3, 14), 0x394, 0x00000016, 0x12C, 0x05 }, // GPIO3_IO14 NAND_DQS
        { IMX_MAKE_PIN_1(3, 15), 0x398, 0x00000016, 0x130, 0x05 }, // GPIO3_IO15 NAND_RE_B
        { IMX_MAKE_PIN_1(3, 16), 0x39C, 0x00000016, 0x134, 0x05 }, // GPIO3_IO16 NAND_READY_B
        { IMX_MAKE_PIN_1(3, 17), 0x3A0, 0x00000016, 0x138, 0x05 }, // GPIO3_IO17 NAND_WE_B
        { IMX_MAKE_PIN_1(3, 18), 0x3A4, 0x00000016, 0x13C, 0x05 }, // GPIO3_IO18 NAND_WP_B
        { IMX_MAKE_PIN_1(3, 19), 0x3A8, 0x00000016, 0x140, 0x05 }, // GPIO3_IO19 SAI5_RXFS
        { IMX_MAKE_PIN_1(3, 20), 0x3AC, 0x00000016, 0x144, 0x05 }, // GPIO3_IO20 SAI5_RXC
        { IMX_MAKE_PIN_1(3, 21), 0x3B0, 0x00000016, 0x148, 0x05 }, // GPIO3_IO21 SAI5_RXD0
        { IMX_MAKE_PIN_1(3, 22), 0x3B4, 0x00001816, 0x14C, 0x05 }, // GPIO3_IO22 SAI5_RXD1
        { IMX_MAKE_PIN_1(3, 23), 0x3B8, 0x00000016, 0x150, 0x05 }, // GPIO3_IO23 SAI5_RXD2
        { IMX_MAKE_PIN_1(3, 24), 0x3BC, 0x00000016, 0x154, 0x05 }, // GPIO3_IO24 SAI5_RXD3
        { IMX_MAKE_PIN_1(3, 25), 0x3C0, 0x00000016, 0x158, 0x05 }, // GPIO3_IO25 SAI5_MCLK
        { IMX_MAKE_PIN_1(4, 0), 0x3C4, 0x00000016, 0x15C, 0x05 }, // GPIO4_IO00 SAI1_RXFS
        { IMX_MAKE_PIN_1(4, 1), 0x3C8, 0x00000016, 0x160, 0x05 }, // GPIO4_IO01 SAI1_RXC
        { IMX_MAKE_PIN_1(4, 2), 0x3CC, 0x00000016, 0x164, 0x05 }, // GPIO4_IO02 SAI1_RXD0
        { IMX_MAKE_PIN_1(4, 3), 0x3D0, 0x00000016, 0x168, 0x05 }, // GPIO4_IO03 SAI1_RXD1
        { IMX_MAKE_PIN_1(4, 4), 0x3D4, 0x00000016, 0x16C, 0x05 }, // GPIO4_IO04 SAI1_RXD2
        { IMX_MAKE_PIN_1(4, 5), 0x3D8, 0x00000016, 0x170, 0x05 }, // GPIO4_IO05 SAI1_RXD3
        { IMX_MAKE_PIN_1(4, 6), 0x3DC, 0x00000016, 0x174, 0x05 }, // GPIO4_IO06 SAI1_RXD4
        { IMX_MAKE_PIN_1(4, 7), 0x3E0, 0x00000016, 0x178, 0x05 }, // GPIO4_IO07 SAI1_RXD5
        { IMX_MAKE_PIN_1(4, 8), 0x3E4, 0x00000016, 0x17C, 0x05 }, // GPIO4_IO08 SAI1_RXD6
        { IMX_MAKE_PIN_1(4, 9), 0x3E8, 0x00000016, 0x180, 0x05 }, // GPIO4_IO09 SAI1_RXD7
        { IMX_MAKE_PIN_1(4, 10), 0x3EC, 0x00001816, 0x184, 0x05 }, // GPIO4_IO10 SAI1_TXFS
        { IMX_MAKE_PIN_1(4, 11), 0x3F0, 0x00000016, 0x188, 0x05 }, // GPIO4_IO11 SAI1_TXC
        { IMX_MAKE_PIN_1(4, 12), 0x3F4, 0x00000016, 0x18C, 0x05 }, // GPIO4_IO12 SAI1_TXD0
        { IMX_MAKE_PIN_1(4, 13), 0x3F8, 0x00000016, 0x190, 0x05 }, // GPIO4_IO13 SAI1_TXD1
        { IMX_MAKE_PIN_1(4, 14), 0x3FC, 0x00000016, 0x194, 0x05 }, // GPIO4_IO14 SAI1_TXD2
        { IMX_MAKE_PIN_1(4, 15), 0x400, 0x00000016, 0x198, 0x05 }, // GPIO4_IO15 SAI1_TXD3
        { IMX_MAKE_PIN_1(4, 16), 0x404, 0x00000016, 0x19C, 0x05 }, // GPIO4_IO16 SAI1_TXD4
        { IMX_MAKE_PIN_1(4, 17), 0x408, 0x00000016, 0x1A0, 0x05 }, // GPIO4_IO17 SAI1_TXD5
        { IMX_MAKE_PIN_1(4, 18), 0x40C, 0x00000016, 0x1A4, 0x05 }, // GPIO4_IO18 SAI1_TXD6
        { IMX_MAKE_PIN_1(4, 19), 0x410, 0x00000016, 0x1A8, 0x05 }, // GPIO4_IO19 SAI1_TXD7
        { IMX_MAKE_PIN_1(4, 20), 0x414, 0x00000016, 0x1AC, 0x05 }, // GPIO4_IO20 SAI1_MCLK
        { IMX_MAKE_PIN_1(4, 21), 0x418, 0x00000016, 0x1B0, 0x05 }, // GPIO4_IO21 SAI2_RXFS
        { IMX_MAKE_PIN_1(4, 22), 0x41C, 0x00000016, 0x1B4, 0x05 }, // GPIO4_IO22 SAI2_RXC
        { IMX_MAKE_PIN_1(4, 23), 0x420, 0x00000016, 0x1B8, 0x05 }, // GPIO4_IO23 SAI2_RXD0
        { IMX_MAKE_PIN_1(4, 24), 0x424, 0x00001816, 0x1BC, 0x05 }, // GPIO4_IO24 SAI2_TXFS
        { IMX_MAKE_PIN_1(4, 25), 0x428, 0x00000016, 0x1C0, 0x05 }, // GPIO4_IO25 SAI2_TXC
        { IMX_MAKE_PIN_1(4, 26), 0x42C, 0x00000016, 0x1C4, 0x05 }, // GPIO4_IO26 SAI2_TXD0
        { IMX_MAKE_PIN_1(4, 27), 0x430, 0x00000016, 0x1C8, 0x05 }, // GPIO4_IO27 SAI2_MCLK
        { IMX_MAKE_PIN_1(4, 28), 0x434, 0x00000016, 0x1CC, 0x05 }, // GPIO4_IO28 SAI3_RXFS
        { IMX_MAKE_PIN_1(4, 29), 0x438, 0x00000016, 0x1D0, 0x05 }, // GPIO4_IO29 SAI3_RXC
        { IMX_MAKE_PIN_1(4, 30), 0x43C, 0x00000016, 0x1D4, 0x05 }, // GPIO4_IO30 SAI3_RXD
        { IMX_MAKE_PIN_1(4, 31), 0x440, 0x00000016, 0x1D8, 0x05 }, // GPIO4_IO31 SAI3_TXFS
        { IMX_MAKE_PIN_1(5, 0), 0x444, 0x00001816, 0x1DC, 0x05 }, // GPIO5_IO00 SAI3_TXC
        { IMX_MAKE_PIN_1(5, 1), 0x448, 0x00000016, 0x1E0, 0x05 }, // GPIO5_IO01 SAI3_TXD
        { IMX_MAKE_PIN_1(5, 2), 0x44C, 0x00000016, 0x1E4, 0x05 }, // GPIO5_IO02 SAI3_MCLK
        { IMX_MAKE_PIN_1(5, 3), 0x450, 0x00000016, 0x1E8, 0x05 }, // GPIO5_IO03 SPDIF_TX
        { IMX_MAKE_PIN_1(5, 4), 0x454, 0x00000016, 0x1EC, 0x05 }, // GPIO5_IO04 SPDIF_RX
        { IMX_MAKE_PIN_1(5, 5), 0x458, 0x00000016, 0x1F0, 0x05 }, // GPIO5_IO05 SPDIF_EXT_CLK
        { IMX_MAKE_PIN_1(5, 6), 0x45C, 0x00000016, 0x1F4, 0x05 }, // GPIO5_IO06 ECSPI1_SCLK
        { IMX_MAKE_PIN_1(5, 7), 0x460, 0x00000016, 0x1F8, 0x05 }, // GPIO5_IO07 ECSPI1_MOSI
        { IMX_MAKE_PIN_1(5, 8), 0x464, 0x00000016, 0x1FC, 0x05 }, // GPIO5_IO08 ECSPI1_MISO
        { IMX_MAKE_PIN_1(5, 9), 0x468, 0x00000016, 0x200, 0x05 }, // GPIO5_IO09 ECSPI1_SS0
        { IMX_MAKE_PIN_1(5, 10), 0x46C, 0x00001816, 0x204, 0x05 }, // GPIO5_IO10 ECSPI2_SCLK
        { IMX_MAKE_PIN_1(5, 11), 0x470, 0x00000016, 0x208, 0x05 }, // GPIO5_IO11 ECSPI2_MOSI
        { IMX_MAKE_PIN_1(5, 12), 0x474, 0x00000016, 0x20C, 0x05 }, // GPIO5_IO12 ECSPI2_MISO
        { IMX_MAKE_PIN_1(5, 13), 0x478, 0x00000016, 0x210, 0x05 }, // GPIO5_IO13 ECSPI2_SS0
        { IMX_MAKE_PIN_1(5, 14), 0x47C, 0x00000016, 0x214, 0x05 }, // GPIO5_IO14 I2C1_SCL
        { IMX_MAKE_PIN_1(5, 15), 0x480, 0x00000016, 0x218, 0x05 }, // GPIO5_IO15 I2C1_SDA
        { IMX_MAKE_PIN_1(5, 16), 0x484, 0x00000016, 0x21C, 0x05 }, // GPIO5_IO16 I2C2_SCL
        { IMX_MAKE_PIN_1(5, 17), 0x488, 0x00000016, 0x220, 0x05 }, // GPIO5_IO17 I2C2_SDA
        { IMX_MAKE_PIN_1(5, 18), 0x48C, 0x00001816, 0x224, 0x05 }, // GPIO5_IO18 I2C3_SCL
        { IMX_MAKE_PIN_1(5, 19), 0x490, 0x00000016, 0x228, 0x05 }, // GPIO5_IO19 I2C3_SDA
        { IMX_MAKE_PIN_1(5, 20), 0x494, 0x00000016, 0x22C, 0x05 }, // GPIO5_IO20 I2C4_SCL
        { IMX_MAKE_PIN_1(5, 21), 0x498, 0x00000016, 0x230, 0x05 }, // GPIO5_IO21 I2C4_SDA
        { IMX_MAKE_PIN_1(5, 22), 0x49C, 0x00000016, 0x234, 0x05 }, // GPIO5_IO22 UART1_RXD
        { IMX_MAKE_PIN_1(5, 23), 0x4A0, 0x00000016, 0x238, 0x05 }, // GPIO5_IO23 UART1_TXD
        { IMX_MAKE_PIN_1(5, 24), 0x4A4, 0x00000016, 0x23C, 0x05 }, // GPIO5_IO24 UART2_RXD
        { IMX_MAKE_PIN_1(5, 25), 0x4A8, 0x00000016, 0x240, 0x05 }, // GPIO5_IO25 UART2_TXD
        { IMX_MAKE_PIN_1(5, 26), 0x4AC, 0x00001816, 0x244, 0x05 }, // GPIO5_IO26 UART3_RXD
        { IMX_MAKE_PIN_1(5, 27), 0x4B0, 0x00000016, 0x248, 0x05 }, // GPIO5_IO27 UART3_TXD
        { IMX_MAKE_PIN_1(5, 28), 0x4B4, 0x00000016, 0x24C, 0x05 }, // GPIO5_IO28 UART4_RXD
        { IMX_MAKE_PIN_1(5, 29), 0x4B8, 0x00000016, 0x250, 0x05 }, // GPIO5_IO29 UART4_TXD
};

    // i.MX8 SOC specific Select Input/Alt Function/Pin map
    //
    // iMX8M
    const IMX_PIN_INPUT_DATA Imx8MGpioPinInputSelectTable[] = {
        //--------------------------------------------------
        // Pin Absolute Number | Pin Alt Mode value | Select Input Reg Offset | Input Select value
        //--------------------------------------------------
        { IMX_MAKE_PIN_1(3, 22), 2, 0x4CC, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD1 - ALT2 - SAI1_TX_SYNC - IOMUXC_SAI1_TX_SYNC_SELECT_INPUT
        { IMX_MAKE_PIN_1(3, 22), 3, 0x4EC, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD1 - ALT3 - SAI5_TX_SYNC - IOMUXC_SAI5_TX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(3, 23), 2, 0x4CC, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD2 - ALT2 - SAI1_TX_SYNC - IOMUXC_SAI1_TX_SYNC_SELECT_INPUT
        { IMX_MAKE_PIN_1(3, 23), 3, 0x4E8, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD2 - ALT3 - SAI5_TX_BCLK - IOMUXC_SAI5_TX_BCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(3, 24), 2, 0x4CC, 2 }, // IOMUXC_SW_MUX_CTL_PAD_SAI5_RXD3 - ALT2 - SAI1_TX_SYNC - IOMUXC_SAI1_TX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(3, 25), 0, 0x52C, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI5_MCLK - ALT0 - SAI5_MCLK - IOMUXC_SAI5_MCLK_SELECT_INPUT
        { IMX_MAKE_PIN_1(3, 25), 1, 0x4C8, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI5_MCLK - ALT1 - SAI1_TX_BCLK - IOMUXC_SAI1_TX_BCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 0), 0, 0x4C4, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_RXFS - ALT0 - SAI1_RX_SYNC - IOMUXC_SAI1_RX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 6), 1, 0x51C, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD4 - ALT1 - SAI6_TX_BCLK - IOMUXC_SAI6_TX_BCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 7), 3, 0x4C4, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD5 - ALT3 - SAI1_RX_SYNC - IOMUXC_SAI1_RX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 8), 1, 0x520, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD6 - ALT1 - SAI6_TX_SYNC - IOMUXC_SAI6_TX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 9), 1, 0x530, 0 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD7 - ALT1 - SAI6_MCLK - IOMUXC_SAI6_MCLK_SELECT_INPUT
        { IMX_MAKE_PIN_1(4, 9), 2, 0x4CC, 4 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_RXD7 - ALT2 - SAI1_TX_SYNC - IOMUXC_SAI1_TX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 10), 0, 0x4CC, 3 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_TXFS - ALT0 - SAI1_TX_SYNC - IOMUXC_SAI1_TX_SYNC_SELECT_INPUT
        { IMX_MAKE_PIN_1(4, 10), 1, 0x4EC, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_TXFS - ALT1 - SAI5_TX_SYNC - IOMUXC_SAI5_TX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 11), 0, 0x4C8, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_TXC - ALT0 - SAI1_TX_BCLK - IOMUXC_SAI1_TX_BCLK_SELECT_INPUT
        { IMX_MAKE_PIN_1(4, 11), 1, 0x4E8, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_TXC - ALT1 - SAI5_TX_BCLK - IOMUXC_SAI5_TX_BCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 16), 2, 0x51C, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD4 - ALT2 - SAI6_TX_BCLK - IOMUXC_SAI6_TX_BCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 18), 2, 0x520, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD6 - ALT2 - SAI6_TX_SYNC - IOMUXC_SAI6_TX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 19), 1, 0x530, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_TXD7 - ALT1 - SAI6_MCLK - IOMUXC_SAI6_MCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 20), 1, 0x52C, 1 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_MCLK - ALT1 - SAI5_MCLK - IOMUXC_SAI5_MCLK_SELECT_INPUT
        { IMX_MAKE_PIN_1(4, 20), 2, 0x4C8, 2 }, // IOMUXC_SW_MUX_CTL_PAD_SAI1_MCLK - ALT2 - SAI1_TX_BCLK - IOMUXC_SAI1_TX_BCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 21), 1, 0x4EC, 2 }, // IOMUXC_SW_MUX_CTL_PAD_SAI2_RXFS - ALT1 - SAI5_TX_SYNC - IOMUXC_SAI5_TX_SYNC_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 22), 1, 0x4E8, 2 }, // IOMUXC_SW_MUX_CTL_PAD_SAI2_RXC - ALT1 - SAI5_TX_BCLK - IOMUXC_SAI5_TX_BCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(4, 27), 1, 0x52C, 2 }, // IOMUXC_SW_MUX_CTL_PAD_SAI2_MCLK - ALT1 - SAI5_MCLK - IOMUXC_SAI5_MCLK_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 2), 2, 0x52C, 3 }, // IOMUXC_SW_MUX_CTL_PAD_SAI3_MCLK - ALT2 - SAI5_MCLK - IOMUXC_SAI5_MCLK_SELECT_INPUT

//        { IMX_MAKE_PIN_1(5, 6), 1, 0x504, 0 }, // IOMUXC_SW_MUX_CTL_PAD_ECSPI1_SCLK - ALT1 - UART3_RX - IOMUXC_UART3_RXD_SELECT_INPUT

//        { IMX_MAKE_PIN_1(5, 7), 1, 0x504, 1 }, // IOMUXC_SW_MUX_CTL_PAD_ECSPI1_MOSI - ALT1 - UART3_TX - IOMUXC_UART3_RXD_SELECT_INPUT

//        { IMX_MAKE_PIN_1(5, 8), 1, 0x500, 0 }, // IOMUXC_SW_MUX_CTL_PAD_ECSPI1_MISO - ALT1 - UART3_CTS_B - IOMUXC_UART3_RTS_B_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 9), 1, 0x500, 1 }, // IOMUXC_SW_MUX_CTL_PAD_ECSPI1_SS0 - ALT1 - UART3_RTS_B - IOMUXC_UART3_RTS_B_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 10), 1, 0x50C, 0 }, // IOMUXC_SW_MUX_CTL_PAD_ECSPI2_SCLK - ALT1 - UART4_RX - IOMUXC_UART4_RXD_SELECT_INPUT

//        { IMX_MAKE_PIN_1(5, 11), 1, 0x50C, 1 }, // IOMUXC_SW_MUX_CTL_PAD_ECSPI2_MOSI - ALT1 - UART4_TX - IOMUXC_UART4_RXD_SELECT_INPUT

//        { IMX_MAKE_PIN_1(5, 12), 1, 0x508, 0 }, // IOMUXC_SW_MUX_CTL_PAD_ECSPI2_MISO - ALT1 - UART4_CTS_B - IOMUXC_UART4_RTS_B_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 13), 1, 0x508, 1 }, // IOMUXC_SW_MUX_CTL_PAD_ECSPI2_SS0 - ALT1 - UART4_RTS_B - IOMUXC_UART4_RTS_B_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 20), 2, 0x524, 0 }, // IOMUXC_SW_MUX_CTL_PAD_I2C4_SCL - ALT2 - PCIE1_CLKREQ_B - IOMUXC_PCIE1_CLKREQ_B_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 21), 2, 0x528, 0 }, // IOMUXC_SW_MUX_CTL_PAD_I2C4_SDA - ALT2 - PCIE2_CLKREQ_B - IOMUXC_PCIE2_CLKREQ_B_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 22), 0, 0x4F4, 0 }, // IOMUXC_SW_MUX_CTL_PAD_UART1_RXD - ALT0 - UART1_RX - IOMUXC_UART1_RXD_SELECT_INPUT

//        { IMX_MAKE_PIN_1(5, 23), 0, 0x4F4, 1 }, // IOMUXC_SW_MUX_CTL_PAD_UART1_TXD - ALT0 - UART1_TX - IOMUXC_UART1_RXD_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 24), 0, 0x4FC, 0 }, // IOMUXC_SW_MUX_CTL_PAD_UART2_RXD - ALT0 - UART2_RX - IOMUXC_UART2_RXD_SELECT_INPUT

//        { IMX_MAKE_PIN_1(5, 25), 0, 0x4FC, 1 }, // IOMUXC_SW_MUX_CTL_PAD_UART2_TXD - ALT0 - UART2_TX - IOMUXC_UART2_RXD_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 26), 0, 0x504, 2 }, // IOMUXC_SW_MUX_CTL_PAD_UART3_RXD - ALT0 - UART3_RX - IOMUXC_UART3_RXD_SELECT_INPUT
        { IMX_MAKE_PIN_1(5, 26), 1, 0x4F0, 0 }, // IOMUXC_SW_MUX_CTL_PAD_UART3_RXD - ALT1 - UART1_CTS_B - IOMUXC_UART1_RTS_B_SELECT_INPUT

//        { IMX_MAKE_PIN_1(5, 27), 0, 0x504, 3 }, // IOMUXC_SW_MUX_CTL_PAD_UART3_TXD - ALT0 - UART3_TX - IOMUXC_UART3_RXD_SELECT_INPUT
        { IMX_MAKE_PIN_1(5, 27), 1, 0x4F0, 1 }, // IOMUXC_SW_MUX_CTL_PAD_UART3_TXD - ALT1 - UART1_RTS_B - IOMUXC_UART1_RTS_B_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 28), 0, 0x50C, 2 }, // IOMUXC_SW_MUX_CTL_PAD_UART4_RXD - ALT0 - UART4_RX - IOMUXC_UART4_RXD_SELECT_INPUT
//        { IMX_MAKE_PIN_1(5, 28), 1, 0x4F8, 0 }, // IOMUXC_SW_MUX_CTL_PAD_UART4_RXD - ALT1 - UART2_CTS_B - IOMUXC_UART2_RTS_B_SELECT_INPUT
        { IMX_MAKE_PIN_1(5, 28), 2, 0x524, 1 }, // IOMUXC_SW_MUX_CTL_PAD_UART4_RXD - ALT2 - PCIE1_CLKREQ_B - IOMUXC_PCIE1_CLKREQ_B_SELECT_INPUT

        { IMX_MAKE_PIN_1(5, 29), 1, 0x4F8, 1 }, // IOMUXC_SW_MUX_CTL_PAD_UART4_TXD - ALT1 - UART2_RTS_B - IOMUXC_UART2_RTS_B_SELECT_INPUT
        { IMX_MAKE_PIN_1(5, 29), 2, 0x528, 1 }, // IOMUXC_SW_MUX_CTL_PAD_UART4_TXD - ALT2 - PCIE2_CLKREQ_B - IOMUXC_PCIE2_CLKREQ_B_SELECT_INPUT

    };
} // namespace

IMX_NONPAGED_SEGMENT_END; //====================================================