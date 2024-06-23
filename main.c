
# include "bsp.h"
# include "bsp_uart.h"
# include "bsp_timer.h"
# include "serial.h"
# include "dev_led.h"
# include "bsp_i2c.h"
# include "app.h"
# include "utils.h"

#if defined (__OS__)    
# include "os.h"
#endif /* OS  */

#if 0
void copy_vector(void)
{
	extern uint16_t vectors_start, vectors_end;

}

#endif

//unsigned char i2cBuffer[BSP_I2C__BUF_SZ];
tenOsRetCode osRetCode;
char uart_str[20];

int main(void)
{

    /* Hardware Initialisation */
	BSP__DISABLE_WDT();
	______disableInt();
//	BSP__ConfigureClock();                  /* default clk freq:1MHz; configure clock 16MHz */
	DEV_LED___Init();
	Init_TestPin();
    BSP_UART__Init();
    BSP_TIMER__TA0_Init( BSP_TIMER__TA0 );
    UTILS__Memset(&(uart_str[0]), 0, 20);        /* Initialise the uart_str buffer */

//  //BSP_I2C__Init( BSP_I2C__B0 );

    /* Initialise buffer to zero */
    // UTILS__MEMSET(&(i2cBuffer[0]), 0, BSP_I2C__BUF_SZ);
#if defined (__OS__)    
    /* Initialise the operating system  */
    OS__Init(OS__enSchedPolicyRoundRobin);

    /* Spawn threads */
    osRetCode = OS__Fork(APP__TaskLed1, 4, 20);
    osRetCode = OS__Fork(APP__TaskLed2, 5, 50);

    /* Transfer the control to the operating system */
    OS__Start();  

#else /* Execute normally without scratchOS */

    ______enableInt();

#endif /* OS  */

	while (1)
	{
        BSP_TIMER__DelayMs(1000);
		//led1_toggle();
		//led2_toggle();
        //BSP_I2C__Read(BSP_I2C__B0, 0x68, 25, &(i2cBuffer[0]));
		//led2_toggle();
		//led1_toggle();
		//led2_toggle();
        //BSP_TIMER__DelayMs(500);
		//TestPin_toggle();
#if 0
        SERIAL__PRINTF_1(SERIAL__enTxtColorMagenta, "Hello %d World\n", 2);
        SERIAL__PRINTF_0("Goodbye %d %d World\n", 2, 3);
        BSP_UART__GetString(uart_str);
        SERIAL__PRINTF_0(" %s\n", uart_str);
#endif 
	}
}

