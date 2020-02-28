# XL320-with-TM4C123G
Library to communicate from Tiva TM4C123G of Texas Instruments™ to the servomotor XL320 of Robotis.

[![TivaDocs](https://img.shields.io/badge/TI%20TM4C123G-Documentation-red)](http://www.ti.com/tool/EK-TM4C123GXL?keyMatch=tm4c123g&tisearch=Search-EN-Everything)
[![Energia IDE](https://img.shields.io/badge/TI%20Energia-IDE-red)](https://energia.nu/download/)
[![XL320](https://img.shields.io/badge/XL320-e--MANUAL-green)](http://emanual.robotis.com/docs/en/dxl/x/xl320/)

### Overview

The Servo XL320 is a very cool servomotor but the problem is the communication to it, since the protocol it uses is UART but in the Half Duplex Version, that is with only 3 wires, GND, VCC and DATA, this library helps us to communicate with this servo while wired to a tri-buffer state such as SN74LS241N which has 4 tri-state buffers, you either can make a chip with this integrated or wire them on a protoboard.


### Wiring

For sending and recieving packages to and from the servo you will have to wire like this:

Since, you have to send and recieve, you will have to wire like this:

* Pin 21 -> VCC LS241N & VCC XL320
* Pin 22 -> GND LS241N & GND XL320
* Pin 23 -> 1G & 2G LS241N 
* Pin Tx -> 1An LS241N
* Ṕin Rx -> 2Yn LS241N

The Pin Tx/Rx can be found on the upper part of the LaunchPad, unfortunately has no pins soldered so you will have to solder them, and you must have the DATA pin of the XL320 wired to nothing, so what you have to do is:

* DATA: 1Yn & 2An LS241N

The Pin 23 of the TM4C123G can be switched by any other pin that has digital output, since this will only help us to switch the state of the tri-state buffer.

If you don't mind on recieving the packages then you just have to wire like this:

* Pin 22 -> GND XL320
* Pin 21 -> VCC XL320
* Pin Tx -> DATA XL320

### Installation

You just have to clone the repository to your personal files, wherever you are going to use it. You can clone the repository with this command below:

```
git clone https://github.com/FernandoFong/XL320-with-TM4C123G.git
```

And then you just have to import it as:

```
#include "XL320.h"
```

Check what you can do and simply add it to your code, don't forget to initialize your Serial Communication at 115200 baud rate and hand over to your robot your object Serial that you are using.

```
XL320 robot;

void setup() {

  Serial.begin(115200);
  robot.begin(Serial);
  //More code
}
```
### Examples

There are a couple of examples, that can be found in /examples/ and are for the Energia.nu IDE to program microcontrollers but since this library is only a C++ library, I'm pretty sure that you can just include it and it will work perfectly, you don't have to compile it or anything. The two examples included is one for only sending packages and the other one is for sending and recieving packages from the servo.

### Comments

This library will work for any microcontoller that supports the baud rate 115200 for Serial communication, not only the Tiva TM4C123G, you can use any other microcontoller from Texas Instruments™, you can read further information by clicking the badges in this README.

### License
[![MIT License](https://img.shields.io/badge/license-MIT-blue)](https://opensource.org/licenses/mit-license.php)
