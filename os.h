/**
 * @file os.h
 * @author Chiemezie Udoh (chiemezieudoh@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-21
 * 
 * @copyright Copyright (c) 2024
 * 
 * This ScratchOs is a loosely couple operating systems for micro controller.
 * which employs the use of Active Objects suitable to manage various components
 * of its operating system. (10 Active objects)
 */

#ifndef OS_H
#define OS_H
#include <stdint.h>
#include "./xlib/xqueue/xqueue.h"

#define OS__NO_OF_THREADS        10
#define NO_OF_CPU_REGS       15
#define OS__STACK_SIZE       100 /* 100 * 4bytes (32bits)      */
#define OS__SWITCH_TICK      1 /* context switch every 1ms */
#define OS__VOID             (void *)0



typedef struct s_thread t_thread;
typedef void (*f_threadHandler)(void);
typedef enum
{
    OS__enStatusReady=0,
    OS__enStatusRunning,
    OS__enStatusBlocked,
    OS__enStatusSleep,
    OS__enStatusFree,
    OS__enStatusSuspended
}   tenOsThreadStatus;



typedef enum e_schedPolicy
{
    OS__enSchedPolicyRoundRobin = 0,
    OS__enSchedPolicyPriority,
    OS__enSchedPolicyEDF,        /* Earliest deadline first */
}   tenOsSchedPolicy;




typedef enum
{
    OS__enEvtSigTimer=0,
}   tenOsEvtSig;



struct s_thread
{
    uint32_t          *sp;
    uint8_t           tid;
    uint32_t          stack[OS__STACK_SIZE];
    tenOsThreadStatus status;
    tenOsEvtSig       event;
    uint8_t           period;
    uint8_t           priority;
    f_threadHandler   handler;
    uint8_t           deadline;
};

typedef struct 
{
    int value;
    t_xqueue *threadQueue; /* Queue to add threads waiting on semaphore */
}   t_osSemaphore;


typedef enum e_osRetCode
{
    OS__enRetSuccess = 0,
    OS__enRetErrForkFailed,
} tenOsRetCode;

#define OS__THREAD_NULL      (t_thread *)0


/*-----------------------------------------------------------------------*/
/*                      FUNCTION DECLARATIONS                            */
/*-----------------------------------------------------------------------*/

extern void ______enableInt(void);
extern void ______disableInt(void);
extern void inline BSP__CriticalStart(void);
extern void inline BSP__CriticalEnd(void);

void      OS__Init(tenOsSchedPolicy schedPolicy);
void      OS__LowPoweMode(void);
void      OS__ThreadInit(t_thread * const me);
void      OS__Tswitch(void);
void      OS__Sched(void);
void      OS__EnqueueThread(t_thread **threadQueue, t_thread *newThread);
t_thread *OS__DequeueThread(t_thread **threadQueue);
void      OS__Suspend(int evtSig);
void      OS__Resume(int evtSig);
int       OS__Block(t_osSemaphore *s);
int       OS__Signal(t_osSemaphore *s);
int       OS__P_CountSemaphore(t_osSemaphore *s);
int       OS__V_CountSemaphore(t_osSemaphore *s);
int       OS__P_BinSemaphore(t_osSemaphore *s);
int       OS__V_BinSemaphore(t_osSemaphore *s);

#endif /* OS_H */
