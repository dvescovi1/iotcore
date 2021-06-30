Adding New Boards and Drivers to the BSP {#newboard}
====

## Adding a New Board

This chapter describes the process of setting up a new board configuration for FFU image builds.

### Initialize a new board configuration

 1. Open PowerShell and run `imx-iotcore\build\tools\NewiMX6Board.ps1 <NewBoardName>`.

      - Note: `<NewBoardName>` must follow the schema of BoardName_SoCType_MemoryCapacity. See `imx-iotcore\build\board` for examples.
      - The following instructions assume an example board named **MyBoard_iMX6Q_1GB**.
      - If the script is blocked by execution policy, invoke a powershell from an administrator command prompt to bypass the powershell script execution policy: `powershell.exe -executionpolicy bypass .\NewiMX6Board.ps1 <NewBoardName>`
 2. This step will create a new board configuration in `imx-iotcore\build\board\` and a new firmware folder in `imx-iotcore\build\firmware`.

### Setup the solution in Visual Studio

 1. Open up the *Solution Explorer* view (Ctrl + Alt + L).

 2. Right-click the *Board Packages* folder and select *Add Existing Project*.

 3. Select `imx-iotcore\build\board\MyBoard_iMX6Q_1GB\Package\MyBoardPackage.vcxproj`.

 4. Right-click your *<new project>* => *Build Dependencies* => *Project Dependencies* then select **HalExtiMX6Timers**, **imxusdhc**, and **mx6pep**.
    * For an i.MX7 project select **HalExtiMX7Timers** and **imxusdhc**.
   
 5. Right-click the *GenerateTestFFU* project => *Build Dependencies* => *Project Dependencies* then select your *<new project>* from the list.

 6. Right-click *GenerateBSP* project => *Properties* => *NMake* => *Build Command Line* => *EDIT* and add `call ..\..\..\tools\BuildBSP.bat MyBoard_iMX6Q_1GB`.

 7. Right-click *GenerateBSP* project => *Properties* => *NMake* => *Rebuild All Command Line* => *EDIT* and add `call ..\..\..\tools\BuildBSP.bat MyBoard_iMX6Q_1GB`.

### Update the firmware for your board

 1. Port the firmware to your board following the [i.MX Porting Guide](#porting-guide).
  
 2. Modify `imx-iotcore\build\firmware\ContosoBoard_iMX6Q_2GB\Makefile` with the appropriate values for all CONFIG options. This is used by the makefile to configure each firmware build.
      
    ```makefile
    # Select the defconfig file to use in U-Boot
    UBOOT_CONFIG=mx6sabresd_nt_defconfig

    # Select the DSC file name to use in EDK2
    EDK2_DSC=Sabre_iMX6Q_1GB
    # Select the subdirectory of the Platform folder for this board
    EDK2_PLATFORM=NXP/Sabre_iMX6Q_1GB
    # Select DEBUG or RELEASE build of EDK2
    EDK2_DEBUG_RELEASE=RELEASE

    # Select the FIT Image Tree Source file used to bundle and sign U-Boot and OP-TEE
    UBOOT_OPTEE_ITS=uboot_optee_unsigned.its
    # Select the FIT Image Tree Source file used to bundle and sign UEFI
    UEFI_ITS=uefi_unsigned.its

    all: firmware_fit.merged firmwareversions.log

    include ../Common.mk

    .PHONY: $(OPTEE)
    # Select the PLATFORM for OP-TEE to build
    $(OPTEE):
          $(MAKE) -C $(OPTEE_ROOT) O=$(OPTEE_OUT) PLATFORM=imx-mx6qsabresd \
          $(OPTEE_FLAGS_IMX6)
    ```

 3. This new firmware folder and updated makefile will allow you to use the common firmware makefile to build your firmwares. The makefile can be invoked from `imx-iotcore\build\firmware`. This can be run directly from WSL, on a Linux host, or in CMD by prepending make with "wsl"

    WSL and Linux:

    ```bash
    cd imx-iotcore/build/firmware
    make MyBoard_iMX6Q_1GB
    ```
    CMD and PowerShell:

    ```bash
    cd imx-iotcore\build\firmware
    wsl make MyBoard_iMX6Q_1GB
    ```

### Build the FFU in Visual Studio

 1. Edit **GenerateFFU.bat** in Build Scripts and comment out the board build target using REM. This will speed up FFU generation time since it will only build the FFU for your board.
 2. Select the Release or Debug build target, then right click and build GenerateTestFFU.
 3. After FFU generation completes, your FFU will be available in ```imx-iotcore\build\solution\iMXPlatform\Build\FFU\MyBoard_iMX6Q_1GB``` and can be flashed to an SD card following the instructions in the [IoT Core Manufacturing Guide](https://docs.microsoft.com/en-us/windows-hardware/manufacture/iot/create-a-basic-image#span-idflashanimagespanflash-the-image-to-a-memory-card).

### Board Package Project Meanings

The board package projects are used to build the following packages:
  * **Platform Firmware:** BootFirmware, BootLoader
  * **Platform Identity:** SystemInformation
  * **File system Layout:** DeviceLayoutProd, OEMDevicePlatform
  * **UpdateOS Drivers:** SVPlatExtensions

The board packages have a dependency on HalExtiMX6Timers, mx6pep, and imxusdhc because those are the minimum set of boot critical drivers for i.MX6, so the UpdateOS Drivers package SVPlatExtensions requires them.


## Adding a New Driver

This chapter describes the process of adding a new driver to FFU image builds.

### Adding a New Driver to the Solution

 1. Right-click the Drivers folder in Solution Explorer and add a New Project.

 2. Select Windows Drivers then Kernel Mode Driver or User Mode Driver. Set the name of your driver and set the location to `imx-iotcore\driver`. The rest of the instructions assume the name `MyTestDriver`. After the project has been created, select it in Solution Explorer and save your changes with Ctrl+S.

 3. Copy the reference TestDriver.wm.xml from `imx-iotcore\build\tools\TestDriver.wm.xml` into your project directory and rename it after your project. The rest of the instructions assume the name `MyTestDriver.wm.xml`.

 4. Edit `MyTestDriver.wm.xml` to set the name, namespace, owner, legacyName, and INF source. The legacyName field determines the name of your driver cab package.

 5. Open `MyTestDriver.vcxproj` in a text editor and add the following XML as the first entry under the `<Project>` tag at the top of the file. Change the wm.xml names to match your new one, then save and close the file.

    ```XML
    <Import Project="$(SolutionDir)..\..\common.props"/>
    <ItemGroup>
      <PkgGen Include="MyTestDriver.wm.xml">
        <AdditionalOptions>/universalbsp</AdditionalOptions>
      </PkgGen>
    </ItemGroup>
    ```

 6. Navigate back to Visual Studio and select Reload Solution if it prompts.

 7. Modify your driver's inf to store driver in the Driver Store. Change the DIRID number in DefaultDestDir and ServiceBinary from 12 to 13. This will cause your driver binary to be stored in `C:\Windows\System32\DriverStore` instead of `C:\Windows\System32\Drivers`.

    ```
    [DestinationDirs]
    DefaultDestDir = 13
    ...
    
    ServiceBinary  = %13%\MyTestDriver.sys
    ```
    
 8. Right-click the GenerateTestFFU project, select Project Dependencies, then check the box next to your new project.


### Adding a Driver to the FFU
 1. After adding your driver to the project and building it, confirm that your driver has built and placed its binaries and .cab file inside of `imx-iotcore\build\solution\iMXPlatform\Build\Release\ARM`.

 2. Open the Device Feature Manifest of your board (for example, `imx-iotcore\build\board\Sabre_iMX6DL_1GB\InputFMs\Sabre_iMX6DL_1GB_DeviceFM.xml`).

 3. Add a new PackageFile section to the XML and modify it with the package name set by legacyName in your wm.xml. Change FeatureID to match the other drivers in your board file, or create a new FeatureID for your feature.

    ```XML
    <PackageFile Path="%BSPPKG_DIR%" Name="MyOEM.MyNamespace.MyTestDriver.cab">
      <FeatureIDs>
      <FeatureID>MYNEWFEATURE_DRIVERS</FeatureID>
      </FeatureIDs>
    </PackageFile>
    ```

 4. **If you did not create a new FeatureID, skip this step.** If you created a new FeatureID in your DeviceFM.xml then you must select it in your ProductionOEMInput.xml and TestOEMInput.xml files to include the driver in the respective image 
(for example `imx-iotcore\build\board\Sabre_iMX6DL_1GB\Sabre_iMX6DL_1GB_TestOEMInput.xml` and `imx-iotcore\build\board\Sabre_iMX6DL_1GB\Sabre_iMX6DL_1GB_ProductionOEMInput.xml`).

    ```XML
    <OEM>
      <Feature>MYNEWFEATURE_DRIVERS</Feature>
      <Feature>IMX_DRIVERS</Feature>
    </OEM>
    ```

 5. Clean the solution then rebuild the GenerateTestFFU project and your driver will be included in the FFU.
