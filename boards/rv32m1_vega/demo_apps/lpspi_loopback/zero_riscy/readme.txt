Overview
========
The lpspi_loopback demo shows how the lpspi do a loopback transfer, LPSPI
master will transmit data to itself, so please connect the SOUT pin to SIN 
pin directly.

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
To make the example work, connections needed to be as follows:
M0 core:  J4-8 connect to J1-6
M4 core:  J4-2 connect to J2-18

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***LPSPI Loopback Demo***

LPSPI loopback test pass!!!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

