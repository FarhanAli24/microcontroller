#include "msp.h"
#include <math.h>
#include "Clock.h"
#include "CortexM.h"
void adcSetup(void){
    P8DIR &= 0x08;              //configure input A22
    P5SEL0 |= 0x08;             //P8.3 SEL = 11
    P5SEL1 |= 0x08;
    ADC14->CTL0 = 0x00000010;   //power on and disable ADC (ENC) for config

    ADC14->CTL0 |= (BIT(30) | BIT(24) | BIT(20) | BIT(19) | BIT(18) | BIT(16) | BIT9 | BIT8);
    ADC14->CTL1 |= 0x00000030;  //14 bit resolution - 16 bit clock conversion time
    ADC14->MCTL[22] |= (BIT4 | BIT2 | BIT1);  //A22 input, single ended, AVCC -> AVSS, no window
    ADC14->MCTL[22] &= ~(BITB | BITA | BIT9 | BIT8 | BIT3 | BIT0);
    ADC14->CTL1 |= BIT(23);   //temperature sensor bit
    ADC14->CTL0 |= 0x00000002;  //enable ADC to complete conversion
}

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Clock_Init48MHz();                              // makes bus clock 48 MHz
    adcSetup();

    int16_t inputData;
    int16_t calcVoltage;
    int16_t fahrenheit;
    int16_t celsius;

    while(1){
        ADC14->CTL0 |= 0x01;                                 //start conversion
        while(!(ADC14->IFGR0) & 0x00000020);                 //wait until conversion is completed
        inputData = ADC14->MEM[22];                          //read and store conversion value


        calcVoltage = (((float)inputData)*(3.3/pow(2,14)));

        celsius = (((calcVoltage) - 0.685)/0.019) - 45;        //temperature in celsius
        fahrenheit = (celsius * 9)/5 + 32;                         //temperature in fahrenheit

        printf("Value is : %d \n", inputData);
        printf("Converted value is : %d Volts\n", calcVoltage);
        printf("Celsius: %d \n", celsius);
        printf("Fahrenheit: %d \n", fahrenheit);
        Clock_Delay1ms(1000);                                  //delay for 1s until next read

    }  //while(1)
}  //main


