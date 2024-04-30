
# include "bsp.h"
# include "bsp_uart.h"
# include "bsp_timer.h"
# include "serial.h"
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
		led1_toggle();
        BSP_TIMER__DelayMs(500);
		led2_toggle();
        BSP_TIMER__DelayMs(500);
		//TestPin_toggle();
#if 0
        SERIAL__Printf(SERIAL__enTxtColorRed, "Hello World - UART \n");
		SERIAL__Printf(SERIAL__enTxtColorMagenta, "Hello World - SERIAL: %%; nbr: %d; char: %c; -ve: %d; \n", 3752, 'b', -3752);
		SERIAL__Printf(SERIAL__enTxtColorBlue,"Hello World - SERIAL: hex: %x; HEX: %X; bin: %b; \n", 555, 555, 38);
		SERIAL__Printf(0,"Hello world\n");
#endif

        //while (1);
	}
}

