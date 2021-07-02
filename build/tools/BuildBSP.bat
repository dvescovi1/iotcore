:: Copyright (c) Microsoft Corporation. All rights reserved.
:: Licensed under the MIT License.

@echo off
setlocal enableextensions disabledelayedexpansion

pushd %~dp0\..\..

if "%1"=="" (
    echo "Missing SOC argument to BuildBSP.bat"
    echo "Usage: BuildBSP.bat <SOC> <boardname>"
    echo "eg: BuildBSP.bat iMX6 HummingBoardEdge_iMX6Q_2GB"
    exit /b 1
)

if "%2"=="" (
    echo "Missing boardname argument to BuildBSP.bat"
    echo "Usage: BuildBSP.bat <SOC> <boardname>"
    echo "eg: BuildBSP.bat iMX6 HummingBoardEdge_iMX6Q_2GB"
    exit /b 1
)

if "%Configuration%"=="" (
    echo configuration environment variable is not set
    exit /b 1
)

if "%PLATFORM%"=="" (
    echo platform environment variable is not set
    exit /b 1
)

set SOC=%1
set BOARDNAME=%2
set REPO_ROOT=%cd%
set BSP_ROOT=%cd%\BSP\%BOARDNAME%
set PKG_ROOT=%BSP_ROOT%\Packages
set BUILD_ROOT=%REPO_ROOT%\build\solution\iotPlatform\Build\%PLATFORM%\%Configuration%
set INFO_ROOT=%REPO_ROOT%\build\board\%BOARDNAME%\Package
set FAILURE=

echo BSP_ROOT is %BSP_ROOT%
echo BUILD_ROOT is %BUILD_ROOT%
echo PKG_ROOT is %PKG_ROOT%
echo REPO_ROOT is %REPO_ROOT%
echo OutDir is %OutDir%

mkdir %BSP_ROOT%\OEMInputSamples 2>NUL
mkdir %BSP_ROOT%\Packages 2>NUL
copy build\board\%BOARDNAME%\%BOARDNAME%_TestOEMInput.xml %BSP_ROOT%\OEMInputSamples\TestOEMInput.xml >NUL
copy build\board\%BOARDNAME%\%BOARDNAME%_ProductionOEMInput.xml %BSP_ROOT%\OEMInputSamples\RetailOEMInput.xml >NUL
copy build\board\%BOARDNAME%\%BOARDNAME%_FMFileList.xml %BSP_ROOT%\Packages\%BOARDNAME%FMFileList.xml >NUL
copy build\board\%BOARDNAME%\InputFMs\%BOARDNAME%_DeviceFM.xml %BSP_ROOT%\Packages >NUL

if "%SOC%"=="iMX6" (
    goto ARM32
)

if "%SOC%"=="iMX8" (
    goto ARM64
)

if "%SOC%"=="BCM2711" (
    goto ARM64
)

echo Unrecognized SOC type: %SOC%
exit /b 1

:ARM32
echo Building ARM32 BSP

:: Copy Driver Packages
mkdir %PKG_ROOT%\USDHC >NUL 2>NUL
copy %BUILD_ROOT%\imxusdhc\* %PKG_ROOT%\USDHC\ >NUL
if errorlevel 1 (set FAILURE=imxusdhc & goto ERROR)
copy %REPO_ROOT%\driver\sd\imxusdhc\imxusdhc.wm.xml %PKG_ROOT%\USDHC\ >NUL

:: Copy Firmware Packages
echo Copying Firmware Packages to %PKG_ROOT%
mkdir %PKG_ROOT%\BootFirmware\BootFirmware >NUL 2>NUL
copy %INFO_ROOT%\BootFirmware\BootFirmware.wm.xml %PKG_ROOT%\BootFirmware\ >NUL
copy %INFO_ROOT%\BootFirmware\uefi.fit %PKG_ROOT%\BootFirmware\BootFirmware\ >NUL

