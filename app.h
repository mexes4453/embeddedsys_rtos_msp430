
#ifndef APP_H
#define APP_H

#include "dev_led.h"
#include "os.h"
#include "serial.h"

#define APP__CRITICAL_START() BSP__CRITICAL_SECTION_START()
#define APP__CRITICAL_END()   BSP__CRITICAL_SECTION_END() 

void APP__Task1(void);
void APP__Task2(void);
void APP__Task3(void);
void APP__Task4(void);

#endif /* APP_H */
