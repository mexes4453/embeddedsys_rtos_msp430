#include "os.h"


t_thread *currThread;
extern void ______enableInt(void);
extern void ______disableInt(void);

void OS__ThreadInit(t_thread * const me, f_threadHandler handler,
                                         uint8_t id,
                                         t_thread *next)
{
    uint8_t idx;
    me->next = next;
    me->tid = id;


    /* Initialize the stack */
    for (idx=0; idx < STACK_SIZE; idx++)
    {
        me->stack[idx] = 0;
    }
     
    /* stack ptr -> R0/PC (align addr to 2) */
    (me->sp) =  (uint32_t *)((((uint32_t)&((me->stack)[0]) +  STACK_SIZE)/ 2) * 2);
    *(--(me->sp)) = 0xAA;     /* load SR  */
    *(--(me->sp)) = 0xAB;     /* load SR  */
    *(--(me->sp)) = ((uint32_t)handler);  /* load pc with function to call */
    *(--(me->sp)) = 0x0000FF0F;    /* R15 */
    *(--(me->sp)) = 0x0000FF0E;    /* R13 */
    *(--(me->sp)) = 0x0000FF0D;    /* R13 */
    *(--(me->sp)) = 0x0000FF0C;    /* R12 */
    *(--(me->sp)) = 0x0000FF0B;    /* R11 */
    *(--(me->sp)) = 0x0000FF0A;    /* R10 */
    *(--(me->sp)) = 0x0000FF09;    /* R09 */
    *(--(me->sp)) = 0x0000FF08;    /* R08 */
    *(--(me->sp)) = 0x0000FF07;    /* R07 */
    *(--(me->sp)) = 0x0000FF06;    /* R06 */
    *(--(me->sp)) = 0x0000FF05;    /* R05 */
    *(--(me->sp)) = 0x0000FF04;    /* R04 */
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
    ______disableInt();

    {
        /* push registers to stack */
        //currThread->sp = sp;
        if ( currThread != (t_thread *)0)
        {
            __asm(" pushm.a #12, r15;"
                  " MOV #currThread, R12 \n\t"  /* R12 = &currThread */
                  " MOV @R12, R12        \n\t"  /* R12 = currThread => currThread->sp */
                  " MOV SP, R12         \n\t"  /* COPY the curr sp to the currThread->sp */
                  );
        }
    }
    OS__Sched(); 
    //sp = currThread->sp;
    /* pop registers to current stack */
    __asm(
          " MOV #currThread, R12 \n\t"  /* R12 = &currThread */
          " MOV @R12, R12        \n\t"  /* R12 = currThread => currThread->sp */
          " MOV @R12, SP         \n\t"  /* COPY the curr sp to the currThread->sp */
          " popm.a #12, r15      \n\t"
);
//    "; mov.w @SP, r14 \n\t"
//    "; nop \n\t bis.w r14, SR  \n\t"
//    "; add #2, SP \n\t"
//    "; pop.a PC \n\t"

    /*  enable interrupt */
    ______enableInt();
    __asm__(" RETA \n\t");
}
