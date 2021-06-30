Basic Terminal Setup {#terminal-setup}
====
During the boot, you can check the U-Boot and UEFI firmware output on the host PC by using the serial interface console.
In the case you don't see any output on the connected display, for example, this might be helpful to confirm that the board is booting.
Common serial communication applications such as HyperTerminal, Tera Term, or PuTTY can be used on the host PC side. The example below describes the serial terminal setup using Tera Term on a host running Windows OS.
The i.MX boards connect the host driver using the micro-B USB connector.

1. Connect the target and the PC running Windows OS using a cable mentioned above.
2. Open Tera Term on the PC running Windows OS and select the settings as shown in
the following figure.

![Tera Term settings for terminal setup](images/com_settings.png "Tera term settings for terminal setup"){ width=8cm }

The USB to serial driver for FTDI chip can be found under [http://www.ftdichip.com/Drivers/VCP.htm](http://www.ftdichip.com/Drivers/VCP.htm).
The FTDI USB to serial converters provide up to four serial ports. Users need to select the first port (COM) in the terminal setup.
The USB to serial driver for CP210x chip can be found under [https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers). 
The CP210x USB to serial converters provide up to two serial ports.
