#include "os.h"


t_thread *currThread;

void OS__ThreadInit(t_thread * const me, f_threadHandler handler,
                                         uint8_t id,
                                         t_thread *next)
{
    me->next = next;
    me->tid = id;

    /* Initialise the stack */
    for (uint8_t idx=0; idx < STACK_SIZE; idx++)
    {
        me->stack[idx] = 0;
    }
     
    /* stack ptr -> R0/PC (align addr to 2) */
    (me->sp) =  (uint16_t *)((((uint16_t)&((me->stack)[0]) +  STACK_SIZE)/ 2) * 2);  
    *(--(me->sp)) = 0xC8;                 /* load SR  */
    *(--(me->sp)) = (uint16_t)handler;    /* load pc with function to call */
    *(--(me->sp)) = 0x0F;    /* R15 */ 
    *(--(me->sp)) = 0x0E;    /* R14 */ 
    *(--(me->sp)) = 0x0D;    /* R13 */
    *(--(me->sp)) = 0x0C;    /* R12 */
    *(--(me->sp)) = 0x0B;    /* R11 */
    *(--(me->sp)) = 0x0A;    /* R10 */
    *(--(me->sp)) = 0x09;    /* R9  */
    *(--(me->sp)) = 0x08;    /* R8  */
    *(--(me->sp)) = 0x07;    /* R7  */
    *(--(me->sp)) = 0x06;    /* R6  */
    *(--(me->sp)) = 0x05;    /* R5  */
    *(--(me->sp)) = 0x04;    /* R4  */
    *(--(me->sp)) = 0x03;    /* R3  */
}

extern t_thread t1;

void OS__Sched(void)
{
    if (currThread == (t_thread *)0)
    {
        currThread = &t1;
    }
    else
    {
        currThread = currThread->next; /* Round Robin */
    }
}


void OS__Tswitch(void)
{
    //void *sp = (void *)0;

    /* disable interrupt */ 
    __asm("NOP\n\t BIC.W #0008h, SR \n\t NOP \n\t");

    {
        /* push registers to stack */
        //currThread->sp = sp;
        if ( currThread != (t_thread *)0)
        {
            __asm(
                  "PUSH R15 \n\t"
                  "PUSH R14 \n\t"
                  "PUSH R13 \n\t"
                  "PUSH R12 \n\t"
                  "PUSH R11 \n\t"
                  "PUSH R10 \n\t"
                  "PUSH R9 \n\t"
                  "PUSH R8 \n\t"
                  "PUSH R7 \n\t"
                  "PUSH R6 \n\t"
                  "PUSH R5 \n\t"
                  "PUSH R4 \n\t"
                  "PUSH R3 \n\t"
                  "MOV #currThread, R12 \n\t"  /* R12 = &currThread */
                  "MOV @R12, R12        \n\t"  /* R12 = currThread => currThread->sp */
                  "MOV SP, @R12         \n\t"  /* COPY the curr sp to the currThread->sp */
                  );

        }
    }
    OS__Sched(); 
    //sp = currThread->sp;
    /* pop registers to current stack */
    __asm(
          "MOV #currThread, R12 \n\t"  /* R12 = &currThread */
          "MOV @R12, R12        \n\t"  /* R12 = currThread => currThread->sp */
          "MOV @R12, SP         \n\t"  /* COPY the curr sp to the currThread->sp */
          "POP R3 \n\t"
          "POP R4 \n\t"
          "POP R5 \n\t"
          "POP R6 \n\t"
          "POP R7 \n\t"
          "POP R8 \n\t"
          "POP R9 \n\t"
          "POP R10 \n\t"
          "POP R11 \n\t"
          "POP R12 \n\t"
          "POP R13 \n\t"
          "POP R14 \n\t"
          "POP R15 \n\t"
          );

    /*  enable interrupt */
    __asm("NOP\n\t BIS.W #0008h, SR \n\t NOP \n\t");
    __asm("RETI \n\t");
}
