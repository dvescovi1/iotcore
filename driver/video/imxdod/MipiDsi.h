// Copyright (c) Microsoft Corporation. All rights reserved.
// Copyright 2019 NXP
// Licensed under the MIT License.

#ifndef _MIPIDSI_H_
#define _MIPIDSI_H_

#define MIPI_DSI_REGISTERS_LENGTH              0x10000

// MIPI DSI Registers
#define IMX_MIPI_DSI_OFFSET_VERSION            0x0000
#define IMX_MIPI_DSI_OFFSET_STATUS             0x0004
#define IMX_MIPI_DSI_OFFSET_RGB_STATUS         0x0008
#define IMX_MIPI_DSI_OFFSET_SWRST              0x000C
#define IMX_MIPI_DSI_OFFSET_CLKCTRL            0x0010
#define IMX_MIPI_DSI_OFFSET_TIMEOUT            0x0014
#define IMX_MIPI_DSI_OFFSET_CONFIG             0x0018
#define IMX_MIPI_DSI_OFFSET_ESCMODE            0x001C
#define IMX_MIPI_DSI_OFFSET_MDRESOL            0x0020
#define IMX_MIPI_DSI_OFFSET_MVPORCH            0x0024
#define IMX_MIPI_DSI_OFFSET_HPORCH             0x0028
#define IMX_MIPI_DSI_OFFSET_MSYNC              0x002C
#define IMX_MIPI_DSI_OFFSET_SDRESOL            0x0030
#define IMX_MIPI_DSI_OFFSET_INTSRC             0x0034
#define IMX_MIPI_DSI_OFFSET_INTMSK             0x0038
#define IMX_MIPI_DSI_OFFSET_PKTHDR             0x003C
#define IMX_MIPI_DSI_OFFSET_PAYLOAD            0x0040
#define IMX_MIPI_DSI_OFFSET_RXFIFO             0x0044
#define IMX_MIPI_DSI_OFFSET_FIFOTHLD           0x0048
#define IMX_MIPI_DSI_OFFSET_FIFOCTRL           0x004C
#define IMX_MIPI_DSI_OFFSET_MEMACCHR           0x0050
#define IMX_MIPI_DSI_OFFSET_MULTI_PKT          0x0078
#define IMX_MIPI_DSI_OFFSET_PLLCTRL_1G         0x0090
#define IMX_MIPI_DSI_OFFSET_PLLCTRL            0x0094
#define IMX_MIPI_DSI_OFFSET_PLLCTRL1           0x0098
#define IMX_MIPI_DSI_OFFSET_PLLCTRL2           0x009C
#define IMX_MIPI_DSI_OFFSET_PLLTMR             0x00A0
#define IMX_MIPI_DSI_OFFSET_PHYCTRL_B1         0x00A4
#define IMX_MIPI_DSI_OFFSET_PHYCTRL_B2         0x00A8
#define IMX_MIPI_DSI_OFFSET_PHYCTRL_M1         0x00AC
#define IMX_MIPI_DSI_OFFSET_PHYCTRL_M2         0x00B0
#define IMX_MIPI_DSI_OFFSET_PHYTIMING          0x00B4
#define IMX_MIPI_DSI_OFFSET_PHYTIMING1         0x00B8
#define IMX_MIPI_DSI_OFFSET_PHYTIMING2         0x00BC


#define IMX_MIPI_DSI_MDRESOL_MAIN_STANDBY      (0x01 << 31)

#define IMX_MIPI_DSI_SWRST_FUNCT_RST           (0x01 << 16)

#endif