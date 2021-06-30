Building and Updating ARM32 Firmware {#build-arm32-firmware}
==============

This chapter describes how to set up a build environment to build the latest ARM32 firmware, update the firmware on the SD Card for testing and development, and include the new firmware in the FFU builds.

## Setting up your build environment

 1. Set up a Linux environment such as:
      * Dedicated Linux system
      * Linux Virtual Machine
      * Windows Subsystem for Linux ([WSL setup instructions](https://docs.microsoft.com/en-us/windows/wsl/install-win10))
        
        > Note: We validate with both Ubuntu in WSL and standalone Ubuntu machines.

 2. Update and install build tools
    
    ```bash
    $ sudo apt-get update
    $ sudo apt-get upgrade
    $ sudo apt-get install build-essential python python-dev python-crypto python-wand device-tree-compiler bison flex swig iasl uuid-dev wget git bc libssl-dev python3-setuptools python3 python3-pyelftools
    $ pushd ~
    $ wget https://releases.linaro.org/components/toolchain/binaries/6.4-2017.11/arm-linux-gnueabihf/gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf.tar.xz
    $ tar xf gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf.tar.xz
    $ rm gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf.tar.xz
    $ popd
    ```

 3. Ensure that case sensitivity is turned on for the u-boot directory when building in WSL.
    
    ! This is not enough, move the repository from /mnt/c to your home directory of WSL or we have to check if case sensitivity for OP tee fixes the problem.
    
    ```bash
    setfattr -n system.wsl_case_sensitive -v 1 u-boot
    ```

 4. Download the BSP archives from NXP web site and extract it. The following commands can be used to extract archives.

    ```bash
    # Note: The contents will be extracted into current directory by default    
    unzip W*_imx-iotcore*.zip # -d <output-directory-path> 
    tar -xf W*_imx-firmware*.tgz # -C <output-directory-path>
    ```

 5. At this point your directory structure looks like the following:

    ```bash
    $ tree -L 1
    .
    |- cst
    |- edk2
    |- external
    |- firmware-imx-8.1
    |- imx-atf
    |- imx-edk2-platforms
    |- imx-iotcore
    |- imx-mkimage
    |- LA_OPT_NXP_Software_License.htm
    |- MSRSec
    |- mu_platform_nxp
    |- optee_os
    |- RIoT
    |- SCR_imx-firmware.txt
    |- SCR_imx-iotcore.txt
    |- SCR-imx-vpu.txt
    |- u-boots
    |- W<os_version>_imx-firmware-<build_date>.tgz
    |- W<os_version>_imx-iotcore-<build_date>.zip
    ```

## Building the firmware

 1. Build firmware to test the setup. Adding "-j 20" to make will parallelize the build and speed it up significantly on WSL, but since the firmwares build in parallel it will be more difficult to diagnose any build failures. You can customize the number to work best with your system.

    ```bash
    $ cd imx-iotcore/build/firmware/<board-name>
    $ make
    ```

 2. After a successful build you should have several output files:

    ```bash
    firmware_fit.merged - Contains SPL, OP-TEE, and U-Boot proper
    uefi.fit - Contains the UEFI firmware
    ```

## Adding updated firmware to your ARM FFU

 1. To make the updated firmware a part of your FFU build, you must copy the firmwares to your board's Package folder in imx-iotcore.

      * Copy `uefi.fit` into `/board/<board-name>/Package/BootFirmware`
      * Copy `firmware_fit.merged` into `/board/<board-name>/Package/BootLoader`
      * You can also use the following make command to copy `uefi.fit` and `firmware_fit.merged` to the correct package directories. 
    
        ```bash
        $ make update-ffu
        ```

 2. If you decided to store the sources and firmwares inside git repository when preparing to commit your changes, you should use the following make command to save your OP-TEE SDK and the commit versions of your firmware automatically in your board folder.

    ```bash
    $ make commit-firmware
    ```

## Deploying firmware to an SD card manually

### Bootable Firmware without installing an FFU

If you want to rapidly deploy and test firmware configurations without needing the full Windows boot, you can prepare an SD card manually to boot only the firmware stages.

The SD card must have two partitions in the following order:

  * 4MB partition at the start of the disk, no file system. This is where U-Boot and OP-TEE get deployed.
  * 50MB partition formatted fat32, optionally labeled efi. This is where UEFI gets deployed.

Here are the steps to run in an administrator CMD to prepare an SD card in Windows:

```bat
powershell Get-WmiObject Win32_DiskDrive
REM Find the SD card in that list and use the number after PhysicalDrive as your disk number.
diskpart
list disk
sel disk <#>
list part
REM Check the partitions to make sure this disk is actually your SD card.
clean
create partition primary size=4
create partition primary size=50
format quick fs=fat32 label=EFI
assign
list vol
exit
```

### Deploying U-Boot and OP-TEE (firmware_fit.merged) for development

On Windows you can use [DD for Windows](http://www.chrysocome.net/dd) from an administrator command prompt to deploy firmware_fit.merged.
Be careful that you use the correct `of` and `seek`, DD will not ask for confirmation.

```bash
powershell Get-WmiObject Win32_DiskDrive
dd if=firmware_fit.merged of=\\.\PhysicalDrive<X> bs=512 seek=2
```

  * Where `PhysicalDrive<X>` is the *DeviceID* of your SD card as shown by `Get-WmiObject`.

You might get the output: `Error reading file: 87 The parameter is incorrect`. This error can be ignored as it refers to non-aligned writes on block devices. DD for Windows will align and retry.

If you are working on a dedicated Linux machine (not WSL or VM) use:
```
dd if=firmware_fit.merged of=/dev/sdX bs=512 seek=2
```

### Deploying UEFI (uefi.fit) for development
Copy `uefi.fit` over to the EFI partition on your SD card.

### Updating the TAs in UEFI {#ta-update}
A firmware TPM TA, and UEFI authenticated variable TA, are included with EDK2. Generally, these TAs should work on any ARM32 system where OP-TEE is running, and eMMC RPMB is available.

These binaries are built using OpenSSL by default, but they can also be built using WolfSSL (See `FTPM_FLAGS` and `AUTHVARS_FLAGS` in `common.mk`).

They are omitted from the firmware if the `CONFIG_NOT_SECURE_UEFI=1` flag is set. This is useful for early development work if RPMB storage is not functioning yet, or if eMMC is not present on the device.

They can be rebuilt using:

```bash
make update_tas
```
This updates the binaries included in the imx-edk2-platforms repository.

#### Clearing RPMB

If the TAs are changed significantly, or the storage becomes corrupted, it may be necessary to clear the OP-TEE secure file system in RPMB. This can be done by building OP-TEE with the `CFG_RPMB_RESET_FAT=y` flag set. This flag will cause OP-TEE to erase its FAT metadata when it first accesses RPMB during every boot. This effectively clears all the data stored by the TAs. After clearing the RPMB OP-TEE should be switched back to `CFG_RPMB_RESET_FAT=n` to allow variables to persist again.
