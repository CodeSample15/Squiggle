[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)


![ ig ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)

#  ST7735_TFT_RPI

## Table of contents


  * [Overview](#overview)
  * [See Also](#see-also)
  * [Installation](#installation)
  * [Test](#test)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system)
      * [Fonts](#fonts)
      * [Bitmap](#bitmap)  
      * [Tool chain](#tool-chain)  
  * [Hardware](#hardware)
  * [Output](#output)

## Overview

* Name: ST7735_TFT_RPI
* Description:

0. C++ Library for a TFT SPI LCD, ST7735 Driver
1. Dynamic install-able Raspberry Pi C++ library.
2. Inverse colour, rotate, sleep, idle  & vertical scroll modes supported.
3. 10 fonts
4. Graphics + print class included.
5. 24 bit colour , 16 bit color & bi-color Bitmaps supported.
6. Hardware and Software SPI
7. Dependency: bcm2835 Library

* Author: Gavin Lyons
* Port of my PIC library at [github link.](https://github.com/gavinlyonsrepo/pic_16F18346_projects)


## See Also

The ST7735_TFT_RPI library has been amalgamated into my combined display library ['Display_Lib_RPI.'](https://github.com/gavinlyonsrepo/Display_Lib_RPI)
Which is under active development and should work on any Linux based single board computer.

## Installation

1. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.73)
	* The bcm2835 library  provides SPI bus, delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

2. Download the STT735_TFT_RPI library
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.

```sh
curl -sL https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/archive/1.7.1.tar.gz | tar xz
```

3. Install library. Run "make" to run the makefile to install library, it will be
    installed to usr/lib and usr/include 

```sh
cd ST7735_TFT_RPI-1.7.1
make
sudo make install
```

Note :: to uninstall library run sudo make uninstall

## Test

1. Next step is to test TFT and installed library with the included main.cpp test example file.
Wire up your TFT. Next enter the example folder and run the makefile in THAT folder,
This second makefile builds the examples file using the just installed library,
and creates a test exe file in "bin". Make run will use "sudo" as the bcm2835 requires root permissions by default [see here for details](http://www.airspayce.com/mikem/bcm2835/)
you should now see the test routines in that file running on the display. The example file is setup by default for a 128x128 red tab TFT.

```sh
cd example/
make
make run
```

2. There are 7 examples files. The default example file is Hello World.
To decide which one the makefile(In examples folder) builds simply edit "SRC" variable
at top of the makefile. In the "User SRC directory Option Section" at top of file.
Pick an example "SRC" directory path and ONE ONLY.

| # | example file name  | Desc|
| ------ | ------ |  ------ |
| 1 | src/Hello_world| Basic use case |
| 2 | src/Hello_world_SWSPI| Basic use case for software SPI |
| 3 | src/Text_Tests | Tests text & fonts|
| 4 | src/Graphic_Tests | graphics |
| 5 | src/Bitmap_Tests | bitmap |
| 6 | src/Function_Tests | function testing (scroll rotate etc)|
| 7 | src/Frame_rate_test | Frame rate per second (FPS) speed tests |


## Software

### User options

In the example files. There are 3 sections in "Setup()" function 
where user can make adjustments to select for SPI type used, PCB type used and screen size.


1. USER OPTION 1 GPIO/SPI TYPE
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 PCB_TYPE, SPI SPEED , SPI_CE_PIN


*USER OPTION 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The TFTSetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for TFTSetupGPIO define which is used.
HW SPI is far faster and more reliable than SW SPI.
The SPI settings are set in TFTSPIHWSettings() function internally at init.
If there are multiple SPI devices on Bus(with different settings) the user can call this method to refresh settings before a tranche of LCD commands.

*USER OPTION 2 Screen size  + Offsets*

User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge 
such as cropped data or defective pixels.
The function TFTInitScreenSize sets them.

*USER OPTION 3 PCB Version, SPI SPEED , SPI_CE_PIN*

TFTInitPCBType function is overloaded(2 off , one for HW SPI the other for SW SPI).

Param1 PCB_TYPE

In the main.cpp in USER OPTION 3 PCB_TYPE select your display type.
By passing an enum type to function  TFTInitPCBType.
Default is "TFT_ST7735R_Red". There are 4 types of the ST7735 TFT display supported.
If your display works but RGB colors are inverted you may have chosen wrong display type.

| Number | Description | Enum label| Tested |
| ---- | ---- | --- | ---| 
| 1 | ST7735B controller| TFT_ST7735B | n/a |
| 2 | ST7735R Green Tab | TFT_ST7735R_Green | n/a |
| 3 | ST7735R Red Tab   | TFT_ST7735R_Red | RED PCB v1.1, 1.44, 128x128 pixels |
| 4 | ST7735S Black Tab | TFT_ST7735S_Black | RED PCB v1.2, 1.8, 128 x 160 pixels |


Param2A SPI_Speed (HW SPI Only)

Here the user can pass the SPI Bus freq in Hertz,
Maximum 125 Mhz , Minimum 30Khz, The default in file is 8Mhz 
If you set to 0 .Speed is set to bcm2835 
constant BCM2835_SPI_CLOCK_DIVIDER_32. If using SW spi, ignore.

Param3 SPI_CE_PIN (HW SPI Onl
Which Chip enable pin to use two choices. If using SW spi, ignore.
	* SPICE0 = 0
	* SPICE1 = 1

Param2B SPI_CommDelay (SW SPI Only)

The user can adjust If user is having reliability issues with SW SPI in some setups.
This is a microsecond delay in SW SPI GPIO loop. It is set to 0 by default, Increasing it will slow 
down SW SPI further.

### File system

In example folder:
The Main.cpp file contains tests showing library functions.
A bitmap data file contains data for bi-color bitmaps and icons tests.
The color bitmaps used in testing are in bitmap folder, 3 16-bit and 3 24-bit images.

There are two makefiles

    1. At root directory builds and installs library at a system level.
    2. Example directory builds example file + installed library to an executable.


### Fonts

Font data table:

| num | enum name | Char size XbyY | ASCII range | Size bytes | Size Scale-able |
| ------ | ------ | ------ | ------ |  ------ | ----- |
| 1 | $_Default | 5x8 |0-0xFF, Full Extended|1275 |Y|
| 2 | $_Thick   | 7x8 |0x20-0x5A, no lowercase letters |406|Y|
| 3 | $_SevenSeg  | 4x8 |0x20-0x7A |360|Y|
| 4 | $_Wide | 8x8 |0x20-0x5A, no lowercase letters|464|Y|
| 5 | $_Tiny | 3x8 |0x20-0x7E |285|Y|
| 6 | $_Homespun  | 7x8 |0x20-0x7E |658|Y|
| 7 | $_Bignum | 16x32 |0x2D-0x3A ,0-10 - . / : |896|N|
| 8 | $_Mednum | 16x16 |0x2D-0x3A ,0-10 - . / :|448|N|
| 9 | $_ArialRound| 16x24 | 0x20-0x7E |4608|N|
| 10 | $_ArialBold | 16x16 |0x20-0x7E |3072|N|
| 11 | $_Mia| 8x16 | 0x20-0x7E |1520|N|
| 12 | $_Dedica | 6x12 |0x20-0x7E |1152|N|

1. $ = TFTFont
2. A print class is available to print out many data types.
3. Fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
4. Font 7-8 are large numerical fonts and cannot be scaled(just one size).
5. Fonts 9-12 Alphanumeric fonts and cannot be scaled(just one size)

Font Methods:

| Font num | Method | Size parameter | Notes |
| ------ | ------ | ------ |  ------ |
| 1-6 | drawChar|Y| draws single  character |
| 1-6 | drawText |Y| draws character array |
| 7-12 | drawChar|N| draws single  character |
| 7-12 | drawText|N| draws character array |
| 1-12 | print |~| Polymorphic print class which will print out many data types |

These  functions return a number greater than zero in event of an error, such as wrong font chosen , ASCII character outside
chosen fonts range, character out of screen bounds and invalid character array pointer object.

### Bitmap

There are four functions to support drawing bitmaps

| Function Name | Colour support | Pixel size |  Note |
| ------ | ------ | ------ | ------ |
| TFTdrawIcon | bi-colour | 8 x(0-127)   | Data vertically addressed |
| TFTdrawBitmap | bi-colour |  2048 bytes max | Data horizontally  addressed |
| TFTdrawBitmap16 | 16 bit color 565 BMP files |  32K max | ------ |
| TFTdrawBitmap24  | 24 bit color BMP files | 48K max | Converted by software to 16-bit color  |

1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Pixel size column assumes 128 by 128 screen.

The TFTdrawBitmap function will return an error if : The Bitmap is completely off screen or 
Invalid Bitmap pointer object. The TFTdrawBitmapXX functions will also return an error if memory
be assigned. All 4 function's will return 0 for success.

### Tool chain

* Development Tool chain. 
	1. Raspberry PI 3 model b
	2. C++, g++ (Debian 12.2.0) 
	3. Raspbian , Debian 12 bookworm OS, , 64 bit.
	4. kernel : aarch64 Linux 6.1.0-rpi7-rpi-v8
	5. [bcm2835 Library 1.73 dependency](http://www.airspayce.com/mikem/bcm2835/). Provides low level SPI bus, delays and GPIO control.

## Hardware

Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC |  VCC |
| 2 | SCLK | SPI_CLK | GPIO5 |
| 3 | SDA | SPI_MOSI | GPIO6 |
| 4 | A0/DC | GPIO24 | GPIO24  |
| 5 | RESET | GPI025  | GPIO25 |
| 6 | SS/CS | SPI_CE0 | GPIO8 |
| 7 | GND | GND | GND |
| 8 | VCC | VCC | VCC  |

1. Connect LED backlight pin 1 thru a 150R/220R ohm resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. Pick any GPIO you want for SW SPI for HW SPI: reset and DC lines are flexible.
5. User can select  SPI_CE0  or SPI_CE1 for HW SPI
![ wiring ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/wiring.jpg)
6. Backlight control is left to user.

## Output

Output of some of the test routine's. Left to right, top to bottom.

1. Different defined colors at default font size 1. Full 16 bit colour 565 available
2. Different sizes of default font. Size 2 3 4 & 5 shown.
3. Different Fonts at font size 2, fonts 1-5. Are these fonts are scale-able, font 6 not shown.
4. Shapes
5. More Shapes!
6. Bitmap (bi-color) A background and a foreground.
7. Clock Demo showing icons, small bitmaps and font 7 "BigNums"
8. 24-bit color bitmap test image
9. 16-bit color bitmap test image
10. Fonts 7-12

![ pic ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/9.jpg)
![ pic1 ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)
![ p2](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/5.jpg)
