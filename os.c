#include "os.h"


static t_thread *OS__currThread;
static t_thread *OS__readyQueue, *OS__blockedQueue, *OS__freeList;
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
                  " PUSHM.A PC                \n\t"  /* store the next instruction on stack       */
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





void      OS__EnqueueThread(t_thread **threadQueue, t_thread *newThread)
{
    t_thread *idxThread;

    /* Check that the address of the queue and that of the thread is valid */
    if (!threadQueue || !newThread) goto escape;
    idxThread = *threadQueue;
    
    /* Check if queue is empty and add new item immediately */
    if ( !idxThread ) 
    {
        *threadQueue = newThread;
        newThread->next = OS__THREAD_NULL;
        goto escape;
    }

    /* May be the head is the right position */
    if (newThread->priority > idxThread->priority)
    {
        newThread->next = idxThread;
        *threadQueue = newThread;
        goto escape;
    }

    /* Add the new thread to the queue */
    while ( idxThread->next != OS__THREAD_NULL )
    {
        if ( idxThread->next->priority < newThread->priority )
        {
            newThread->next = idxThread->next;
            idxThread->next = newThread;
            goto escape;
        }
        idxThread = idxThread->next;
    }

    /* Maybe the tail is the right position */
    idxThread->next = newThread;
    newThread->next = OS__THREAD_NULL;

escape:
    return ;
}




t_thread *OS__DequeueThread(t_thread **threadQueue)
{
    t_thread *head = OS__THREAD_NULL;

    /* Check that the queue pointer is valid */
    if (!threadQueue) goto escape;
    head = *threadQueue;

    /* Check the queue head is a valid point - queue not empty */
    if (!head) goto escape;

    /**
     * Remove head from the top of the queue by assigning next thread
     * as the new head */
    *threadQueue = head->next;

escape:
    return (head);
}

void      OS__Suspend(int evtSig)
{
    BSP__CriticalStart();
    OS__currThread->event = evtSig;
    OS__currThread->status = OS__enStatusSuspended;
    OS__Tswitch();
    BSP__CriticalEnd();
}



void      OS__Resume(int evtSig)
{
    t_thread *t = OS__currThread;

    BSP__CriticalStart();
    while (t->next != OS__currThread)
    {
        t = t->next;
        if ((t->status == OS__enStatusSuspended) && 
            (t->event == evtSig))
        {
            t->status = OS__enStatusReady;
            OS__EnqueueThread(&OS__readyQueue, t);
        }
    }
    BSP__CriticalEnd();
}




/*----------------------------------------------------------------*/
/*                          Semaphore                             */
/*----------------------------------------------------------------*/

int OS__Block(t_osSemaphore *s)
{
    OS__currThread->status = OS__enStatusBlocked;
    OS__EnqueueThread(&(s->threadQueue), OS__currThread);
    OS__Tswitch();
}



int OS__Signal(t_osSemaphore *s)
{
    t_thread *t = OS__DequeueThread(&s->threadQueue);
    t->status = OS__enStatusReady;
    OS__EnqueueThread(&(OS__readyQueue), t);
}



int OS__P_CountSemaphore(t_osSemaphore *s)
{

    BSP__CriticalStart();
    s->value--;
    if (s->value < 0)
    {
        block(s);
    }
    BSP__CriticalEnd();
}



int OS__V_CountSemaphore(t_osSemaphore *s)
{

    BSP__CriticalStart();
    s->value++;
    if (s->value <= 0)
    {
        signal(s);
    }
    BSP__CriticalEnd();
}



int OS__P_BinSemaphore(t_osSemaphore *s)
{

    BSP__CriticalStart();
    s->value--;
    if (s->value == 1)
    {
        s->value = 0;
    }
    else
    {
        block(s);
    }
    BSP__CriticalEnd();
}



int OS__V_BinSemaphore(t_osSemaphore *s)
{

    BSP__CriticalStart();
    if (s->threadQueue == 0)
    {
        s->value = 1;
    }
    else
    {
        signal(s);
    }
    BSP__CriticalEnd();
}