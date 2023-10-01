#include "msp.h"
#include "msp432.h"
#include <stdint.h>
#include <stdbool.h>
#include "Clock.h"
#include "../inc/Clock.h"
#include "../inc/CortexM.h"

#define PERIOD 40000 // 20 ms at 48MHz
#define MIN_PULSEWIDTH 1100 // 2% pulse width
#define MAX_PULSEWIDTH 7200 // 12% pulse width


void DelayTenMilliseconds(void)
{
//***********Start the delay and wait************//
//stop the timer
//clear the timer
//start timer in continuous mode

  TA2CTL &= ~0x0030;//stop mode
  TA2R = 0;//TA1CTL |= BIT2;//clear timer
  TA2CTL |= BIT5;//continuous mode

  while(!(TA2CCTL0 & 0x0001)){}; //complete while loop, wait for the CCIFG to assert
  TA2CCTL0 &= ~0x0001;//clear the CCIFG
  TA2CTL &= ~0x0030;//stop the timer
}


 void delay1 (int Pulse)
 {


     P7DIR |= 0x40;
     P7SEL1 &= ~0x40;
     P7SEL0 |= 0x40;

     TA1CTL |= 0x0200; TA1CTL &= ~0x0100;  // choose SMCLK for the clock source
     TA1CTL |= 0x0080; TA1CTL &= ~0x0040; //SM CLK Divider

     TA2CCTL2 |= 0x00E0;
     TA2CCR0 = 59999;  // clocks for 20ms period
     TA2CCR2 = Pulse; // clocks for 10% duty cycle

     TA1CTL |= 0x0010; // UPMODE

 }


int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
        Clock_Init48MHz();

    int direction = 1; // will be used to sweep from -90 to +90 degrees

    int stepSize = 0;

    while(1)
    {
        if(direction == 1)
        {

            for(stepSize = 6900; stepSize > 1000; stepSize += 20)//we will increment the stepSize until we reach 5999
            {
                delay1(stepSize);
                DelayTenMilliseconds();
                DelayTenMilliseconds();
                DelayTenMilliseconds();
                DelayTenMilliseconds();
                printf("loop 1");


            }
            direction = 0;//after the for loop direction will update back to 0
        }


        if(direction == 0)   //direction will be set at 0 so else statement will run
        {
            for(stepSize = 6901; stepSize > 1000; stepSize -= 20)//we will decrement the stepSize until we reach 0
            {
                delay1(stepSize);
                DelayTenMilliseconds();
                DelayTenMilliseconds();
                DelayTenMilliseconds();
                DelayTenMilliseconds();
                printf("loop 2");

            }
            direction = 1;//after the for loop direction will update to 1 that will make the first if statement to TRUE
        }


    }
    }
