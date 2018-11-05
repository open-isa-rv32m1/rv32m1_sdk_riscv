Overview
========
The SPI flash demo application demonstrates the use of the flash component to erase, program, and read an
external flash device.

Toolchain supported
===================
- GCC RISC-V Embedded 7.1.1
- RISC-V Eclipse IDE 4.7.2

Hardware requirements
=====================
- Mini USB cable
- RV32M1-VEGA board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Running the demo
================
When the demo runs successfully, the following message will be seen on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***SPI Flash Demo***

1 - Erase entire chip
2 - Erase sectors
3 - Erase block
4 - Program one page with pattern (0x5a) and verify
5 - Read byte

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Enter number 1-5, follow the tips  and no "Failed" wording will be on the terminal if the demo runs successfully.
The funcation 1 need to takes about 2 minutes, please wait.
Customization options
=====================

