Overview
========

The tstmr example shows the usage of TSTMR driver in application. The TSTMR module is a free running incrementing counter that starts running after system reset de-assertion and can be read at any time by the software for determining the software ticks.The TSTMR runs off the 1 MHz clock and resets on every system reset.

In this example, it would output a time stamp information when the application is ready. And then, delay for 1 second with TSTMR_DelayUs() function. Before and after the delay, it would output the two time stamps information again.


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
This demo prints through the OpenSDA serial port, don't need special board setting.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on RV32M1-VEGA board.
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
Note: when debug is attached, tool will configure ETM, which will block the read under this case, it will always return 0 when ETM is enabled and CPU read the counter. So this example can't run successfully when debug on M4 core. Please Download the program to the target board and press the reset button to begin running on M4 core.

When the demo runs successfully, will get the similar messages on the OpenSDA terminal.

~~~~~~~~~~~~~~~~~~~~~~
 Timestamp1 = 09c4460

 Test the delay function, delay for 1 second

 Start time = 09c5cfd

 End time = 0aba7c9
~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

