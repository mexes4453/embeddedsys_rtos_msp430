#include "app.h"



void APP__TaskLed1(void)
{
    uint32_t volatile cnt=200000;
    /* Red LED */
    while (1)
    {
        DEV_LED__ToggleRed();
        //BSP_TIMER__DelayMs(30);
        OS__Delay(5); /* 1 tick ~ 1ms */
        if ( !(cnt--) )
        {
            DEV_LED__OffRed();
            OS__Kill(OS__GetCurrThread());
        }
    }
}



void APP__TaskLed2(void)
{
    /* Green LED */
    uint32_t volatile cnt=100000;
    while (1)
    {
        DEV_LED__ToggleGreen();
        //BSP_TIMER__DelayMs(30);
        OS__Delay(5); /* 1 tick ~ 1ms */
        if ( !(cnt--) )
        {
            DEV_LED__OffGreen();
            OS__Kill(OS__GetCurrThread());
        }
    }
}
