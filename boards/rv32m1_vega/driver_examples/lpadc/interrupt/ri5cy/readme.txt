Overview
========

The lpdc_single_interrupt example shows how to use interrupt with LPADC driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the LPADC's
sample input. When running the project, typing any key into debug console would trigger the conversion. ADC watermark 
interrupt would be asserted once the number of datawords stored in the ADC Result FIFO is greater than watermark value.
In ADC ISR, the watermark flag would be cleared by reading the conversion result value. Also, result information would
be printed when the execution return to the main function.


Toolchain supported
===================
- GCC RISC-V Embedded 7.1.1
- RISC-V Eclipse IDE 4.7.2

Hardware requirements
=====================
- Mini/micro USB cable
- RV32M1-VEGA board
- Personal Computer

Board settings
==============
Connect pin(J2-4) to input voltage.

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Interrupt Example
Please press any key to get user channel's ADC value.
ADC value: 4093
ADC interrupt count: 1
ADC value: 4095
ADC interrupt count: 2
ADC value: 4093
ADC interrupt count: 3
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

