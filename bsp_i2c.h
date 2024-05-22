ifndef BSP_I2C_H
#define BSP_I2C_H
//#include "io430f5659.h"
#include "bsp.h"
#include "led.h"


#define BSP_I2C__BUF_SZ          (30)
#define BSP_I2C__B0_BASE_ADDR    (0x5E0U)

typedef struct
{
    _IO_B_REG_ CTL1;          /* offset 0x00 - control register 1             */
    _IO_B_REG_ CTL0;          /* offset 0x01 - control register 0             */
    _IO_B_REG_ RESERVED_0[4]; /* offset 0x02                                  */
    _IO_B_REG_ BR0;           /* offset 0x06 - Baud rate - 0                  */
    _IO_B_REG_ BR1;           /* offset 0x07 - Baud rate - 1                  */
    _IO_B_REG_ RESERVED_1[2]; /* offset 0x08                                  */
    _IO_B_REG_ STAT;          /* offset 0x0A - Status Register                */
    _IO_B_REG_ RESERVED_2[1]; /* offset 0x0B                                  */
    _IO_B_REG_ RXBUF;         /* offset 0x0C - Transmit buffer                */
    _IO_B_REG_ RESERVED_3[1]; /* offset 0x0D                                  */
    _IO_B_REG_ TXBUF;         /* offset 0x0E - Transmit buffer                */
    _IO_B_REG_ RESERVED_4[1]; /* offset 0x0F                                  */
    _IO_W_REG_ I2C0A;         /* offset 0x10 - Auto baudrate control register */
    _IO_W_REG_ I2CSA;         /* offset 0x12 - IrDA transmit control reg      */
    _IO_B_REG_ RESERVED_6[8]; /* offset 0x14                                  */
    _IO_B_REG_ IE;            /* offset 0x1C - Interrupt enable register      */
    _IO_B_REG_ IFG;           /* offset 0x1D - Interrupt flag register        */
    _IO_W_REG_ IV;            /* offset 0x1E - Interrupt Vector register      */
}   t_i2cBx;




#define BSP_I2C__B0        ((t_i2cBx *)BSP_I2C__B0_BASE_ADDR)

void BSP_I2C__Init(t_i2cBx *i);
void BSP_I2C__Read(t_i2cBx *i, unsigned char devAddr, 
                               unsigned char dataLen,
                               unsigned char *dataBuffer);

#endif