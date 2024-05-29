#include "os.h"



static tenOsSchedPolicy OS__schedPolicy;
static t_thread         OS__threads[OS__NO_OF_THREADS];
static t_xqueue         OS__threadPool[OS__NO_OF_THREADS];
static t_xqueue        *OS__threadQueueFree, *OS__threadQueueReady; 
static t_thread        *OS__currThread = OS__THREAD_NULL;
t_xqueue               *OS__threadQueueSleep, *OS__threadQueueBlocked;
t_xqueue               *OS__currThreadNode=XQUEUE__NULL;
t_xqueue               *OS__nextThreadNode=XQUEUE__NULL;
uint32_t                OS__switchPeriod = OS__SWITCH_TICK;


static void      OS__TaskIdle(void)
{
    while (1)
    {
        /* flash lights */
        led1_toggle();
        BSP_TIMER__DelayMs(1000);
        led2_toggle();
        BSP_TIMER__DelayMs(1000);

        /* nothing for cpu to do */
        /* switch to low power mode with interrupt */
    }
}

void OS__Init(tenOsSchedPolicy schedPolicy)
{
    OS__schedPolicy = schedPolicy;

    OS__threadQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(OS__threadPool[0]),
                                                                     OS__NO_OF_THREADS,
                                                                     XQUEUE__VOID);
    OS__Fork(OS__TaskIdle, 0, 1); /* 1Hz */

    /**
     * One valid thread is available in ready queue, therefore 
     * it will be schedule as next thread to run.
     * The OS_Sched will skip this thread node if there are non-idle threads
     * in the queue which are ready to run.  */
    OS__nextThreadNode = OS__threadQueueReady;

    /* Convert the threadQueueReady to circular queue by linking the tail to head  */
    OS__threadQueueReady->prev = OS__threadQueueReady->tail;
    OS__threadQueueReady->tail->next = OS__threadQueueReady;
}



tenOsRetCode OS__Fork(f_threadHandler handler, uint8_t priority, uint8_t period)
{
    tenOsRetCode retCode = OS__enRetErrForkFailed;
    t_xqueue     *qEntry = XQUEUE__Dequeue(&OS__threadQueueFree);
    uint8_t      idx;
    t_thread     *t = OS__THREAD_NULL;

    if (!qEntry) goto escape;
    idx = qEntry->qid;                 /* the qid is used as idx for thread array */
    t = &(OS__threads[idx]);
    t->tid = idx;
    t->status = OS__enStatusReady;
    t->period = period;
    t->priority = (OS__schedPolicy == OS__enSchedPolicyRoundRobin) ? 0 : priority;
    t->handler = handler;
    OS__ThreadInit( t );

    /* Embed the thread inside the queue node */
    qEntry->content = (void *)t;
    qEntry->priority = t->priority;
    XQUEUE__StaticEnqueue(&OS__threadQueueReady, qEntry);
    retCode = OS__enRetSuccess;

escape:
    return (retCode);
}


t_thread *OS__GetCurrThread(void)
{
    return (OS__currThread);
}



tenOsRetCode      OS__SetThreadStatus(t_thread *t, tenOsThreadStatus status)
{
    tenOsRetCode retCode = OS__enRetFailure;

    if (!t) goto escape;

    t->status = status;
    retCode = OS__enRetSuccess;

escape:
    return (retCode);
}



tenOsRetCode OS__Kill(t_thread *t)
{
    tenOsRetCode retCode = OS__enRetErrKillFailed;
    t_xqueue     *n = XQUEUE__NULL;

    if (!t) goto escape;

    t->status = OS__enStatusFree;
    n = XQUEUE__DequeueNode(&OS__threadQueueReady, (void *)t);
    if (n)
    {
        XQUEUE__StaticEnqueue(&OS__threadQueueFree, n);
        retCode = OS__enRetSuccess;
    }
escape:
    return (retCode);
}


