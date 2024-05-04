#ifndef BSP_UART_H
#define BSP_UART_H

//#include <assert.h>
#include "bsp.h"
#include "ring_buffer.h"


#define SMCLK (1048576)
#define BRCLK (SMCLK)
#define BSP_UART__BAUDRATE (9600)
#define BSP_UART__DIVISOR  ((float)BRCLK / UART__BAUDRATE)
#define BSP_UART__DIVISOR_16BIT ((unsigned short)UART__DIVISOR)
#define BSP_UART__DIVISOR_LOW_BYTE  (UART__DIVISOR_16BIT & 0xFF)
#define BSP_UART__DIVISOR_HIGH_BYTE (UART__DIVISOR_16BIT >> 8)
#define BSP_UART__DIVISOR_FRACTIONAL (UART__DIVISOR - UART__DIVISOR_16BIT)

/* --- UCA0MCTL -- Modulation control register --- */
/* First Modulation stage value */
#define BSP_UART__UCBRF  (0)

/* Second Modulation stage value */
#define BSP_UART__UCBRS  ((unsigned char)(8 * UART__DIVISOR_FRACTIONAL))

#define BSP_UART__UC0S16        (0)         /* Over Sampling mode disabled */
#define BSP_UART__VECTOR_IDX_A0  56         /* 0xFFF0 - vector address     */
#define BSP_UART__UCA0IV_RX      0x02       /* Interrupt vector identifier */
#define BSP_UART__UCA0IV_TX      0x04       /* Interrupt vector identifier */
#define BSP_UART__UCAx_B_RX      (0x01)     /* bit 0 - RX for IE & IFG Reg */
#define BSP_UART__UCAx_B_TX      (0x02)     /* bit 1 - TX for IE & IFG Reg */
#define BSP_UART__TA0_BASE_ADDR  (0x05C0U)


typedef struct
{
    _IO_B_REG_ CTL1;          /* offset 0x00 - control register 1             */
    _IO_B_REG_ CTL0;          /* offset 0x01 - control register 0             */
    _IO_B_REG_ RESERVED_0[4]; /* offset 0x02                                  */
    _IO_B_REG_ BR0;           /* offset 0x06 - Baud rate - 0                  */
    _IO_B_REG_ BR1;           /* offset 0x07 - Baud rate - 1                  */
    _IO_B_REG_ MCTL;          /* offset 0x08 - Modulation control register    */
    _IO_B_REG_ RESERVED_1[1]; /* offset 0x09                                  */
    _IO_B_REG_ STAT;          /* offset 0x0A - Status Register                */
    _IO_B_REG_ RESERVED_2[1]; /* offset 0x0B                                  */
    _IO_B_REG_ RXBUF;         /* offset 0x0C - Transmit buffer                */
    _IO_B_REG_ RESERVED_3[1]; /* offset 0x0D                                  */
    _IO_B_REG_ TXBUF;         /* offset 0x0E - Transmit buffer                */
    _IO_B_REG_ RESERVED_4[1]; /* offset 0x0F                                  */
    _IO_B_REG_ ABCTL;         /* offset 0x10 - Auto baudrate control register */
    _IO_B_REG_ RESERVED_5[1]; /* offset 0x11                                  */
    _IO_B_REG_ IRTCTL;        /* offset 0x12 - IrDA transmit control reg      */
    _IO_B_REG_ IRRCTL;        /* offset 0x13 - IrDA receive control reg       */
    _IO_B_REG_ RESERVED_6[8]; /* offset 0x14                                  */
    _IO_B_REG_ IE;            /* offset 0x1C - Interrupt enable register      */
    _IO_B_REG_ IFG;           /* offset 0x1D - Interrupt flag register        */
    _IO_W_REG_ IV;            /* offset 0x1E - Interrupt Vector register      */
}   t_uartAx;


#define BSP_UART__A0        ((t_uartAx *)BSP_UART__TA0_BASE_ADDR)
#define BSP_UART__BUFSZ     (16)

typedef struct 
{
    t_ringBuffer rxBuf;
    t_ringBuffer txBuf;
}   t_uartBuf;


typedef enum
{
    BSP_UART__enStateTxIdle,
    BSP_UART__enStateTxBusy,
}   tenUartState;

/* ========= Function declaration ======== */
void BSP_UART__Init(void);


/**
 * @brief 
 * This function sends a character via uart. 
 * 
 * @arg unsigned char c byte to transmit.
 */
void  BSP_UART__PutChar(unsigned char c);
void  BSP_UART__PutString(char *s);



/**
 * @brief 
 * This function receives a character via uart. 
 * 
 * @return unsigned char
 */
unsigned char BSP_UART__GetChar(void);

t_uartBuf *BSP_UART__GetBufAddr(void);




void BSP_UART__ISR_A0_TX_RX(void);

#endif /* BSP_UART_H */

