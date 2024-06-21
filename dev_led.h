#ifndef DEV_LED_H
#define DEV_LED_H
#include <msp430.h>

#define DEV_LED__BITMASK_GREEN     0x80
#define DEV_LED__BITMASK_RED       0x01


void DEV_LED___Init(void);
void DEV_LED__ToggleRed(void);
void DEV_LED__ToggleGreen(void);
void DEV_LED__OffRed(void);
void DEV_LED__OffGreen(void);
/* ----------------------------------------*/
void Init_TestPin(void);
void TestPin_toggle(void);



#endif /* DEV_LED_H */