mkdir %PKG_ROOT%\BootLoader\BootLoader >NUL 2>NUL
copy %INFO_ROOT%\BootLoader\BootLoader.wm.xml %PKG_ROOT%\BootLoader\ >NUL
copy %INFO_ROOT%\BootLoader\firmware_fit.merged %PKG_ROOT%\BootLoader\BootLoader\ >NUL

:: Copy the UpdateOS Package XML
mkdir %PKG_ROOT%\SVPlatExtensions >NUL 2>NUL
copy %INFO_ROOT%\SVPlatExtensions\BSP.svupdateOS.wm.xml %PKG_ROOT%\SVPlatExtensions\svupdateOS.wm.xml >NUL

:: Copy Platform Information Packages
echo Copying Platform Information Packages to %PKG_ROOT%
mkdir %PKG_ROOT%\SystemInformation >NUL 2>NUL
copy %INFO_ROOT%\SystemInformation\* %PKG_ROOT%\SystemInformation\ >NUL

mkdir %PKG_ROOT%\DevicePlatform\OEMDevicePlatform >NUL 2>NUL
mkdir %PKG_ROOT%\DevicePlatform\OEMDevicePlatformMin2GB >NUL 2>NUL
copy %INFO_ROOT%\OEMDevicePlatform\OEMDevicePlatform.wm.xml %PKG_ROOT%\DevicePlatform\ >NUL
copy %INFO_ROOT%\OEMDevicePlatform\DevicePlatform.xml %PKG_ROOT%\DevicePlatform\OEMDevicePlatform >NUL
copy %INFO_ROOT%\OEMDevicePlatformMin2GB\OEMDevicePlatformMin2GB.wm.xml %PKG_ROOT%\DevicePlatform\ >NUL
copy %INFO_ROOT%\OEMDevicePlatformMin2GB\DevicePlatform.xml %PKG_ROOT%\DevicePlatform\OEMDevicePlatformMin2GB >NUL

mkdir %PKG_ROOT%\DeviceLayout\DeviceLayoutProd >NUL 2>NUL
mkdir %PKG_ROOT%\DeviceLayout\DeviceLayoutProdMin2GB >NUL 2>NUL
echo %INFO_ROOT%\DeviceLayoutProd\DeviceLayoutProd.wm.xml %PKG_ROOT%\DeviceLayout\
copy %INFO_ROOT%\DeviceLayoutProd\DeviceLayoutProd.wm.xml %PKG_ROOT%\DeviceLayout\ >NUL
echo %INFO_ROOT%\DeviceLayoutProd\devicelayoutprod.xml %PKG_ROOT%\DeviceLayout\DeviceLayoutProd
copy %INFO_ROOT%\DeviceLayoutProd\devicelayoutprod.xml %PKG_ROOT%\DeviceLayout\DeviceLayoutProd >NUL
copy %INFO_ROOT%\DeviceLayoutProdMin2GB\DeviceLayoutProdMin2GB.wm.xml %PKG_ROOT%\DeviceLayout\ >NUL
copy %INFO_ROOT%\DeviceLayoutProdMin2GB\devicelayoutprodmin2gb.xml %PKG_ROOT%\DeviceLayout\DeviceLayoutProdMin2GB >NUL

exit /b 0

:ARM64

:: Copy Driver Packages
mkdir %PKG_ROOT%\USDHC >NUL 2>NUL
copy %BUILD_ROOT%\imxusdhc\* %PKG_ROOT%\USDHC\ >NUL
if errorlevel 1 (set FAILURE=imxusdhc & goto ERROR)
copy %REPO_ROOT%\driver\sd\imxusdhc\imxusdhc.wm.xml %PKG_ROOT%\USDHC\ >NUL

mkdir %PKG_ROOT%\GPIO >NUL 2>NUL
copy %BUILD_ROOT%\bcmgpio\* %PKG_ROOT%\GPIO\ >NUL
if errorlevel 1 (set FAILURE=bcmgpio & goto ERROR)
copy %REPO_ROOT%\driver\gpio\bcmgpio\bcmgpio.wm.xml %PKG_ROOT%\GPIO\ >NUL

