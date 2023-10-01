
#include "msp.h"
#include "msp432.h"
#include <stdint.h>
#include <stdbool.h>
#include "Clock.h"
#include "Clock.h"
#include "CortexM.h"

#define PERIOD 40000 // 20 ms at 48MHz
#define MIN_PULSEWIDTH 1100 // 2% pulse width
#define MAX_PULSEWIDTH 7200 // 12% pulse width



 void delay1 (int Pulse)
 {



     P6DIR |= 0b01000000;
     P6SEL1 &=  ~0xC0;
     P6SEL0 |=  0xC0;

     TA2CTL &= ~0x0030;
     TA2CTL |= 0x0200; TA2CTL &= ~0x0100;  // choose SMCLK for the clock source
     TA2CTL |= 0x0080; TA2CTL &= ~0x0040; //SM CLK Divider

     TA2CCTL3 |= 0x00E0;
     TA2CCR0 = 59999; // clocks for 20ms period
     TA2CCR3 = Pulse; // clocks for 10% duty cycle

     TA2CTL |= 0x0010; // UPMODE

 }


int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
        Clock_Init48MHz();

    int direction = 1; // will be used to sweep from -90 to +90 degrees

    uint16_t stepSize = 0;

    //delay1(7500);


while(1)
{

    if(direction == 1)
    {

        for(stepSize = 1500; stepSize <= 7500; stepSize += 100)//we will increment the stepSize until we reach 5999
        {
            delay1(stepSize);

            printf("Loop 1 %d \n", stepSize);


        }
        direction = 0;//after the for loop direction will update back to 0
    }


    if(direction == 0)   //direction will be set at 0 so else statement will run
    {
        for(stepSize = 7500; stepSize >= 1500; stepSize -= 100)//we will decrement the stepSize until we reach 0
        {
            delay1(stepSize);

            printf("Loop 2\n", stepSize);

        }
        direction = 1;//after the for loop direction will update to 1 that will make the first if statement to TRUE
    }


}
}

