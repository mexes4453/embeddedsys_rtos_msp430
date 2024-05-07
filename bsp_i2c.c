#include "bsp_i2c.h"


void BSP_I2C__Init(t_i2cBx *i)
{
    i->CTL1 |= UCSWRST;               // put eUSCI_B in reset state
    i->CTL0 |= UCMODE_3 + UCMST;      // I2C master mode
    i->CTL0 |= 0x01;                  // I2C sychronous mode
    i->BR0 = 8;                      // baud rate = SMCLK / 10
    //i->CTL1 |= 0x04;                  // bit3 automatic STOP assertion
    i->CTL1 |= BSP_I2C__R_CTL1_B76_UCSSEL;                  // select clock SMCLK bit 7 & 6
    //UCB0TBCNT = 0x07;               // TX 7 bytes of data
    i->I2CSA = 0x68;                // address slave is 0x68
    P3SEL |= 0x03;                    // configure I2C pins (device specific)
                                      // p3.0-sda & p3.1-clk
    i->CTL1 &= ~(UCSWRST);            // eUSCI_B in operational state
    i->IE |= UCRXIE;                  // enable TX-interrupt
    // GIE;                           // general interrupt enable
}

#if 0
...
// inside the eUSCI_B TX interrupt service routine
UCBxTXBUF = 0x77; // fill TX buffer
   BSP__REG_BITS_SET(BSP_I2C__REG_ADR_UCB0CTL1, 
                     BSP_I2C__REG_UCB0CTL1_B0_UCSRST, BSP__REG_MASK_BITS_8);  // Enable SW reset

   BSP__REG_BITS_SET(BSP_I2C__REG_ADR_UCB0CTL0,
                     (BSP_I2C__REG_UCB0CTL0_B21_UCMODEx +   /* Select synchronous mode - i2c */
                     BSP_I2C__REG_UCB0CTL0_B0_UCSYNC),      /* Activate synchronous mode  */
                     BSP__REG_MASK_BITS_8);  

   BSP__REG_BITS_SET(BSP_I2C__REG_ADR_UCB0CTL1, 
                     BSP_I2C__REG_UCB0CTL1_B76_UCSSELx,    /* Select clock source */
                     BSP__REG_MASK_BITS_8);      

#endif

static inline void BSP_I2C__Delay(uint8_t t)
{
    unsigned int x;
    while (t)
    {
        for (x=50000; x !=0; x--){}
        t--;
    }
}

void BSP_I2C__Read(t_i2cBx *i, unsigned char devAddr, 
                               unsigned char dataLen,
                               unsigned char *dataBuffer)
{
    unsigned char byte = i->RXBUF; /* Read buffer to clear RXIFG */
    i->CTL1 |= UCSWRST;    // put eUSCI_B in reset state
    i->I2CSA = devAddr;    // address slave is 12hex
    i->CTL1 &= ~(BSP_I2C__R_CTL1_B4_UCTR);    // clear the UCTR Bit4 for receiver mode.
    i->CTL1 &= ~(UCSWRST);   // eUSCI_B in operational state
    
    
    while (dataLen > 0)
    {

    /* Check that the bus is not busy - busy bit - b4 */
    //led1_toggle();
    //TestPin_toggle();
    //BSP_I2C__Delay(1);
    //while (i->STAT & BSP_I2C__R_STAT_B4_UCBBUSY );
    //led1_toggle();
    //TestPin_toggle();

    TestPin_toggle();
    led1_toggle();
    i->CTL1 |= (BSP_I2C__R_CTL1_B1_UCSTT);  /* Generate start condition STT*/
    led1_toggle();
    TestPin_toggle();

    // Wait until STT is cleared after sending slave addr
    TestPin_toggle();
    led1_toggle();
    while (i->CTL1 & (BSP_I2C__R_CTL1_B1_UCSTT));
    led1_toggle();
    TestPin_toggle();

    // Poll the RXIFG to check if data is received
    TestPin_toggle();
    led1_toggle();
    while (!(i->IFG & (BSP_I2C__R_IFG_B0_UCRXIFG + 0x20 )));
    TestPin_toggle();
    led1_toggle();

    /* retrieve data */
    *dataBuffer = i->RXBUF;
    dataLen--;                   /* decrement dataLen */
    dataBuffer++;                /* setup buffer for next byte from slave */
    /* Send a repeated start */ /* go back to top */ 
    /* end communication STP */

    }
    i->CTL1 |= (BSP_I2C__R_CTL1_B2_UCSTP);
  
  return; 
}
