// Reflectance.c
// Provide functions to take measurements using the kit's built-in
// QTRX reflectance sensor array.  Pololu part number 3672. This works by outputting to the
// sensor, waiting, then reading the digital value of each of the
// eight phototransistors.  The more reflective the target surface is,
// the faster the voltage decays.
// Daniel and Jonathan Valvano
// July 11, 2019

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// reflectance even LED illuminate connected to P5.3
// reflectance odd LED illuminate connected to P9.2
// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

#include <stdint.h>
#include "msp432.h"
#include <stdbool.h>
#include "Clock.h"
#include "Timer.h"
#include "CortexM.h"


// ------------Reflectance_Init------------
// Initialize the GPIO pins associated with the QTR-8RC
// reflectance sensor.  Infrared illumination LEDs are
// initially off.
// Input: none
// Output: none
void Reflectance_Init(void){
    // write this as part of Lab 6
    P7DIR &= 0x0000;//setting port 7 as inputs to read value
    P9DIR |=  BIT2;//IR LED
    P5DIR |=  BIT3;//IR LED
    P7OUT &= 0x0000;
    P9OUT &= 0x0000;
    P5OUT &= 0x0000;


    //--MAKING PORT 9 GPIO
    P9SEL1 &= ~BIT2;
    P9SEL0 &= ~BIT2;
    //--------------------

    //--MAKING PORT 7 GPIO
    P7SEL1 &= 0x0000;
    P7SEL0 &= 0x0000;
    //--------------------

    //--MAKING PORT 5 GPIO
    P5SEL1 &= ~BIT3;
    P5SEL0 &= ~BIT3;
    //--------------------

}

// ------------Reflectance_Read------------
// Read the eight sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 100 ms
// Make the sensor pins input
// wait t us
// Read sensors
// Turn off the 8 IR LEDs
// Input: time to wait in usec
// Output: sensor readings
// Assumes: Reflectance_Init() has been called

uint8_t Reflectance_Read(uint32_t time){
uint8_t result;
    // write this as part of Lab 6
  result = 0; // replace this line

P5OUT |= BIT3;
P9OUT |= BIT2;
P7DIR |= 0b11111111;
P7OUT |= 0b11111111;
Clock_Delay1ms(100);
P7DIR &= 0x0000;
Clock_Delay1us(time);
result = P7IN;

  P7OUT &= 0x0000;
  P9OUT &= 0x0000;
  P5OUT &= 0x0000;

  return result;
}

// Perform sensor integration
// Input: data is 8-bit result from line sensor
// Output: position in 0.1mm relative to center of line
int32_t Reflectance_Position(uint8_t data){
    // write this as part of Lab 6
int32_t weights[] = {-334, -238, -142, -48, 48, 142, 238, 334};
int32_t sum = 0;
int32_t i=0;
int32_t total = 0;
for (i = 0; i < 8; i++)
{
    int32_t sensor_value = (data >> i) & 0x01;
    sum += sensor_value * weights[i];
    total += sensor_value;
}

if (total > 0)
{
    return (sum) / total; // Position in 0.1mm units
}
else
{
    return 0;
}

 return 0; // replace this line
}


