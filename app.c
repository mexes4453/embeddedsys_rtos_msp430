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
            OS__SetStatus(OS__GetCurrThread(), OS__enStatusFree);
            OS__Sched();
            OS__Kill(OS__GetCurrThread());
            OS__Tswitch();
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
            OS__SetStatus(OS__GetCurrThread(), OS__enStatusFree);
            OS__Sched();
            OS__Kill(OS__GetCurrThread());
            OS__Tswitch();
        }
    }
}