
#ifndef OS_H
#define OS_H
#include <stdint.h>

#define NO_OF_THREADS        2
#define NO_OF_CPU_REGS       15
#define OS__STACK_SIZE       100 /* 100 * 4bytes (32bits)      */
#define OS__SWITCH_TICK      100 /* context switch every 100ms */

typedef struct s_thread t_thread;

struct s_thread
{
    uint32_t  *sp;
    t_thread  *next;
    uint8_t   tid;
    uint32_t  stack[OS__STACK_SIZE];
};

typedef void (*f_threadHandler)(void);

void OS__ThreadInit(t_thread * const me, f_threadHandler handler,
                                         uint8_t id,
                                         t_thread *next);
void OS__Tswitch(void);
void OS__Sched(void);

#endif /* OS_H */
