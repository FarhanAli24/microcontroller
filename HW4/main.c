#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "Clock.h"
#include "CortexM.h"
#include "Init_Ports.h"

/**
 * main.c
 */
volatile uint8_t wasInterrupt = 0;
void PORT5_IRQHandler(void)
{
    wasInterrupt = 1;//tell main about interrupt
    P5IFG &= 0x00; // clear interrupt flags
}


void main(void)
{
    PORT5_IRQHandler();
    Port5_Init();
    P5OUT &= 0b00000000;
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	EnableInterrupts();

	srand(20); //seeding random number using current time
	int randomTime = rand(); //generating a random number

	int i;
	int j;
	int x;

while(1)
{
    P5OUT &= ~BIT0;//turn OFF red LED;
    P5OUT &= ~BIT2;//Turn OFF Green Led;
    P5OUT &= ~BIT1;//turn OFF yellow LED
    EnableInterrupts();

    for(i=0; i <= randomTime; i++)
    {
        Clock_Delay1ms(1);
    }

    for(j=0; j <= 3000; j++)
    {
        P5OUT |= BIT1;//turning on Yellow LED
        if(wasInterrupt == 1 && (P5OUT &= BIT1) != 1)
        {
            P5OUT &= ~BIT0;//turn OFF red LED;
            P5OUT |= BIT2;//Turn on Green LED
            P5OUT &= ~BIT1;//turn off yellow LED
            for(j=0; j <= 2000; j++)//leave Green LED on for 2 second
            {
                Clock_Delay1ms(1);
            }
            wasInterrupt = 0; //clear flag
            j = 3001;

        }
        Clock_Delay1ms(1);
    }
    DisableInterrupts();


    if(wasInterrupt == 0 && (P5OUT &= ~BIT2) != 0)
    {
        P5OUT &= ~BIT1;//turn OFF yellow LED
        P5OUT &= ~BIT2;//Turn OFF Green Led;
        P5OUT |= BIT0;//turn on red LED;

        for(i=0; i <= 3000; i++)
        {
            Clock_Delay1ms(1);
        }
        P5OUT &= ~BIT0;//turn OFF red LED;
    }

}

}
