#include <msp430.h> 
#include "eUSCI_I2CDriver.h"

/**
 * main.c
 */

char Message1[] = {0x06, 0x01};
char Message2[] = {0x07, 0x01};
char Message3[] = {0x08, 0x01};
char Message4[] = {0x09, 0x01};
char LEDDriverAddress = 0x67;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	I2CInit();
    PM5CTL0 &= ~LOCKLPM5;

	int i = 0;
	__bis_SR_register(GIE);
	while(1)
	{
	    //__delay_cycles(1000000);
	    I2CWriteMultipleBytes(LEDDriverAddress, 2, &Message1);
        //__delay_cycles(1000000);
        I2CWriteMultipleBytes(LEDDriverAddress, 2, &Message2);
        //__delay_cycles(1000000);
        I2CWriteMultipleBytes(LEDDriverAddress, 2, &Message3);
        //__delay_cycles(1000000);
        I2CWriteMultipleBytes(LEDDriverAddress, 2, &Message4);
	}
	
	return 0;
}
