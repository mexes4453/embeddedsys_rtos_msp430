#include "dev_led.h"

void DEV_LED___Init(void)
{
	P1DIR |= DEV_LED__BITMASK_RED;		// Set P1.0 to output direction
	P4DIR |= DEV_LED__BITMASK_GREEN;		// Set P4.7 to output direction
    DEV_LED__OffRed();
    DEV_LED__OffGreen();
}



void DEV_LED__ToggleRed(void)
{
	P1OUT ^= DEV_LED__BITMASK_RED;		// Toggle P1.0 using XOR
}

void DEV_LED__ToggleGreen(void)
{
	P4OUT ^= DEV_LED__BITMASK_GREEN;		// Toggle P1.0 using XOR
}



void DEV_LED__OffRed(void)
{
    P1OUT &= ~DEV_LED__BITMASK_RED;
}

void DEV_LED__OffGreen(void)
{
    P4OUT &= ~DEV_LED__BITMASK_GREEN;
}





void Init_TestPin(void)
{
	P1DIR |= (1<<5);		// Set P1.5 to output direction
}


void TestPin_toggle(void)
{
	P1OUT ^= (1<<5);		// Toggle P1.5 using XOR
}
