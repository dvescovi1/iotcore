Basic Board Setup {#boot-switch}
=======

| Feature              |   MCIMX6Q-SDB/SDP      |      MCIMX6QP-SDB      |     MCIMX6DL-SDP       |
|:---------------------|:----------------------:|:----------------------:|:----------------------:|
| BSP name             | Sabre_iMX6Q_1GB        | Sabre_iMX6QP_1GB       | Sabre_iMX6DL_1GB       |
| Debug UART*          | J509                   |  J509                  | J509                   |
| Default display      | J8 (HDMI)***           |  J8 (HDMI)***          | J8 (HDMI)***           |
| SD card boot slot    | J507 (SD3)             |  J507 (SD3)            | J507 (SD3)             |
| SD card boot DIP cfg | ![DIP switch configuration](images/boards/imx6q_bs_sd.png "DIP switch configuration"){width=3cm} | ![DIP switch configuration](images/boards/imx6q_bs_sd.png "DIP switch configuration"){width=3cm} | ![DIP switch configuration](images/boards/imx6q_bs_sd.png "DIP switch configuration"){width=3cm} |
| eMMC boot DIP cfg    | ![DIP switch configuration](images/boards/imx6q_bs_emmc.png "DIP switch configuration"){width=3cm} | ![DIP switch configuration](images/boards/imx6q_bs_emmc.png "DIP switch configuration"){width=3cm} | ![DIP switch configuration](images/boards/imx6q_bs_emmc.png "DIP switch configuration"){width=3cm} |

| Feature              |     MCIMX6SX-SDB       |    MCIMX7SABRE       |
|:---------------------|:----------------------:|:--------------------:|
| BSP name             | Sabre_iMX6SX_1GB       | Sabre_iMX7D_1GB      |
| Debug UART*          | J16                    | J11                  |
| Default display      | J12 (LVDS)             | J9 (HDMI)            |
| SD card boot slot    | J4 (SD4)               | J6 (SD1)             |
| SD card boot DIP cfg | ![DIP switch configuration](images/boards/imx6sx_bs.png "DIP switch configuration"){width=6cm} | ![DIP switch configuration](images/boards/imx7d_bs.png "DIP switch configuration"){width=4cm} |
| eMMC boot DIP cfg    | N/A                    | N/A                  |


| Feature              |    MCIMX6UL-EVK      |   MCIMX6ULL-EVK      |
|:---------------------|:--------------------:|:--------------------:|
| BSP name             | EVK_iMX6UL_512MB     | EVK_iMX6ULL_512MB    |
| Debug UART*          | J1901                | J1901                |
| Default display      | J901 (LCDIF)         | J901 (LCDIF)         |
| SD card boot slot    | J301**               | J301**               |
| SD card boot DIP cfg | ![DIP switch configuration](images/boards/imx6ull_bs.png "DIP switch configuration"){width=3cm}| ![DIP switch configuration](images/boards/imx6ull_bs.png "DIP switch configuration"){width=3cm} |
| eMMC boot DIP cfg    | N/A                  | N/A                  |

| Feature              |    MCIMX8M-EVK       |   8MMINILPD4-EVK      |   8MNANOD4-EVK        |
|:---------------------|:--------------------:|:---------------------:|:---------------------:|
| BSP name             | NXPEVK_IMX8M_4GB     | NXPEVK_IMX8M_Mini_2GB | EVK_IMX8MN_2GB        |
| Debug UART*          | J1701                | J901                  | J901                  |
| Default display      | J1001 (HDMI)         | J801 (MIPI DSI)       | J801 (MIPI DSI)       |
| SD card boot slot    | J1601**              | J701**                | J701**                |
| SD card boot DIP cfg | ![DIP switch configuration](images/boards/imx8mq_bs_sd.png "DIP switch configuration"){width=3cm}| ![DIP switch configuration](images/boards/imx8mm_bs_sd.png "DIP switch configuration"){width=3cm}| ![DIP switch configuration](images/boards/imx8mm_bs_sd.png "DIP switch configuration"){width=3cm}|
| eMMC boot DIP cfg    |       N/A            |        N/A            |        N/A            |

| Legend | Meaning                                                                       |
|:-------|:------------------------------------------------------------------------------|
| *      | Serial port configuration: 115200 baud, 8 data bits, 1 stop bit, no parity.   |
| **     | MicroSD card slot                                                             |
| ***    | Updated UEFI image needed to have LVDS as default display output. Please set TRUE for "PcdLvdsEnable" in *.dsc file. Section [PcdsFeatureFlag.common]. |
