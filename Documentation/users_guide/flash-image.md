Flash a Windows IoT Core image {#flash-image}
==============

This chapter describes the process of creating a bootable SD card from a downloaded FFU file containing an image of Windows 10 IoT Core system.


1) Download and Start the [Windows IoT Core Dashboard](https://go.microsoft.com/fwlink/p/?LinkId=708576) utility.
1) Navigate to "Set up a new device" tab.
1) Select **NXP [i.MX6/iMX7/i.MX8]** under "Device Type" list box.
1) Select **Custom** under "OS Build" list box.
1) Click **Browse** and navigate and select the FFU file you have downloaded or created by building the BSP.
1) Plug the SD card into the PC, and choose this SD card in "Drive" list box.
1) Set the **Device Name** and **Administrator Password** for your device.
1) Check the **I accept the software license terms** checkbox (lower right) and click **Install**.

![IoT Dashboard](images/dashboard.png "IoT Dashboard")

Windows IoT Core Dashboard will now open a command window and use DISM (Deployment Image Servicing and Management Tool) to flash the FFU file to your microSD card. Make sure you back up any files on your card before proceeding. Flashing will erase all previously stored data on the SD card. 

![Flash](images/rpiflashffudism.jpg "Flash")

**NOTE:**
Alternatively, you can also use the DISM command to manually flash the image:

```cmd
dism.exe /Apply-Image /ImageFile:"D:\flash.ffu" /ApplyDrive:\.\PhysicalDriveX /SkipPlatformCheck
```
Where "PhysicalDriveX" is a name of your SDCARD physical drive. You can use wmic command to see your physical drives:

```cmd
wmic diskdrive get Name, Manufacturer, Model, InterfaceType, MediaType, SerialNumber
```

For more information about flashing the FFU onto an SD Card using the Windows IoT Core Dashboard, follow the [IoT Core Manufacturing Guide](https://docs.microsoft.com/en-us/windows-hardware/manufacture/iot/create-a-basic-image#span-idflashanimagespanflash-the-image-to-a-memory-card).

Once the SD card will be created, plug the card into the board and power up the board. The board should successfully boot up. If not, check the configuration of the boot switches - chapter [Basic Board Setup](#boot-switch). 
Optionally you can follow the steps in [Basic Terminal Setup](#terminal-setup) to establish serial connection between the host PC and the target IoT device to check output from U-Boot and UEFI.
