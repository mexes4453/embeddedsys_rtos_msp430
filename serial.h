#ifndef SERIAL_H
#define SERIAL_H

#include "bsp_uart.h"
# include <stdarg.h>
# include <stddef.h>

#define SERIAL__NBR_DICT_STR "0123456789ABCDEF"

typedef enum
{
    SERIAL__enLowerCase,
    SERIAL__enUpperCase,
}   tenSerialNbrCase;

typedef enum
{
    SERIAL__enTxtColorDefault,
    SERIAL__enTxtColorGreen,
    SERIAL__enTxtColorBlue,
    SERIAL__enTxtColorRed,
    SERIAL__enTxtColorYellow,
    SERIAL__enTxtColorMagenta,
    SERIAL__enTxtColorCyan,
    SERIAL__enTxtColorMax,
}   tenSerialTxtColor;

uint32_t SERIAL__Printf(tenSerialTxtColor color, const char *s, ...);   /* Variadic function */



#endif  /* SERIAL_H  */