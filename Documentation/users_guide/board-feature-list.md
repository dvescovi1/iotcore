Feature List per Board {#feature-list}
=======

Table: Overview of the currently supported features for every board.

| Feature      | MCIMX6Q-SDB/SDP |   MCIMX6QP-SDB   |  MCIMX6DL-SDP    |  MCIMX6SX-SDB    |
|:-------------|:---------------:|:----------------:|:----------------:|:----------------:|
| BSP name     | Sabre_iMX6Q_1GB | Sabre_iMX6QP_1GB | Sabre_iMX6DL_1GB | Sabre_iMX6SX_1GB |
| SD Card boot |        Y        |        Y         |        Y         |        Y         |
| eMMC boot    |        Y        |        Y         |        Y         |        N*        |
| Audio        |        Y        |        Y         |        Y         |        Y         |
| GPIO         |        Y        |        Y         |        Y         |        Y         |
| I2C          |        Y        |        Y         |        Y         |        Y         |
| Ethernet     |        Y        |        Y         |        Y         |        Y         |
| PWM          |        Y        |        Y         |        Y         |        Y         |
| SD Card      |        Y        |        Y         |        Y         |        Y         |
| eMMC         |        Y        |        Y         |        Y         |        N*        |
| SPI (master) |        Y        |        Y         |        Y         |        Y         |
| Display      |        Y        |        Y         |        Y         |        Y         |
| UART         |        Y*       |        Y*        |        Y*        |        Y*        |
| USB (host)   |        Y        |        Y         |        Y         |        Y         |
| PCIe         |        Y        |        Y         |        Y         |        Y         |
| TrEE         |        Y        |        Y         |        Y         |        Y         |
| M4           |       N/A       |       N/A        |       N/A        |       N**        |
| Processor PM |        Y        |        Y         |        Y         |        Y         |
| Device PM    |        Y        |        Y         |       N**        |       N**        |
| LP standby   |       N**       |       N**        |       N**        |       N**        |
| Display PM   |        Y        |        Y         |        Y         |        Y         |
| DMA          |        Y        |        Y         |        Y         |        Y         |

| Feature      |   MCIMX6UL-EVK   |   MCIMX6ULL-EVK   |   MCIMX7SABRE   |   MCIMX8M-EVK     |     8MMINILPD4-EVK    |  8MNANOD4-EVK  |
|:-------------|:----------------:|:-----------------:|:---------------:|:-----------------:|:---------------------:|:--------------:|
| BSP name     | EVK_iMX6UL_512MB | EVK_iMX6ULL_512MB | Sabre_iMX7D_1GB | NXPEVK_IMX8M_4GB  | NXPEVK_IMX8M_Mini_2GB | EVK_IMX8MM_2GB |
| SD Card boot |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| eMMC boot    |        N*        |        N*         |        Y        |         Y         |          Y            |       Y        |
| Audio        |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| GPIO         |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| I2C          |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| Ethernet     |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| PWM          |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| SD Card      |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| eMMC         |        N*        |        N*         |        Y        |         Y         |          Y            |       Y        |
| SPI (master) |        N*        |        N*         |        Y        |        N/A        |          Y            |       Y        |
| Display      |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| UART         |        Y*        |         Y*        |        Y*       |         Y*        |          Y*           |       Y*       |
| USB (host)   |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| PCIe         |       N/A        |        N/A        |        Y        |         Y         |          Y            |       Y        |
| TrEE         |        Y         |         N         |        Y        |         Y         |          Y            |       Y        |
| M4           |       N/A        |        N/A        |       N**       |        N**        |         N**           |      N**       |
| Processor PM |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| Device PM    |       N**        |        N**        |       N**       |        N**        |         N**           |      N**       |
| LP standby   |       N**        |        N**        |       N**       |        N**        |         N**           |      N**       |
| Display PM   |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| DMA          |        Y         |         Y         |        Y        |         Y         |          Y            |       Y        |
| VPU          |       N/A        |        N/A        |       N/A       |         Y         |          Y            |        N/A     |


| Legend | Meaning                                                                       |
|:-------|:------------------------------------------------------------------------------|
| Y      | Enabled                                                                       |
| Y*     | To enable the UART, the kernel debugger must be disabled by running the following command on the device and rebooting. The UART exposed is the same UART that the kernel debugger uses. `bcdedit /debug off` |
| N/A    | Feature not applicable                                                        |
| N*     | Feature not enabled - feature is not available in default board configuration |
| N**    | Feature not enabled - feature is not currently supported                      |
| PM     | Power management                                                              |
| LP     | Low power                                                                     |

Not all features of a given subsystem maybe fully enabled and/or optimized. If you encounter issues with supported features, please open an issue.
