#include "app.h"



void APP__Task1(void)
{
    uint32_t volatile cnt=200000;
    t_thread  *t = OS__GetCurrThread();
    /* Red LED */
    while (1)
    {
        //DEV_LED__ToggleRed();
        //APP__CRITICAL_START();
        SERIAL__Printf(SERIAL__enTxtColorRed, "Hello -> From taskId(%d) - Red\n", t->tid);
        //APP__CRITICAL_END();
        //BSP_TIMER__DelayMs(30);
        OS__Delay(10); /* 1 tick ~ 1ms */
        if ( !(cnt--) )
        {
            DEV_LED__OffRed();
            OS__Kill(t);
        }
    }
}



void APP__Task2(void)
{
    /* Green LED */
    t_thread  *t = OS__GetCurrThread();
    uint32_t volatile cnt=100000;
    while (1)
    {
        //DEV_LED__ToggleGreen();
        //APP__CRITICAL_START();
        SERIAL__Printf(SERIAL__enTxtColorGreen, "Hello -> From taskId(%d) - Green\n", t->tid);
        //APP__CRITICAL_END();
        //BSP_TIMER__DelayMs(30);
        OS__Delay(5); /* 1 tick ~ 1ms */
        if ( !(cnt--) )
        {
            DEV_LED__OffGreen();
            OS__Kill(OS__GetCurrThread());
        }
    }
}



void APP__Task3(void)
{
    /* Green LED */
    t_thread  *t = OS__GetCurrThread();
    uint32_t volatile cnt=100000;
    while (1)
    {
        //DEV_LED__ToggleGreen();
        //APP__CRITICAL_START();
        SERIAL__Printf(SERIAL__enTxtColorBlue, "Hello -> From taskId(%d) - Blue\n", t->tid);
        //APP__CRITICAL_END();
        //BSP_TIMER__DelayMs(30);
        OS__Delay(5); /* 1 tick ~ 1ms */
        if ( !(cnt--) )
        {
            DEV_LED__OffGreen();
            OS__Kill(OS__GetCurrThread());
        }
    }
}




void APP__Task4(void)
{
    /* Green LED */
    t_thread  *t = OS__GetCurrThread();
    uint32_t volatile cnt=100000;
    while (1)
    {
        //DEV_LED__ToggleGreen();
        //APP__CRITICAL_START();
        SERIAL__Printf(SERIAL__enTxtColorYellow, "Hello -> From taskId(%d) - Yellow\n", t->tid);
        //APP__CRITICAL_END();
        //BSP_TIMER__DelayMs(30);
        OS__Delay(5); /* 1 tick ~ 1ms */
        if ( !(cnt--) )
        {
            DEV_LED__OffGreen();
            OS__Kill(OS__GetCurrThread());
        }
    }
}


