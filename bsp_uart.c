#include "bsp_uart.h"

//static_assert((UART__BAUDRATE == 9600), "Invalid baudrate");

/* Private variables */
static t_uartBuf uartBuf;
static unsigned char rxBuffer[BSP_UART__BUFSZ];
static unsigned char txBuffer[BSP_UART__BUFSZ];

static inline void BSP_UART__DisableInterrupt(t_uartAx *u, uint8_t bitMask)
{
    u->IE &= ~bitMask;
}
static inline void BSP_UART__EnableInterrupt(t_uartAx *u, uint8_t bitMask)
{
    u->IE |= bitMask;
}
static inline void BSP_UART__ClearInterrupt(t_uartAx *u, uint8_t bitMask)
{
    u->IFG &= ~bitMask;
}

t_uartBuf *BSP_UART__GetBufAddr(void)
{
    return (&uartBuf);
}

void BSP_UART__Init(void)
{
	/* Configure Port for alternate function
	 * P3.3,4 = USCI_A0 TXD/RXD*/ 
	P3SEL |= BIT3+BIT4;
	P3DIR |= BIT3;

	/* Set bit for configuration */
	BSP_UART__A0->CTL1 |= UCSWRST;

	/* Use default start(1bit) | data (8bits) | stop (1bit) */
	BSP_UART__A0->CTL0 = 0;

	/* Select SMCLK as clock source */
	BSP_UART__A0->CTL1 |= UCSSEL_2;

	/* Set clock prescaler to the integer part of divisor N */
	BSP_UART__A0->BR0 = 6;
	BSP_UART__A0->BR1 = 0;
//	UCA0BR0 = BSP_UART__DIVISOR_LOW_BYTE;
//	UCA0BR1 = BSP_UART__DIVISOR_HIGH_BYTE;

	/* Set modulation to account for the fractional part of
	 * divisor N */
    BSP_UART__A0->MCTL = UCBRS_0 + UCBRF_13 + UCOS16;
	//UCA0MCTL = (UART__UCBRF << 4) + (UART__UCBRS << 1) + BSP_UART__UC0S16;
    
    BSP_UART__A0->IFG = 0; // clearr interrupts 

    /* Enable interrupt */
	//BSP_UART__A0->IE = 0x03; // (UCRXIE | UCTXIE);
    BSP_UART__EnableInterrupt(BSP_UART__A0, (BSP_UART__UCAx_B_RX | BSP_UART__UCAx_B_TX));


    RING_BUFFER__Init(&(uartBuf.rxBuf), rxBuffer, BSP_UART__BUFSZ);
    RING_BUFFER__Init(&(uartBuf.txBuf), txBuffer, BSP_UART__BUFSZ);

	/* Clear bit to allow operation */
	BSP_UART__A0->CTL1 &= ~UCSWRST;
}


void BSP_UART__PutChar(unsigned char c)
{
	/* Check that there is no ongoing transmission */
    //while (!(BSP_UART__A0->IFG & 0x02)){};
    tenUartState txState = BSP_UART__enStateTxIdle;
    
    /* Check that there is room to add char in the tx buffer */
    while( RING_BUFFER__IsFull(&(uartBuf.txBuf)) ){};
    
    if ( !(RING_BUFFER__IsEmpty(&(uartBuf.txBuf))) )
    {
        txState = BSP_UART__enStateTxBusy;
    }
    BSP_UART__DisableInterrupt(BSP_UART__A0, BSP_UART__UCAx_B_TX);
    RING_BUFFER__Put(&(uartBuf.txBuf), c);

    if (txState == BSP_UART__enStateTxIdle)
    {
        BSP_UART__A0->TXBUF = RING_BUFFER__Peek(&(uartBuf.txBuf));
    }
    BSP_UART__EnableInterrupt(BSP_UART__A0, BSP_UART__UCAx_B_TX);

    /* Handle the return carrier - Upon newline (\n) the cursor should be moved
       back to the beginning of the newline */
    if ( c == '\n')
    {
        //RING_BUFFER__Put(&(uartBuf.txBuf), '\r');
        //
        BSP_UART__PutChar('\r');
    }
}


void  BSP_UART__PutString(char *s)
{
    uint32_t idx;
    for (idx =0; s[idx] != '\0'; idx++)
    {
        BSP_UART__PutChar(s[idx]);
    }
    return ;
}




char BSP_UART__GetChar(void)
{
    char c;

    /**
     * Check that that the rx buffer is not empty else loop until
     * data is available in the buffer to consume */
    while( RING_BUFFER__IsEmpty(&(uartBuf.rxBuf)) ){};

    /* Retrieve data from the rx ring buffer within safe region
       by disabling the interrupt */
    BSP_UART__DisableInterrupt(BSP_UART__A0, BSP_UART__UCAx_B_RX);
    c = RING_BUFFER__Get(&(uartBuf.rxBuf));
    BSP_UART__EnableInterrupt(BSP_UART__A0, BSP_UART__UCAx_B_RX);

    return c;
}



void BSP_UART__GetString(char *s)
{
    while ((*s = (char)BSP_UART__GetChar(void)) != '\r')
    {
        BSP_UART__PutChar(*s++);
    }
    /* Null terminate the string */
    *s = 0;
}





__attribute__((interrupt(BSP_UART__VECTOR_IDX_A0))) 
void BSP_UART__ISR_A0_TX_RX(void)
{   
    switch (BSP_UART__A0->IV)
    {
        case BSP_UART__UCA0IV_TX:
        {
	        //led_toggle();
            RING_BUFFER__Get(&(uartBuf.txBuf));                            /* Remove chr from tx buffer */
            BSP_UART__ClearInterrupt(BSP_UART__A0, BSP_UART__UCAx_B_TX);   /* clear interrupts Tx only  */
            if ( !(RING_BUFFER__IsEmpty(&(uartBuf.txBuf))) )
            {
                BSP_UART__A0->TXBUF = RING_BUFFER__Peek(&(uartBuf.txBuf));
            }
            else
            {
                BSP_UART__DisableInterrupt(BSP_UART__A0, BSP_UART__UCAx_B_TX);
            }
            break ;
        }
        case BSP_UART__UCA0IV_RX:
        {
            BSP__CRITICAL_SECTION_START();
            RING_BUFFER__Put(&(uartBuf.rxBuf), BSP_UART__A0->RXBUF );
            BSP__CRITICAL_SECTION_END();
            break ;
        }
        default:
        { /* do nothing */
        	//led_toggle();
        }
    }
}
