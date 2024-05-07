#include "bsp.h"
#include "bsp_timer.h"

extern uint8_t  OS__switchPeriod; 
static uint16_t BSP_TIMER__tick=0;

void BSP_TIMER__TA0_Init(t_timerTx0 *t)
{
    /* Select clock source */
    t->CTL |= BSP_TIMER__TA0CTL_B98_TASSELx;

    /* Enable interrupt */
    t->CCTL0 |= BSP_TIMER__TA0CCTL0_B4_CCIE;

    t->CTL |= 0x0080;   /* Divide SMCLK by 4 bits 7:6 */
    t->EX0 |= 0x0000;   /* Divide SMCLK by 1 */  

    /**
     * set tick count for 1ms = (1048576Hz / (4 * 1 * 1000)) = 262
     * counter is zero based */
    t->CCR0 = 261;
    
    /* start timer */
    t->CTL |= BSP_TIMER__TA0CTL_B54_MC;
}


uint16_t BSP_TIMER__GetTickCnt(void)
{
    uint16_t tick = BSP_TIMER__tick;
    return (tick);
}




void BSP_TIMER__DelayMs(uint16_t ticks)
{
    uint16_t start = BSP_TIMER__GetTickCnt();

    while ((BSP_TIMER__GetTickCnt() - start) < ticks)
    {
        /* Do nothing */
    }
}




extern void OS__Tswitch(void);
__attribute__((interrupt(BSP_TIMER__VECTOR_IDX_TA0))) 
void Timer_A0_ISR( void )
{
    BSP_TIMER__tick++;

#if 0
    /* Switch context after 100 ms -> 1msec */
    if (BSP_TIMER__tick % OS__switchPeriod == 0)
    {
        ______disableInt();
        OS__Tswitch();
        ______enableInt();
    }
#endif 

}
