#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "Clock.h"
#include "CortexM.h"
#include "Init_Ports.h"
#include "Motor.h"
#include "Timer.h"
#include "Reflectance.h"

/**
 * main.c
 */

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Port2_Init();
	Port3_Init();
	Port4_Init();
	TimerInit();
	Reflectance_Init();
	Clock_Init48MHz(); // makes bus clock 48 MHz
	int32_t data = 0;
	int32_t position = 0;


    while(1)
          {

                data = Reflectance_Read(1300);
                position=Reflectance_Position(data);

               if(position == 0)
               {

                   Motor_Forward(7000,7000);
               }

               if(position >= 48)
               {
                   Motor_Left(0);
                   Motor_Right(7000);

               }

               if(position <= -48)
               {
                   Motor_Right(0);
                   Motor_Left(7000);
               }


         //   printf("%f \n",data);



            DelayTenMilliseconds(); //Delay 10ms to pace state machine


          }


}
