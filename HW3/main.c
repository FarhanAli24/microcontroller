#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "Clock.h"
#include "CortexM.h"
#include "Init_Ports.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer


	int p = -12;

	double fnote = 0;

	double Tnote = 0;


	for(p <= 12; p++;)
	{
	    printf("TESTING");
	    fnote = 440*2^(p/12);
	    Tnote = (3*10^6)/(2*fnote) + 1;

	    printf("Tnote: %u \n", Tnote);
	}


}
