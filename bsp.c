
#include "bsp.h"


void BSP__ConfigureClock(void)
{
    // Set DCO range and frequency
    UCSCTL1 = DCORSEL_7; // Set DCO range to 16MHz

    // Set ACLK, MCLK, and SMCLK sources and dividers
    UCSCTL2 = FLLD_0 + 0x00; // MCLK = SMCLK = 16MHz, ACLK sourced from VLO
    UCSCTL3 = SELREF__REFOCLK; // FLL reference = REFO (internal 32kHz oscillator)
    UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK; // ACLK = REFO, SMCLK and MCLK = DCOCLK
}






