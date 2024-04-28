
# include "bsp.h"
# include "bsp_uart.h"
# include "bsp_timer.h"
# include "led.h"




#if 0
void copy_vector(void)
{
	extern uint16_t vectors_start, vectors_end;

}

#endif
//extern t_uartBuf uartBuf;



int main(void)
{
	BSP__DISABLE_WDT();                     // stop watchdog timer
//	BSP__ConfigureClock();                  // default clk freq:1MHz; configure clock 16MHz
	led_init();
	Init_TestPin();
    BSP_UART__Init();
    BSP_TIMER__TA0_Init( BSP_TIMER__TA0 );
    BSP__ENABLE_INTERRUPT();

	while (1)
	{
		//led_toggle();
        BSP_TIMER__DelayMs(1000);
		//TestPin_toggle();
		BSP_UART__PutChar('t');
		BSP_UART__PutChar('b');
		BSP_UART__PutChar('y');
		BSP_UART__PutChar('t');
		BSP_UART__PutChar('e');
		BSP_UART__PutChar('s');
		BSP_UART__PutChar('\n');
		BSP_UART__PutString("Hello World\n");

        //while (1);
	}
}

