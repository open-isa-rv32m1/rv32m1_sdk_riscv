Overview
========
The CAU3 Example project is a demonstration program that uses the KSDK software implement
several cryptography algorithms using the CAU3 software driver.


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
No special settings are required.

Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set apropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: one
5.  Confirm selected parameters by OK button.

Running the demo
================
Example output on terminal:

CAU3 Driver Example

AES ECB Test pass
SHA-256 Test pass
ModMul Test pass
Customization options
=====================

