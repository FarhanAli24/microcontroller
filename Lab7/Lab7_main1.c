#include "msp.h"
#include <msp432.h>
#include <stdint.h>
#include <stdbool.h>
#include "Clock.h"
#include "CortexM.h"
#include "motor.h"
#include "Init_Ports.h"
#include "Timer.h"

#define TRIGGER 0x04
#define ECHO 0x08

#define microsecondsToClockCycles(a) ( (a) * 1.5 )       //assume 12Mhz clock divided by 8
#define clockCyclesToMicroseconds(a) ( (a) / 1.5 )       // 1.5 clock cycles = 1us

void Servo(uint16_t angle);
uint32_t pulseIn (void);


void ServoInit(void)
{
    Servo(4500);     //call Servo() function to center servo
    Clock_Delay1ms(10);     //delay here to give servo time to move - can use built in timer function
    TA3CTL &= ~0x0030; //stop the timer
    return;
}
void Servo(uint16_t angle_count)
{
    TA3CTL |= 0x0200; TA3CTL &= ~0x0100;  // choose SMCLK for the clock source
    TA3CTL |= 0x0080; TA3CTL &= ~0x0040; //SM CLK Divider

    TA3CCTL2 |= 0x00E0;

    TA3CCR0 = 59999; // clocks for 20ms period
    TA3CCR2 = angle_count; // clocks for 10% duty cycle

    TA3CTL |= 0x0010; // UPMODE
    return;
}
uint16_t distanceInCm(void)
{
    uint16_t distance;

    P6OUT |= TRIGGER;      //drive trigger pin high
    Clock_Delay1us(10);      //wait 10 us - can use built-in timer function
    P6OUT &= ~TRIGGER;      //drive trigger pin low
    distance = (pulseIn() * 0.034)/2;      //calculate distance using s=t * 0.034/2. t comes from pulseIn() function
    if(distance >= 0)
    {
      return distance;      //return the distance
    }
    return 0;      //if no echo (distance = 0), assume object is at farthest distance
}
uint32_t pulseIn (void)
{
    uint32_t width;// = 0;
    uint32_t time = 0;
    uint32_t maxcount = 56999;  //max count for 38 ms (timeout)
    TA2CTL |= 0x0020;           //set timer for continuous mode
    TA2R = 0;                   //reset the count

    while(!(P6IN & ECHO))//wait for the pulse to start (while Echo is low)
    {
        if(TA2R > maxcount)
        {
            return 0;   //if count is greater than maxcount return 0
        }

    }



    TA2R = 0;  //reset the count
    while((P6IN & ECHO)) //wait for the pulse to finish (while Echo is high)
    {
        if(TA2R > maxcount)
        {
            return 0; //if count is greater than maxcount return 0
        }
    }


    width = TA2R;//read the count (width of the return pulse)
    TA2CTL &= ~0x0030;//stop the timer
    time = clockCyclesToMicroseconds(width);// convert the reading to microseconds.


    return time;               //return the microsecond reading
}

void main(void)
{

    uint16_t distance, right_wall, left_wall;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Clock_Init48MHz();  // makes bus clock 48 MHz
	//call all the port initialization functions
	Port8_Init();
	Port6_Init();
	Port5_Init();
	Port4_Init();
	Port3_Init();
	Port2_Init();
	TimerInit();
	ServoInit();
	//These are the states of the state machine
	enum motor_states {FORWARD, BACKWARD, RIGHT, LEFT, SWEEP_RIGHT, SWEEP_LEFT} state, prevState;

	state = FORWARD;          //start in FORWARD state
	prevState = !FORWARD;   //used to know when the state has changed
	uint16_t stateTimer = 0;           //used to stay in a state
	bool isNewState;              //true when the state has switched

	//uint16_t x = 0;
    Servo(4500);//middle
    Clock_Delay1ms(20);
	while(1) {
	    isNewState = (state != prevState);
            prevState = state;
	    distance = distanceInCm();  //this needs to be moved to the states that use it
	    int distance1Scan = 0;
	    int distance2Scan = 0;
	    switch(state)
	    {

	    case FORWARD:
	               //entry housekeeping
	               if (isNewState)
	               {
	                   stateTimer = 0;
	                   ServoInit();//middle
	                   Clock_Delay1ms(20);
	                   right_wall = 0;
	                   left_wall = 0;
	               }
	               //state business
	               Motor_Forward(14999, 14999);
	               //exit condition
	               if (distance < 4) {
	                   state = BACKWARD;
	               }
	           break;

	           case BACKWARD:
	               //entry housekeeping
	               if (isNewState) {
	                   stateTimer = 0;
	               }
	               //state business

	               Motor_Backward(14999, 14999);

	               stateTimer++;

                    if (stateTimer >= 50)
                       {
                           Motor_Stop();
                       }
	               //exit condition
	               if (stateTimer > 100) {
	                   state = SWEEP_RIGHT;
	               }
	           break;

	           case SWEEP_RIGHT:
                   //entry housekeeping
                   if (isNewState) {
                       stateTimer = 0;
                   }
                   //state business
                   if (stateTimer < 25) {
                       Servo(4700);
                       Clock_Delay1ms(20);
                   }
                   if (stateTimer >= 50) {
                       Servo(2000);
                       Clock_Delay1ms(20);
                   }
                   stateTimer++;

                   right_wall = distance;
                   //exit condition
                   if (stateTimer >= 100) {
                       state = SWEEP_LEFT;
                   }
               break;

	           case SWEEP_LEFT:
                   //entry housekeeping
                   if (isNewState)
                   {
                       stateTimer = 0;
                   }
                   //state business
                   if (stateTimer < 25)
                   {
                       Servo(4700);
                       Clock_Delay1ms(20);
                   }
                   if (stateTimer >= 50)
                   {
                       Servo(7500);
                       Clock_Delay1ms(20);
                   }
                   stateTimer++;

                   left_wall = distance;
                   //exit condition
                   if (stateTimer >= 100) {
                       if (right_wall > left_wall) {
                           state = LEFT;
                       }
                       else
                       {
                           state = RIGHT;
                       }
                   }
	                   break;

	           case RIGHT:
                   //entry housekeeping
                   if (isNewState) {
                       stateTimer = 0;
                   }
                   //state business
                   Motor_Right(14999);
                   stateTimer++;
                   //exit condition
                   if (stateTimer > 70) {
                       Servo(4500);//middle
                       Clock_Delay1ms(20);
                       state = FORWARD;
                   }
               break;

	           case LEFT:
                   //entry housekeeping
                   if (isNewState) {
                       stateTimer = 0;
                   }
                   //state business
                   Motor_Left(14999);
                   stateTimer++;
                   //exit condition
                   if (stateTimer > 70) {
                       Servo(4500);//middle
                       Clock_Delay1ms(20);
                       state = FORWARD;
                   }
	           break;
               Servo(4500);//middle
               Clock_Delay1ms(20);
	           default: state = FORWARD;
	          break;


       } //end switch
	    Clock_Delay1ms(20);
	} //while

}
