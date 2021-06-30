Building Windows 10 IoT Core for NXP i.MX Processors {#build-bsp}
==============
## Building the BSP
Before you start building the BSP, you need to have an archive with latest BSP sources from NXP sites downloaded and extracted. After that, you should get the following directory structure:

```bash
$ tree -L 1
.
|- imx-iotcore
|- LA_OPT_NXP_Software_License.htm
|- SCR_imx-iotcore.txt
|- SCR-imx-vpu.txt
|- W<os_version>_imx-iotcore-<build_date>.zip
```

### Required Tools
The following tools are required to build the driver packages and IoT Core FFU: Visual Studio 2017, Windows Kits (ADK/SDK/WDK), and the IoT Core OS Packages.

#### Visual Studio 2017
  * Make sure that you **install Visual Studio 2017 before the WDK** so that the WDK can install a required plugin.
  * Download [Visual Studio 2017](https://docs.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads#step-1-install-visual-studio).
  * During install select **Desktop development with C++**.
  * During install select the following in the Individual components tab. If these options are not available try updating VS2017 to the latest release:
      * **VC++ 2017 version 15.9 v14.16 Libs for Spectre (ARM)**
      * **VC++ 2017 version 15.9 v14.16 Libs for Spectre (ARM64)**
      * **VC++ 2017 version 15.9 v14.16 Libs for Spectre (X86 and x64)**
      * **Visual C++ compilers and libraries for ARM**
      * **Visual C++ compilers and libraries for ARM64**

#### Windows Kits from Windows 10, version 1809
  * **IMPORTANT: Make sure that any previous versions of the ADK and WDK have been uninstalled!**
  * Install [ADK 1809](https://developer.microsoft.com/en-us/windows/hardware/windows-assessment-deployment-kit)
  * Install [WDK 1809](https://docs.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads)
      * Scroll down and select Windows 10, version 1809.	
      * Make sure that you allow the Visual Studio Extension to install after the WDK install completes.
  * If the WDK installer says it could not find the correct SDK version, install [SDK 1809](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/)
      * Scroll down and select Windows 10 SDK, version 1809 (10.0.17763.0).
  * After installing all Windows Kits, restart computer and check if you have correct versions installed in Control panel.

#### IoT Core OS Packages
  * Visit the [Windows IoT Core Downloads](https://www.microsoft.com/en-us/software-download/windows10IoTCore#!) page and download "Windows 10 IoT Core Packages – Windows 10 IoT Core, version 1809 (LTSC)".
  * Open the iso ```Windows10_IoTCore_Packages_ARM32_en-us_17763_253.msi```
  * Install ```Windows_10_IoT_Core_ARM_Packages```
  * Install ```Windows_10_IoT_Core_ARM64_Packages``` for ARM64 builds.

### One-Time Environment Setup
Test certificates must be installed to generate driver packages on a development machine.
 1. Open an Administrator Command Prompt.
 2. Navigate to your BSP and into the folder `imx-iotcore\build\tools`.
 3. Launch `StartBuildEnv.bat`.
 4. Run `SetupCertificate.bat` to install the test certificates.

Some tools may not work correctly if LongPath is not enabled, therefore run following command in console:
 1. Execute `reg add HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem /v LongPathsEnabled  /t REG_DWORD  /d 1` command.

The USN journal registry size has to be set to 1 Mb on your development PC to avoid errors[^1] during FFU creation process by the following:
 1. Execute `reg add HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem /v NtfsAllowUsnMinSize1Mb  /t REG_DWORD  /d 1` command.
 2. Restart PC.

[^1]: The USN journal error message: `Error: CreateUsnJournal: Unable to create USN journal, as one already exists on volume`

Make sure you are running Visual Studio 2017 as Administrator when compiling BSP or assembling the test FFU.

### Creating test FFU
The iMXPlatform Visual Studio solution contains project capable of generating the test FFU for the i.Mx platform.
Generated FFU aims to be used to try out the Windows 10 IoT Core on supported development boards.
OEM manufacturers should follow the [Building the FFU with the IoT ADK AddonKit][] chapter instead.

#### Start generating the FFU
 1. Launch Visual Studio 2017 as Administrator.
 2. Open the solution imx-iotcore\build\solution\iMXPlatform\iMXPlatform.sln located in the path where you have extracted BSP archive.
 3. Change the build type from Debug to Release. Change the build flavor from ARM to ARM64 if building for iMX8.
 4. If secure boot feature is enabled it is required to use OEM signed drivers with exception of HAL that has to be signed by Microsoft. For this reason HAL is distributed signed in binary form along with the BSP sources inside `imx-iotcore\components\NXP.SignedDrivers` directory. For details on deployment of prebuilt HAL see  [How to use the signed prebuilt HAL drivers with the BSP][] section.
 5. To build press Ctrl-Shift-B or choose Build -> Build Solution from menu. This will compile all driver packages then generate the FFU.
 (Depending on the speed of the build machine FFU generation may take around 10-20 minutes.)
 6. After a successful build the new FFU will be located in `imx-iotcore\build\solution\iMXPlatform\Build\FFU\Sabre_iMX6Q_1GB\` for ARM builds and `imx-iotcore\build\solution\iMXPlatform\Build\FFU\NXPEVK_iMX8M_4GB` for ARM64 builds.
 7. The FFU contains firmware components for the NXP IMX8M EVK with i.MX8M Quad Core SOM depending on build flavor. This firmware is automatically applied to the SD Card during the FFU imaging process.

#### Building the FFU for other boards
In order to build an FFU for another board you'll need to modify GenerateFFU.bat in the Build Scripts folder of the Solution Explorer. Comment out the default Sabre_iMX6Q_1GB or NXPEVK_iMX8M_4GB builds with REM and uncomment any other boards you want to build.
```cmd
REM cd /d %BATCH_HOME%
REM echo "Building EVK_iMX6ULL_512MB FFU"
REM call BuildImage EVK_iMX6ULL_512MB EVK_iMX6ULL_512MB_TestOEMInput.xml

cd /d %BATCH_HOME%
echo "Building Sabre_iMX6Q_1GB FFU"
call BuildImage Sabre_iMX6Q_1GB Sabre_iMX6Q_1GB_TestOEMInput.xml
```

### Building the FFU with the IoT ADK AddonKit
 1. Launch Visual Studio 2017 as Administrator.
 2. Open the solution imx-iotcore\build\solution\iMXPlatform\iMXPlatform.sln located in the path where you have extracted BSP archive.
 3. Change the build type from Debug to Release. Change the build flavor from ARM to ARM64 if building for iMX8.
 4. If secure boot feature is enabled it is required to use OEM signed drivers with exception of HAL that has to be signed by Microsoft. For this reason HAL is distributed signed in binary form along with the BSP sources inside `imx-iotcore\components\NXP.SignedDrivers` directory. For details on deployment of prebuilt HAL see  [How to use the signed prebuilt HAL drivers with the BSP][] section.
 5. Build the GenerateBSP project to create a BSP folder in the root of the repository.
 6. Clone the [IoT ADK AddonKit](https://github.com/ms-iot/iot-adk-addonkit) and switch to a working commit. The repository is under active development and might contain errors.
 7. Follow the [Create a basic image instructions](https://docs.microsoft.com/en-us/windows-hardware/manufacture/iot/create-a-basic-image) from the IoT Core Manufacturing guide with the following changes.
      * When importing a BSP use one of the board names from the newly generated BSP folder in the imx-iotcore repository.
        ```
        Import-IoTBSP Sabre_iMX6Q_1GB <Path to imx-iotcore\BSP>
        ```
      * When creating a product use the same board name from the BSP import.
        ```
        Add-IoTProduct ProductA Sabre_iMX6Q_1GB
        ```

### How to use the signed prebuilt HAL drivers with the BSP

The Secure Boot feature of Windows 10 IoT Core requires use of signed drivers. The signing is to be done by OEM with exception of the HAL that should be signed by Microsoft certification authority. HAL is therefore provided signed and prebuild with the BSP sources inside `imx-iotcore\components\NXP.SignedDrivers\BootDrivers.<ARCHITECTURE>` directory. To use these prebuild signed drivers in BSP project there are two options available:

 a) In case the BSP created by Visual Studio is imported by *Iot-AddonKit* it is possible to replace the imported *dll* and *sys* files by hand. However this requires to do the replacement after each reimport of the BSP.
 
 b) The other option is to modify the `iMXPlatform.sln` so the prebuilt driver binaries get copied instead of being built. This ensures the signed drivers will get into BSP directory that could be imported by *Iot-AddonKit* as well as into the testing FFU. The modification can be done by following the steps bellow:

  1. Open the `iMXPlatform.sln` solution in Visual Studio 2017 (run as Administrator).
  2. Remove the drivers being replaced from the solution.
  
      1. Right-click *HAL Extensions* -> choose *Remove*.
      2. Expand *Drivers* -> right-click *<drive-name>* -> choose *Remove*.
  3. Add *New solution folder* (eg 'NXP prebuilt drivers') by right-clicking the *Solution iMXPlatform.sln* -> *add* -> choose *New solution folder*.
  4. Add the projects with prebuilt signed drivers to the newly added solution folder. An example resulting *Solution Explorer* view is shown below the list of steps.
     
     List of projects: 
       - `imx-iotcore\components\NXP.SignedDrivers\VsProjects\HalExtiMX7Timers\HalExtiMX7Timers.vcxproj`
       - `imx-iotcore\components\NXP.SignedDrivers\VsProjects\HalExtiMXDma\HalExtiMXDma.vcxproj`
       - `imx-iotcore\components\NXP.SignedDrivers\VsProjects\imxgpio\imxgpio.vcxproj`
       - `imx-iotcore\components\NXP.SignedDrivers\VsProjects\mx6pep\mx6pep.vcxproj`
       - `imx-iotcore\components\NXP.SignedDrivers\VsProjects\imxusdhc\imxusdhc.vcxproj`
       - `imx-iotcore\components\NXP.SignedDrivers\VsProjects\HalExtiMX6Timers\HalExtiMX6Timers.vcxproj`
     
  5. Add the newly added projects as dependencies of *GenerateBSP* and *GenerateTestFFU* (right-click the *Solution iMXPlatform.sln* -> choose *Properties* -> *Project Dependencies*).
  6. Rebuild the added projects to make sure all changes to the solution take place (right-click *<the-newly-added-solution-folder>* -> choose *Rebuild*).
  7. Now the after making sure these drivers can be deployed you can continue with the [Creating test FFU][] chapter or the [Building the FFU with the IoT ADK AddonKit][] chapter to create FFU to be deployed on your SD card.

 ![Solution explorer with NXP prebuilt drivers projects](images/nxp-prebuilt-drivers-in-solution.png "Solution explorer with NXP prebuilt drivers projects"){width=10cm}
