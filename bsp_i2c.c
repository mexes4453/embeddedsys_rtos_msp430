#include "bsp_i2c.h"


void BSP_I2C__Init(t_i2cBx *i)
{
    i->CTL1 |= UCSWRST;               // put eUSCI_B in reset state
    i->CTL0 |= UCMODE_3 + UCMST;      // I2C master mode
    i->CTL0 |= 0x01;                  // I2C sychronous mode
    i->BR0 = 8;                      // baud rate = SMCLK / 10
    i->CTL1 |= 0x04;                  // bit3 automatic STOP assertion
    i->CTL1 |= 0x0C;                  // select clock SMCLK
    //UCB0TBCNT = 0x07;               // TX 7 bytes of data
    i->I2CSA = 0x0068;                // address slave is 12hex
    P3SEL |= 0x01;                    // configure I2C pins (device specific)
                                      // p2.1-sda & p2.2-clk
    i->CTL1 &= ~(UCSWRST);            // eUSCI_B in operational state
    i->IE |= UCTXIE;                  // enable TX-interrupt
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



void BSP_I2C__Read(t_i2cBx *i, unsigned char devAddr, 
                               unsigned char dataLen,
                               unsigned char *dataBuffer)
{
    int x=0;
  i->CTL1 |= UCSWRST;    // put eUSCI_B in reset state
  i->I2CSA = devAddr;    // address slave is 12hex
  i->CTL1 &= ~(0x10);    // clear the UCTR Bit4 for receiver mode.
  i->CTL1 &= ~(UCSWRST);   // eUSCI_B in operational state
  
  /* Check that the bus is not busy - busy bit - b4 */
  led1_toggle();
  while (i->STAT & 0x10 );
  for (x=500000; x !=0; x--);
  led1_toggle();
  while (dataLen != 0)
  {
      i->CTL1 |= (0x02);  /* Generate start condition STT*/
  
      // Wait until STT is cleared after sending slave addr
      while (i->CTL1 & (0x02));
      led1_toggle();

      // Poll the RXIFG to check if data is received
      while (i->IFG & (0x01));
      led2_toggle();
      *dataBuffer = i->TXBUF;
      dataLen--;                   /* decrement dataLen */
      dataBuffer++;                /* setup buffer for next byte from slave */
      led2_toggle();
      /* Send a repeated start */ /* go back to top */ 
  }
  
  /* end communication STP */
  i->CTL1 |= (0x04);
  
  return; 
}
