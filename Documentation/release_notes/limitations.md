Known Issues/Limitations
====

Read through all hardware-related reference material and ensure that you have made all the necessary hardware modifications before using the software.

The imx-iotcore reference BSP has the following limitations:
 - SD/eMMC is the only supported boot media


Table: Common known issues and workarounds for i.MX Family SoC

| SoC            | Module  | Source      | Description                   | Workaround                 |
|:---------------|:--------|:------------|:------------------------------|:---------------------------|
| All SoC                                | Boot                  | Software    | In case multiple SD cards marked as bootable are inserted before power on target might fail to boot. | It is recommended to newly format the SD card using Diskpart. |
| All SoC                                | UART                  | Software    | Command line option dtr=off shall be used while opening UART device via MinComm.exe. Otherwise UART device cannot be opened.                    | No workaround                                 |
| All SoC                                | USB                   | Software    | Only USB HOST mode is supported.                                                                                                                | No workaround                                 |
| All SoC                                | MU Platform build     | Software    | The MuEnvironment python module imported by `PlatformBuild.py` is not fully supported. This results in `PlatformBuild.py --setup` command to fail. | Released BSP doesn't require `PlatformBuild.py --setup` to be run. If the command is required to be executed it is needed to setup a git repository in MSRSec or in its parent directory. The MSRSec repository can be obtained from Github. |
| All SoC                                | VS2017 build          | Software    | Build of iMXPlatform.sln fails if there are spaces in project path. | Place the imx-iotcore directory in way its path doesn't contain spaces. |
| All SoC                                | WSL build             | Software    | OP-TEE Trusted OS couldn't be built inside WSL environment if located in Windows file system. | Move sources to WSL root drive eg $HOME. |
| i.MX 6Q, 6QP, 6DL, 6SX                 | UEFI                  | Software    | USB stack is not enabled in UEFI by default. | Download PciEmulation.c and PciEmulation.inf files from imx-edk2-platforms MSFT github and enable USB stack by set CONFIG_USB symbol to TRUE value in *.dsc file |
| i.MX 6DL                               | WinPe                 | Software    | Kernel crash can be observed after SW restart from WinPE. | HW restart is needed to suppress this behaviour. |
| i.MX 6UL, 6ULL, 6SX, 6DL               | PEP                   | Software    | PEP driver is not implemented, the power management may not be fully functional.                                                                | No workaround                                 |
| i.MX 6SX, 6UL, 6ULL                    | IMXDOD                | Software    | Display back-light is not turned off after switch to PowerDeviceD3 mode (parallel LCD display should be "frozen", LCD signals stop to generate) | No workaround                                 |
| i.MX 7D                                | IMXDOD                | Software    | Display back-light is not turned off after switch to PowerDeviceD3 mode (parallel LCD display should be "frozen", LCD signals stop to generate) | No workaround                                 |
| i.MX 8                                 | eMMC Boot             | Software    | eMMC boot on ARM64. flash.bin is not deployed from WinPE onto eMMC device thus execution of bootloader from SD card is still needed when booting from eMMC. | No workaround                                 |
| i.MX 8                                 | FFU deployment        | Software    | Deploying FFU file to SD card using IotCoreDashboard application causes early restart of the device resulting in improper initialization when Device name is changed in IotCoreDashboard. | Do not change Device name in IotCoreDashboard application or use Dism.exe to burn FFU image. |
| i.MX 8MM, 8MN                          | USB                   | Software    | USB Type-C port driver (imxUcmTcpciCxClient) is not implemented yet. Port properties are detected/configured only in UBOOT and are not enumerated when OS is running. | USB cable must be plugged before U-Boot is running. |
| i.MX 8MM, 8MN                          | Display               | Software    | Display driver supports 1920 x 1080 60 Hz resolution only.                                                                                                  | No workaround                                 |
| i.MX 8MQ                               | Display               | Software    | Display driver supports 1280 x 720 60 Hz resolution only.                                                                                                   | No workaround                                 |
| i.MX 8MQ, 8MM                          | VPU                   | Software    | Because a GPU driver is not available yet, operations on GPU, such as scaling, resize and other transformation are not HW supported. These operations are computed by CPU and consumes enormous piece of computation time. Interlaced videos needs to be scaled inside the VPU driver, thus interlaced videos may be laggy and cause high CPU load. | No workaround for interlaced videos. For progressive videos, deny scaling in player.  |
