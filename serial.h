#ifndef SERIAL_H
#define SERIAL_H

#include "bsp_uart.h"
# include <stdarg.h>
# include <stddef.h>

#define SERIAL__NBR_DICT_STR "0123456789ABCDEF"

typedef enum e_serialNbrCase
{
    SERIAL__enLowerCase = 0,
    SERIAL__enUpperCase,
}   tenSerialNbrCase;

typedef enum e_serialTxtColor
{
    SERIAL__enTxtColorDefault = 0,
    SERIAL__enTxtColorGreen,
    SERIAL__enTxtColorBlue,
    SERIAL__enTxtColorRed,
    SERIAL__enTxtColorYellow,
    SERIAL__enTxtColorMagenta,
    SERIAL__enTxtColorCyan,
    SERIAL__enTxtColorMax,
}   tenSerialTxtColor;

uint32_t SERIAL__Printf(tenSerialTxtColor color, const char *s, ...);   /* Variadic function */

#define SERIAL__PRINTF_0(txt, ...) SERIAL__Printf(SERIAL__enTxtColorDefault, txt, __VA_ARGS__)
#define SERIAL__PRINTF_1(color, txt, ...) SERIAL__Printf(color, txt, __VA_ARGS__)


#endif  /* SERIAL_H  */
