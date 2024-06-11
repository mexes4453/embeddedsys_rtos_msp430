#include "utils.h"




inline void UTILS__Memset(void *addr, char value, unsigned int byteCnt)
{
    if (addr != UTILS_NULL)
    {
        while (byteCnt)
        {
            (*(char *)addr) = value;
            addr++;
            byteCnt--;
        }
    }
}