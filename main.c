
# include "bsp.h"
# include "bsp_uart.h"
# include "bsp_timer.h"
# include "serial.h"
# include "led.h"
# include "bsp_i2c.h"
# include "app.h"
# include "utils.h"

#if defined (OS)    
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

int main(void)
{
	BSP__DISABLE_WDT();
	______disableInt();
//	BSP__ConfigureClock();                  /* default clk freq:1MHz; configure clock 16MHz */
	led_init();
	Init_TestPin();
    BSP_UART__Init();
    BSP_TIMER__TA0_Init( BSP_TIMER__TA0 );
//  //BSP_I2C__Init( BSP_I2C__B0 );

    /* Initialise buffer to zero */
    // UTILS__MEMSET(&(i2cBuffer[0]), 0, BSP_I2C__BUF_SZ);
#if defined (OS)    
    /* Initialise the operating system  */
    OS__Init(OS__enSchedPolicyRoundRobin);

    /* Spawn threads */
    osRetCode = OS__Fork(APP__TaskLed1, 4, 20);
    osRetCode = OS__Fork(APP__TaskLed2, 5, 50);
#endif /* OS  */
    ______enableInt();

	while (1)
	{
        BSP_TIMER__DelayMs(10);
		//led1_toggle();
		//led2_toggle();
        //BSP_I2C__Read(BSP_I2C__B0, 0x68, 25, &(i2cBuffer[0]));
		//led2_toggle();
		//led1_toggle();
		//led2_toggle();
        //BSP_TIMER__DelayMs(500);
		//TestPin_toggle();
#if 1
        SERIAL__PRINTF_1(SERIAL__enTxtColorMagenta, "Hello %d World\n", 2);
        SERIAL__PRINTF_0("Goodbye %d %d World\n", 2, 3);
#endif 
#if 0
        SERIAL__Printf(SERIAL__enTxtColorRed, "Hello World - UART \n");
		SERIAL__Printf(SERIAL__enTxtColorMagenta, "Hello World - SERIAL: %%; nbr: %d; char: %c; -ve: %d; \n", 3752, 'b', -3752);
		SERIAL__Printf(SERIAL__enTxtColorBlue,"Hello World - SERIAL: hex: %x; HEX: %X; bin: %b; \n", 555, 555, 38);
		SERIAL__Printf(0,"Hello world\n");
#endif
	}
}

