#include "os.h"


extern void ______enableInt(void);
extern void ______disableInt(void);
static t_thread *OS__currThread;
uint8_t  OS__switchPeriod = OS__SWITCH_TICK;

void OS__ThreadInit(t_thread * const me, f_threadHandler handler,
                                         uint8_t id,
                                         t_thread *next)
{
    uint8_t idx;
    me->next = next;
    me->tid = id;


    /* Initialize the stack */
    for (idx=0; idx < OS__STACK_SIZE; idx++)
    {
        me->stack[idx] = 0;
    }
     
    /* stack ptr -> R0/PC (align addr to 2) */
    (me->sp) =  (uint32_t *)((((uint32_t)&((me->stack)[0]) +  OS__STACK_SIZE)/ 2) * 2);
    *(--(me->sp)) = 0x000000C0;     /* load SR - Enable SCG0(bit6) & SCG1(7)  */
    *(--(me->sp)) = ((uint32_t)handler);  /* load pc with function to call    */
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
    if (OS__currThread == (t_thread *)0)
    {
        OS__currThread = &t1;
    }
    else
    {
        OS__currThread = OS__currThread->next; /* Round Robin */
    }
}


void OS__Tswitch(void)
{
    ______disableInt();

    {
        
        if ( OS__currThread != (t_thread *)0)
        {
            /**
             * Save the context of the current thread onto its private stack.
             * prior to scheduling of the next thread/process */
            __asm(
                  " PUSHM.A #12, r15          \n\t"  /* push multiple registers (R4-R15) to stack */
                  " MOVA &OS__currThread, R12 \n\t"  /* R12 = &currThread = currThread->sp        */
                  " MOVA SP, 0x0(R12)         \n\t"  /* COPY the curr sp to the currThread->sp    */
                  );
        }
    }

    /* Schedule the next thread/process to run on the CPU - Processor */
    OS__Sched(); 
    
    /* Restore the context of newly scheduled thread from its private stack */
    __asm(
          " MOVA &OS__currThread, R12 \n\t"  /* R12 = &currThread = currThread->sp             */
          " MOVA @R12, SP             \n\t"  /* COPY the curr sp to the currThread->sp         */
          " POPM.A #12, r15           \n\t"  /* pop multiple registers (R4-R15) as address off */
                                             /* the current thread/process stack */
         );

    ______enableInt();

    /* Return and pop the PC off the stack of current thread/process */
    __asm__(" RETA \n\t");
}

