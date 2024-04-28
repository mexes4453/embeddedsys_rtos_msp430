#include "serial.h"

static char *colorScheme[SERIAL__enTxtColorMax] = 
{
    "\033[0m",
    "\033[1;32m",
    "\033[1;34m",
    "\033[1;31m",
    "\033[1;33m",
    "\033[1;35m",
    "\033[1;36m",
};


/* print numbers according to their base system (2, 10, 16...)*/
static inline uint16_t PrintBaseNbrs(int nbr, uint8_t base, tenSerialNbrCase c)
{
    uint16_t      len = 0;
    unsigned char chr;
    uint8_t       digit;
    
    if ( nbr >= base )
    {
        len += PrintBaseNbrs((nbr/base), base, c);
    }
    digit = (nbr % base);
    chr = SERIAL__NBR_DICT_STR[digit];

    /* Check if the character is either of ABCDE */
    if ( (digit > 9) && (digit < 16) && c == SERIAL__enLowerCase)
    {
        BSP_UART__PutChar( (chr + 32) );
    }
    else
    {
        BSP_UART__PutChar(chr);
    }
    len++;
    return (len);
    
}



/* print decimal, integer, unsigned integer numbers */
static inline uint16_t print_diu(int nbr)
{
    uint16_t len = 0;
    if (nbr < 0)
    {
        BSP_UART__PutChar('-');
        len++;
        nbr *= -1;
    }
    len += PrintBaseNbrs(nbr, 10, SERIAL__enLowerCase);
    return (len);
}


/* Print hexadecimal numbers in upper and lower case */
static inline uint16_t print_xX(uint32_t nbr, char fmt)
{
    uint16_t len = 0;

    BSP_UART__PutString("0x");
    len = 2;

    if (fmt == 'x')
    {
        len += PrintBaseNbrs(nbr, 16, SERIAL__enLowerCase);
    }
    else if (fmt == 'X')
    {
        len += PrintBaseNbrs(nbr, 16, SERIAL__enUpperCase);
    }
    return (len);
}


/* print binary number */
static inline uint16_t print_b(uint32_t nbr)
{
    uint16_t len = 0;

    BSP_UART__PutString("0b");
    len = 2;
    len += PrintBaseNbrs(nbr, 2, SERIAL__enLowerCase);
    return (len);
}



uint32_t SERIAL__Printf(tenSerialTxtColor color, const char *s, ...)   /* Variadic function */
{
    uint32_t len = 0;
    va_list  argList;
    char     fmtSpec;

    if (s == (void *)0) goto escape;

    va_start(argList, s);
    BSP_UART__PutString(colorScheme[color]);
    while(*s != '\0')
    {
        if (*s != '%')
        {
            BSP_UART__PutChar(*s);
            len++;
        }
        else
        {
            fmtSpec = *(s + 1);
            s++;                /* skip '%' to the next character       */
            switch(fmtSpec)     /* print character based on format spec */
            {
                case '%':
                {
                    BSP_UART__PutChar(fmtSpec);
                    len++;
                    break ;
                }
                case 'c':
                {
                    BSP_UART__PutChar((unsigned char)va_arg(argList, int));
                    len++;
                    break ;
                }
                case 'd':
                case 'i':
                case 'u':
                {
                    len += print_diu(va_arg(argList, int));
                    break ;
                }
                case 'x':
                case 'X':
                {
                    len += print_xX(va_arg(argList, unsigned int), fmtSpec);
                    break ;
                }
                case 'b':
                {
                    len += print_b(va_arg(argList, unsigned int));
                    break ;
                }
                default:
                {/*do nothing */}
            }
        }
        s++;
    }
    BSP_UART__PutString(colorScheme[SERIAL__enTxtColorDefault]);
    va_end(argList);
escape:
    return (len);
}

