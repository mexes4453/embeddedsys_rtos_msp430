#include "app.h"



void APP__TaskLed1(void)
{
    uint16_t volatile cnt=5000;
    /* Red LED */
    while (1)
    {
        led1_toggle();
        BSP_TIMER__DelayMs(30);
        if ( !(cnt--) )
        {
            OS__Kill(OS__GetCurrThread());
        }
    }
}



void APP__TaskLed2(void)
{
    /* Green LED */
    uint16_t volatile cnt=3000;
    while (1)
    {
        led2_toggle();
        BSP_TIMER__DelayMs(30);
        if ( !(cnt--) )
        {
            OS__Kill(OS__GetCurrThread());
        }
    }
}