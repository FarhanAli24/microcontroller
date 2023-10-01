#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "Clock.h"
#include "CortexM.h"
#include "Init_Ports.h"
#include "Motor.h"


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
    //TA0CTL &= ~BIT7;
    TA0CTL |= 0x0040; //clk divider 2


    TA0CCTL3 |= 0x00E0; //Reset/set block 3
    TA0CCTL4 |= 0x00E0; //Reset/set block 4

    TA0CCR0 = 59999;//60k clock for 10ms period
    TA0CCR3 = 14999;//25% duty cycle
    TA0CCR4 = 14999;



//***********Now initialize TimerAx for the delay function************//
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
    TA1CTL &= ~BIT7;
    TA1CTL |= 0x0040; //clk divider 2

    TA1CCR0 = 59999;//10ms delay

}

void DelayTenMilliseconds(void)
{
//***********Start the delay and wait************//
//stop the timer
//clear the timer
//start timer in continuous mode

  TA1CTL &= ~0x0030;//stop mode
  TA1R = 0;//TA1CTL |= BIT2;//clear timer
  TA1CTL |= BIT5;//continuous mode

  while(!(TA1CCTL0 & 0x0001)){}; //complete while loop, wait for the CCIFG to assert
  TA1CCTL0 &= ~0x0001;//clear the CCIFG
  TA1CTL &= ~0x0030;//stop the timer
}

void main(void)
{
WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
Clock_Init48MHz(); // makes bus clock 48 MHz
TimerInit();
// call port initialization functions
Port2_Init();
Port3_Init();
Port5_Init();
//declare enumerated states, declare starting state, declare previous state, declare state timer
enum motor_states {forward, turnRight} state, prevState;
//declare boolean to know if state has switched use lecture 3 for state machine reference
state = forward;                   //start state
prevState = !forward;                //used to know when the state has changed
uint16_t stateTimer;       //used to stay in a state
bool isNewState;           //true when the state has switched
while(1)
{


    isNewState = (state != prevState);
    prevState = state;  //save state for next time

switch (state) {

        case forward:
                if(isNewState)
                {
                    Motor_Stop();
                    stateTimer = 0;
                }

                Motor_Forward(14999, 14999);
                stateTimer++;

                if(stateTimer >= 110)
                {
                  state = turnRight;
                }

        break;
        case turnRight:

                      if(isNewState)
                      {
                         Motor_Stop();
                          stateTimer = 0;
                      }

                      Motor_Right(14999);
                      stateTimer++;

                      if(stateTimer >= 102)
                      {
                          state = forward;
                      }

\
                  break;
                  default:
                      state = forward;

//create default statement for switch
} //end switch

//Motor_Forward(14999, 14999);
DelayTenMilliseconds(); //Delay 10ms to pace state machine
P2OUT ^= 0b00010000;//Toggle LED to demonstrate 10ms delay
} //end while(1)
} //end main(
