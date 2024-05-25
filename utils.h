#ifndef UTILS_H
#define UTILS_H

/* -----------------------------------*/
/*               MACROS               */
/* -----------------------------------*/

/* Defines the null pointer as defined in stdlib.h */
#define UTILS_NULL ((void *)0)



/*
 * This macro emulates the memcpy function in <string.h>
 * It copies the byte data from source address to destination
 * address given the number of bytes to copy.
 * @dstAddr : ptr to destination buffer
 * @srcAddr : ptr to source buffer
 * @byteCnt : Number of bytes to copy from src to dst.
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use when enough buffer is provided for dst.
 */
#define UTILS_MEMCPY(dstAddr,srcAddr,byteCnt)\
    if (((dstAddr) != UTILS_NULL) && ((srcAddr) != UTILS_NULL))\
    {\
        for (int idx=0; idx<(byteCnt); idx++)\
        {\
            (dstAddr)[idx] = (srcAddr)[idx];\
        }\
    }\



/*
 * This macro emulates the memset function in <string.h>
 * It write the given value to destination address, given the
 * number of bytes to write.
 * @addr    : ptr to destination buffer
 * @value   : data to write
 * @byteCnt : Number of bytes
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use when enough buffer is provided for addr.
 */
#define UTILS__MEMSET(addr,value,byteCnt)\
{\
    if ((addr) != UTILS_NULL)\
    {\
        for (int unsigned idx=0; idx<(byteCnt); idx++)\
        {\
            ((unsigned char *)(addr))[idx] = (value);\
        }\
    }\
}



/*
 * This macro is used to assert that a given condition is true
 * It locks the processor in a while loop if the condition is 
 * evaluated to be false.
 * This macro helps to avoid unnecessary use of if else statement
 * to check if a condition is true or false.
 * It is also helpful in catching bugs in a piece of code.
 * */
#define UTILS_ASSERT(cond) \
    if (!cond) while (1); \


/*
 * This macro reads the value of a bit in a register or memory address
 * @regAddr   ptr to destination buffer
 * @bitPos  : position of the bit on the reg
 * @regMask : indicates the number of bits in register
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use to read single bit value at a time.
 */
#define UTILS_SINGLE_REG_BIT_READ(regAddr, bitPos, regMask) \
((*(regAddr)) & ((1 << (bitPos)) & (regMask)))




/*
 * This macro set the value of a bit in a register or memory address
 * @regAddr   ptr to destination buffer
 * @bitPos  : position of the bit on the reg
 * @regMask : indicates the number of bits in register
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use to set single bit value at a time.
 */
#define UTILS_SINGLE_REG_BIT_SET(regAddr, bitPos, regMask) \
((*(regAddr)) | ((1 << (bitPos)) & (regMask)))



/*
 * This macro clears the value of a bit in a register or memory address
 * @regAddr   ptr to destination buffer
 * @bitPos  : position of the bit on the reg
 * @regMask : indicates the number of bits in register
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use to clear single bit value at a time.
 */
#define UTILS_SINGLE_REG_BIT_UNSET(regAddr, bitPos, regMask) \
((*(regAddr)) & ~((1 << (bitPos)) & (regMask)))



/*
 * This macro is returns the bit value of a register given the bitmask
 * @regAddr : Register to be configured (clear bits values)
 * @bitMask : Bits to be cleared on register
 *          : Example -> 0x03. Clears bits 0 and 1 on given register
 * @regMask : Specifies the mask for the register bit size and
 *          : also indicates the bit size of the register.
 * */
#define UTILS_REG_BITS_READ(regAddr, bitMask, regMask)\
    (*(regAddr) & ((bitMask) & (regMask)))

#endif /* UTILS_H */