void OS__ThreadInit(t_thread * const me)
{
    uint8_t idx;

    /* Initialize the stack */
    for (idx=0; idx < OS__STACK_SIZE; idx++)
    {
        me->stack[idx] = 0;
    }
    
    /* Set to default status - Ready */
    /* stack ptr -> R0/PC (align addr to 2) */
    (me->sp) =  (uint32_t *)((((uint32_t)&((me->stack)[0]) +  OS__STACK_SIZE)/ 2) * 2);
    *(--(me->sp)) = 0x000000C0;     /* load SR - Enable SCG0(bit6) & SCG1(7)  */
    *(--(me->sp)) = ((uint32_t)(me->handler));  /* load pc with function to call    */
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


void OS__Sched(void)
{

    if (OS__currThreadNode != OS__nextThreadNode)
    {
        /**
         * This path will be taken during the start of the OS and 
         * It will schedule the thread at the top of the ready queue.
         * The low power mode (idle thread) will be scheduled if its 
         * the only thread in the queue.*/
        /**
         * This path will also be taken when a thread willing relinguish
         * the CPU and schedules the next thread.  */
        //OS__currThreadNode = OS__nextThreadNode;
    }
    else
    {
        /**
         * Subsequent context/TaskSwitch will take this path and will be
         * performed based on the initiated scheduling policy which simply
         * selects the next node in the queue.  */
        /** 
         * This also means that no other thread relinguish the CPU willingly.
        */
        switch (OS__schedPolicy)
        {
            case OS__enSchedPolicyEDF:
            case OS__enSchedPolicyPriority:
            default: /* Roundrobin */
            {
                OS__nextThreadNode = OS__nextThreadNode->next;
            }
        }
    }
    /**
     * Attempt to skip the idle thread if there are other threads
     * in the ready queue.
     * The idle thread is scheduled only if it's the only thread within
     * the queue */
    if ((OS__nextThreadNode->qid == 0) ) 
    {
        if ( XQUEUE__GetLevel(&OS__threadQueueReady) == 2 &&
           (((t_thread *)(OS__nextThreadNode->next->content))->status != OS__enStatusReady)
        )
        { /* schedule idle thread */}
        else 
        {
            OS__nextThreadNode = OS__nextThreadNode->next;
        }
    }
}


void OS__Tswitch(void)
{
    ______disableInt();

    
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

    /* Schedule the next thread/process to run on the CPU - Processor */
    OS__Sched(); 
    OS__currThreadNode = OS__nextThreadNode;
    OS__currThread = (t_thread *)(OS__currThreadNode->content);
    OS__switchPeriod = OS__SWITCH_TICK;
    
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



#if 0
void      OS__Suspend(int evtSig)
{
    t_thread *t = (t_thread *)(OS__currThreadNode->content);
    BSP__CriticalStart();
    t->event = evtSig;
    t->status = OS__enStatusSuspended;
    OS__Tswitch();
    BSP__CriticalEnd();
}



void      OS__Resume(int evtSig)
{
    t_thread *t = (t_thread *)(OS__currThreadNode->content);

    BSP__CriticalStart();
    while (t->next != OS__currThread)
    {
        t = t->next;
        if ((t->status == OS__enStatusSuspended) && 
            (t->event == evtSig))
        {
            t->status = OS__enStatusReady;
            //XQUEUE__StaticEnqueue(&OS__threadQueueReady, t);
            //OS__EnqueueThread(&OS__readyQueue, t);
        }
    }
    BSP__CriticalEnd();
}




/*----------------------------------------------------------------*/
/*                          Semaphore                             */
/*----------------------------------------------------------------*/

int OS__Block(t_osSemaphore *s)
{
    t_thread *t = (t_thread *)(s->threadQueue->content);
    t->status = OS__enStatusBlocked;
    XQUEUE__StaticEnqueue(&(s->threadQueue), t);
    //OS__EnqueueThread(&(s->threadQueue), OS__currThread);
    OS__Tswitch();
}



int OS__Signal(t_osSemaphore *s)
{
    t_thread *t = OS__DequeueThread(&s->threadQueue);
    t->status = OS__enStatusReady;
    XQUEUE__StaticEnqueue(&OS__threadQueueReady, t);
    //OS__EnqueueThread(&(OS__readyQueue), t);
}



int OS__P_CountSemaphore(t_osSemaphore *s)
{

    BSP__CriticalStart();
    s->value--;
    if (s->value < 0)
    {
        OS__Block(s);
    }
    BSP__CriticalEnd();
}



int OS__V_CountSemaphore(t_osSemaphore *s)
{

    BSP__CriticalStart();
    s->value++;
    if (s->value <= 0)
    {
        OS__Signal(s);
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
        OS__Block(s);
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
        OS__Signal(s);
    }
    BSP__CriticalEnd();
}
#endif
