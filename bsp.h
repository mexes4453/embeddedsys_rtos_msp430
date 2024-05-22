#ifndef BSP_H
#define BSP_H

#include <msp430.h>
# include <stdint.h>
# include "led.h"

// user guide
//https://www.ti.com/lit/ug/slau208q/slau208q.pdf?ts=1712822562409&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FMSP430F6779%253Futm_source%253Dgoogle%2526utm_medium%253Dcpc%2526utm_campaign%253Depd-msp-null-44700045336317482_prodfolderdynamic-cpc-pf-google-eu_int%2526utm_content%253Dprodfolddynamic%2526ds_k%253DDYNAMIC+SEARCH+ADS%2526DCM%253Dyes%2526gad_source%253D1%2526gclid%253DEAIaIQobChMIqbu7mtm5hQMVv5VQBh0DZgjFEAAYASAAEgL4xfD_BwE%2526gclsrc%253Daw.ds

//datasheet
//https://www.ti.com/lit/ds/symlink/msp430f5529.pdf?ts=1713014033864&ref_url=https%253A%252F%252Fwww.mouser.com%252F

#define _IO_W_REG_    volatile unsigned short 
#define _IO_B_REG_    volatile uint8_t
#define WDT_CTL    (*((unsigned short volatile *)0x015C))
#define WDT_PW     (0x5A00)
#define WDT_HOLD   (1U << 7) 

#define BSP__DISABLE_WDT() { WDT_CTL = WDT_PW | WDT_HOLD; }


void ______enableInt(void);
void ______disableInt(void);

#define BSP__CRITICAL_SECTION_START() ______disableInt()
#define BSP__CRITICAL_SECTION_END()   ______enableInt()
/* Function declaration */
void BSP__ConfigureClock(void);
//void BSP__EnableInterrupt(void);



#endif /* BSP_H */