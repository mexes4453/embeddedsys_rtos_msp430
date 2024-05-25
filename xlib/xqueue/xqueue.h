#ifndef XQUEUE_H
#define XQUEUE_H

/**
 * The queue or xqueue uses a priority scheme with highest 
 * number having the highest priority 
 * 
 */

typedef enum e_xqueueType
{
    XQUEUE__enPriorityFifo=0,
    XQUEUE__enLifo,
}   tenXqueueType;


typedef struct s_xqueue t_xqueue;

struct s_xqueue
{
	void			*content;
    unsigned int    priority;
    tenXqueueType   type;
    unsigned int    capacity;
	t_xqueue        *next;
	t_xqueue        *prev;
	t_xqueue        *tail;
    unsigned char   qid;
};


typedef enum e_xqueueRetCode
{
    XQUEUE__enRetErrNullPtrAccess = -1,
    XQUEUE__enRetSuccess=0,
    XQUEUE__enRetFailure,
    XQUEUE__enRetErrFull,
    XQUEUE__enRetErrDupEnqueueAttempt,
}   tenXqueueRetCode;



#define XQUEUE__NULL    (t_xqueue *)0
#define XQUEUE__VOID    (void *)0
#define XQUEUE__NODE_SZ (sizeof(t_xqueue))

t_xqueue         *XQUEUE__StaticInit(tenXqueueType t, 
                                     t_xqueue *queue,
                                     unsigned int queueSize,
                                     void *item);
unsigned int      XQUEUE__GetLevel(t_xqueue **q);
t_xqueue         *XQUEUE__Dequeue(t_xqueue **q);
char              XQUEUE__IsDuplicate(t_xqueue **q, t_xqueue *item);
tenXqueueRetCode  XQUEUE__StaticEnqueue(t_xqueue **q, t_xqueue *item);
void              XQUEUE__EnqueueOnPriority(t_xqueue **q, t_xqueue *item);
void              XQUEUE__StaticDeInit(t_xqueue **q);
tenXqueueRetCode  XQUEUE__StaticEnqueueNewItem(t_xqueue **rscQueue,
                                               t_xqueue **opsQueue,
                                               void *item,
                                               unsigned int priority);
void             *XQUEUE__GetItem(t_xqueue *q);
t_xqueue         *XQUEUE__FindNode(t_xqueue **q, void *item);
t_xqueue         *XQUEUE__DequeueNode(t_xqueue **q, void *item);


#endif /* XQUEUE_H */
