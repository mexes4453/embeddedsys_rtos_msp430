#include "app.h"

void APP__TaskLed1(void)
{
    /* Red LED */
    while (1)
    {
        led1_toggle();
        BSP_TIMER__DelayMs(10);
    }
}


void APP__TaskLed2(void)
{
    /* Green LED */
    while (1)
    {
        led2_toggle();
        BSP_TIMER__DelayMs(30);
    }
}