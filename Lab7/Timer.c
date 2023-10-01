#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>
#include "Clock.h"
#include "CortexM.h"
#include "Init_Ports.h"
void TimerInit(void)
{
//**********First initialize TimerA0 for PWM
//Since the motors are connected to P2.6 and P2.7, use TimerA0, compare blocks 3 & 4****
//stop the timer
//choose SMCLK for the clock source
//choose clock divider of 2
//Outmode 7: reset/set

    //0b1111111111001111
    TA0CTL &= ~0x0030; //Stopping Timer enabled


//***********Selecting SMCLK***********
    //0b0000001000000000
    TA0CTL |= 0x0200;

    //0b1111111011111111
    TA0CTL &= ~0x0100;
//*************************************

    TA0CTL |= 0x0040; //clk divider 2

    TA0CCTL3 |= 0x00E0; //Reset/set block 3
    TA0CCTL4 |= 0x00E0; //Reset/set block 4
    TA0CCR0 = 59999;
    TA0CCR3 = 14999;
    TA0CCR4 = 14999;




//*******************TIMER FOR SERVO******************************************

//****************************************************************************


//*******************TIMER FOR ULTRASONIC SENSOR***************************
    TA2CTL &= ~0x0030;
    TA2CTL |= 0x0200;
    TA2CTL &= ~0x0100;  // choose SMCLK for the clock source

    TA2CTL |= BIT7;
    TA2CTL |= BIT6; //SM CLK Divider 8

   TA2CCTL3 |= 0x00E0;
   TA2CCR0 = 59999; // clocks for 20ms period
//*************************************************************************




//***********Now initialize TimerAx for the delay function************
//stop the timer
//choose SMCLK for the clock source
//choose clock divider of 2
//configure compare register for 10ms delay

TA1CTL &= ~0x0030;//stop mode


//***********Selecting SMCLK***********
    //0b0000001000000000
    TA1CTL |= 0x0200;

    //0b1111111011111111
    TA1CTL &= ~0x0100;
//*************************************

    TA1CTL |= 0x0040; //clk divider 2

    TA1CCR0 = 59999; //configure compare register for 10ms delay

}

void DelayTenMilliseconds(void)
{
//***********Start the delay and wait************//
//stop the timer
//clear the timer
//start timer in continuous mode

  TA1CTL &= ~0x0030;//stop mode
  TA1R = 0;//clear timer
  TA1CTL |= 0x0020;//start timer in continuous mode

  while(!(TA1CCTL0 & 0x0001)){}; //complete while loop, wait for the CCIFG to assert
  TA1CCTL0 &= ~0x0001;//clear the CCIFG
  TA1CTL &= ~0x0030;//stop the timer
}


