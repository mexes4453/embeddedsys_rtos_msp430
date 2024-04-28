#include "led.h"

void led_init(void)
{
	P1DIR |= 0x01;		// Set P1.0 to output direction
    P1OUT = 0x00;
}



void led_toggle(void)
{
	P1OUT ^= 0x01;		// Toggle P1.0 using XOR
}


void Init_TestPin(void)
{
	P1DIR |= (1<<5);		// Set P1.5 to output direction
}


void TestPin_toggle(void)
{
	P1OUT ^= (1<<5);		// Toggle P1.5 using XOR
}
