# NB3000AL-HostFirmware
Altium NanoBoard 3000AL custom Host-FPGA firmware to use the board without Altium Software.

# How to install
A Altium Designer version up to v17 is required. I used a version Summer 09 that came with my NanoBoard. 
I didn't get this version to work with Windows 10 and installed Windows XP under VirtualBox.
No Altium license is required to flash new firmware.
So, set jumper "Platform upgrade enable" and switch on the board. "Golden" boot image will boot.
Now start the Altium Designer and automatic firmware update will start.
Click on "next" button, select new firmware (bin\Firmware.bin in this repository) and click "next" again.
Update process will take a few minutes. Done!

Now the driver should be installed. I used Zadig 2.5 (https://zadig.akeo.ie) to install WinUSB driver.

There are two ways to load the FPGA:
- via USB using "loader.exe" tool
- via JTAG using external ByteBlaster connected on "System JTAG" Header:
  
  TDI : pin 5
  
  TDO : pin 6

  TCK : pin 7

  TMS : pin 8

  GND : pin 9 and 10

For testing purposes i created an LED-Blink FPGA test file (blink.rbf). Just run "loader.exe blink.rbf" and red LED 7 will blink.