:: Copy Components
echo Copying components to %PKG_ROOT%
mkdir %PKG_ROOT%\Arm64CrtRuntime >NUL 2>NUL
copy %BUILD_ROOT%\Arm64CrtRuntime\* %PKG_ROOT%\Arm64CrtRuntime\ >NUL
if errorlevel 1 (set FAILURE=Arm64CrtRuntime & goto ERROR)
copy %REPO_ROOT%\components\Arm64CrtRuntime\Arm64CrtRuntime.wm.xml %PKG_ROOT%\Arm64CrtRuntime\ >NUL

mkdir %PKG_ROOT%\Arm64DefaultApp >NUL 2>NUL
copy %REPO_ROOT%\components\Arm64DefaultApp\Packages\* %PKG_ROOT%\Arm64DefaultApp\ >NUL
if errorlevel 1 (set FAILURE=Arm64DefaultApp & goto ERROR)

:: Copy Firmware Packages
echo Copying Firmware Packages to %PKG_ROOT%
mkdir %PKG_ROOT%\BootFirmware\BootFirmware >NUL 2>NUL
copy %INFO_ROOT%\BootFirmware\BootFirmware.wm.xml %PKG_ROOT%\BootFirmware\ >NUL
copy %INFO_ROOT%\BootFirmware\uefi.fit %PKG_ROOT%\BootFirmware\BootFirmware\ >NUL

mkdir %PKG_ROOT%\BootLoader\BootLoader >NUL 2>NUL
copy %INFO_ROOT%\BootLoader\BootLoader.wm.xml %PKG_ROOT%\BootLoader\ >NUL
copy %INFO_ROOT%\BootLoader\flash.bin %PKG_ROOT%\BootLoader\BootLoader\ >NUL

:: Copy the UpdateOS Package XML
mkdir %PKG_ROOT%\SVPlatExtensions >NUL 2>NUL
copy %INFO_ROOT%\SVPlatExtensions\BSP.svupdateOS.wm.xml %PKG_ROOT%\SVPlatExtensions\svupdateOS.wm.xml >NUL

:: Copy Platform Information Packages
echo Copying Platform Information Packages to %PKG_ROOT%
mkdir %PKG_ROOT%\SystemInformation >NUL 2>NUL
copy %INFO_ROOT%\SystemInformation\* %PKG_ROOT%\SystemInformation\ >NUL

mkdir %PKG_ROOT%\DevicePlatform\OEMDevicePlatform >NUL 2>NUL
copy %INFO_ROOT%\OEMDevicePlatform\OEMDevicePlatform.wm.xml %PKG_ROOT%\DevicePlatform\ >NUL
copy %INFO_ROOT%\OEMDevicePlatform\DevicePlatform.xml %PKG_ROOT%\DevicePlatform\OEMDevicePlatform >NUL

mkdir %PKG_ROOT%\DeviceLayout\DeviceLayoutProd >NUL 2>NUL
echo %INFO_ROOT%\DeviceLayoutProd\DeviceLayoutProd.wm.xml %PKG_ROOT%\DeviceLayout\
copy %INFO_ROOT%\DeviceLayoutProd\DeviceLayoutProd.wm.xml %PKG_ROOT%\DeviceLayout\ >NUL
echo %INFO_ROOT%\DeviceLayoutProd\devicelayoutprod.xml %PKG_ROOT%\DeviceLayout\DeviceLayoutProd
copy %INFO_ROOT%\DeviceLayoutProd\devicelayoutprod.xml %PKG_ROOT%\DeviceLayout\DeviceLayoutProd >NUL

exit /b 0

:ERROR
echo BuildBSP.bat failed to copy %FAILURE%binaries to the BSP directory! Please ensure that the drivers have built in Visual Studio.
exit /b 1
