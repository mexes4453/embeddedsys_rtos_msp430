#ifndef BSP_TIMER_H
#define BSP_TIMER_H

#include "bsp.h"

#define BSP_TIMER__VECTOR_IDX_TA0         (53)   /* 0x0FFEAU - Vector addr */
#define BSP_TIMER__TA0_BASE_ADDR          (0x0340U)
#define BSP_TIMER__TB0_BASE_ADDR          (0x03C0U)


#define BSP_TIMER__TA0CTL_B54_MC          0x0010U   /* Mode Ctrl - up mode */
#define BSP_TIMER__TA0CTL_B1_TAIE         0x0002U   /* Mode Ctrl - up mode */
#define BSP_TIMER__TA0CTL_B2_TACLR        0x0004U   /* Mode Ctrl - up mode */
#define BSP_TIMER__TA0CTL_B98_TASSELx     0x0200U   /* Clock source select - SMCLK */
#define BSP_TIMER__TA0CCTL0_B4_CCIE       0x0010U   /* Interrupt Enabled */




typedef struct
{
    _IO_W_REG_ CTL;           /* offset 0x00 - control register              */
    _IO_W_REG_ CCTL0;         /* offset 0x02 - capture/compare control reg 0 */
    _IO_W_REG_ CCTL1;         /* offset 0x04 - capture/compare control reg 1 */
    _IO_W_REG_ CCTL2;         /* offset 0x06 - capture/compare control reg 2 */
    _IO_W_REG_ CCTL3;         /* offset 0x08 - capture/compare control reg 3 */
    _IO_W_REG_ CCTL4;         /* offset 0x0A - capture/compare control reg 4 */
    _IO_W_REG_ RESERVED_0[2]; /* offset 0x0C                                 */
    _IO_W_REG_ R;             /* offset 0x10 - counter register              */
    _IO_W_REG_ CCR0;          /* offset 0x12 - capture/compare register 0    */
    _IO_W_REG_ CCR1;          /* offset 0x14 - capture/compare register 1    */
    _IO_W_REG_ CCR2;          /* offset 0x16 - capture/compare register 2    */
    _IO_W_REG_ CCR3;          /* offset 0x18 - capture/compare register 3    */
    _IO_W_REG_ CCR4;          /* offset 0x1A - capture/compare register 4    */
    _IO_W_REG_ RESERVED_1[2]; /* offset 0x1C                                 */
    _IO_W_REG_ EX0;           /* offset 0x20 - expansion                     */
    _IO_W_REG_ RESERVED_2[6]; /* offset 0x22                                 */
    _IO_W_REG_ IV;            /* offset 0x2E - interrupt vector              */
}   t_timerTx0;


#define BSP_TIMER__TA0        ((t_timerTx0 *)BSP_TIMER__TA0_BASE_ADDR)
#define BSP_TIMER__TB0        ((t_timerTx0 *)BSP_TIMER__TB0_BASE_ADDR)

void BSP_TIMER__TA0_Init(t_timerTx0 *t);


uint16_t    BSP_TIMER__GetTickCnt(void);
void BSP_TIMER__DelayMs(uint16_t ticks);



void Timer_A0_ISR( void );


#endif /* BSP_TIMER_H */