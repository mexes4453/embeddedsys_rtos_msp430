#include "xqueue.h"



t_xqueue *XQUEUE__StaticInit(tenXqueueType t, t_xqueue *queue,
                                              unsigned int queueSize,
                                              void *item)
{
    t_xqueue     *head = XQUEUE__NULL;
    t_xqueue     *currNode = XQUEUE__NULL;
    unsigned int idx=0;

    /* Check that the pointer arguments are valid */
    if (!queue) goto escape;
    
    /* Check that nbr of item is greater than 0 */
    if (!queueSize) goto escape;
    head = queue;
    /* Initialise and link all the nodes */
    for (idx=0; idx < queueSize; idx++)
    {
        currNode = queue + (idx);            /* next item in array of queue item */
        currNode->capacity = queueSize;
        currNode->priority = 0;
        currNode->type = t;
        currNode->content = XQUEUE__VOID;
        currNode->next = (currNode + 1);
        currNode->prev = (currNode - 1);
        currNode->qid = idx;
    }
    /**
     * At this point, the last item in the queue array has its next pointing
     * to an location outside the queue array size range.
     * The last item should point to NULL*/
    currNode->next = XQUEUE__NULL;
    head->tail = currNode;         /* update the tail address on the head node of queue */
    head->prev = XQUEUE__NULL;

    /* Load data (item) into the queue head if available */
    /* This can occur during the queue initialisation user could
     * provide null value if valid item is not provided */
    head->content = (item ? item : XQUEUE__VOID);
escape:
    return (head);
}




unsigned int XQUEUE__GetLevel(t_xqueue **q)
{
    unsigned int entryCnt = 0;
    t_xqueue     *head = XQUEUE__NULL;
    t_xqueue     *tail = XQUEUE__NULL;

    /* Check that the pointer arguments are valid */
    if (!q) goto escape;

    head = *q;
    if (!head) goto escape;
    tail = head->tail;

    while (head != tail)
    {
        entryCnt++;
        head = head->next;
    }
    entryCnt++; /* add the tail */
escape:
    return (entryCnt);
}




t_xqueue *XQUEUE__Dequeue(t_xqueue **q)
{
    t_xqueue *head = XQUEUE__NULL;
    t_xqueue *tail = XQUEUE__NULL;

    /* Check that the pointer arguments are valid */
    if (!q || !(*q)) goto escape;

    head = *q;

    /* Check if it is the only node remaining in the queue */
    if ( XQUEUE__GetLevel(q) == 1)
    {
       (*q) = XQUEUE__NULL;
       goto removeLinks;
    }

    tail = head->tail;   /* Retrieve tail addr from queue head */

    *q = head->next;     /* update queue head with next item   */
    /**
     * Check that the new head is not null and update its prev link
     * and queue tail addr */
    if ( (*q) != XQUEUE__NULL )
    {
        (*q)->prev = head->prev;
        (*q)->tail = tail;
    }
removeLinks:
    head->next = XQUEUE__NULL; /* remove link */
    head->prev = XQUEUE__NULL; /* remove link */
    head->tail = XQUEUE__NULL; /* remove link */

escape:
    return (head);
}




tenXqueueRetCode XQUEUE__StaticEnqueue(t_xqueue **q, t_xqueue *item)
{
    tenXqueueType t;
    tenXqueueRetCode retCode = XQUEUE__enRetFailure;
    unsigned int  level;
    t_xqueue *head;

    /* Check that the pointer arguments are valid */
    if (!q || !item) goto escape;

    head = *q;
    /* Add immediately if the queue is empty */
    if (!head)
    {
        *q = item;
        item->next = XQUEUE__NULL;
        item->prev = XQUEUE__NULL;
        item->tail = item;
        retCode = XQUEUE__enRetSuccess;
        goto escape;
    }

    /* Check that there is enough room to add item */
    level = XQUEUE__GetLevel(q);
    if (level == head->capacity)
    {
        retCode = XQUEUE__enRetErrFull;
        goto escape;
    }

    /* Check that queue does not already has item within */
    if (XQUEUE__IsDuplicate(q, item)) 
    {
        retCode = XQUEUE__enRetErrDupEnqueueAttempt;
        goto escape;
    }


    t = head->type;
    /* Add item to queue based on queue type */
    if (t == XQUEUE__enLifo ) /* Add item to the queue head (front)*/
    {   
        item->prev = XQUEUE__NULL;
        *q = item;           /* Place item on top */
        item->next = head;   /* connect the rest to new item */
        item->tail = head->tail;
        head->prev = item;
    }
    else /* queue type must be of PRIORITY or FIFO */
    {    /* Add item to the within queue head, middle, tail */
        XQUEUE__EnqueueOnPriority(q, item);
    }
    retCode = XQUEUE__enRetSuccess;
escape:
    return (retCode);
}



char XQUEUE__IsDuplicate(t_xqueue **q, t_xqueue *item)
{
    t_xqueue *head = *q;
    char      retCode= 0;
    
    /* check item node with tail */
    if (item == (*q)->tail)
    {
        retCode =  1;
        goto escape;
    }

    /* Check item with head until the tail */
    while (head != (*q)->tail)
    {
        if ( head == item )
        {
            retCode = 1;
            break ;
        }
        head = head->next;
    }
escape:
    return (retCode);
}



void    XQUEUE__EnqueueOnPriority(t_xqueue **q, t_xqueue *item)
{

    t_xqueue *head = (*q);  /* retrieve tail node addr */
    t_xqueue *tail = head->tail;  /* retrieve tail node addr */
    t_xqueue *currNode = head;

    /* Check if the correct spot is at the head */
    if (item->priority > currNode->priority)
    {
        item->next = currNode;
        item->prev = currNode->prev;
        item->tail = tail;
        (*q) = item;
        goto escape;
    }
    /* Find the right spot within the list of nodes */
    while ( (currNode != XQUEUE__NULL) && (currNode != tail) )
    {
        if ( item->priority > currNode->priority )
        {
            item->next = currNode;
            item->prev = currNode->prev;
            goto escape;
        }
        currNode = currNode->next;
    }
    /* last item in the list - New item takes last position */
    item->next = currNode->next;
    currNode->next = item;
    item->prev = currNode;
    (*q)->tail = item; /* Update the tail addr on queue head */

escape:
    return ;
}




void             XQUEUE__StaticDeInit(t_xqueue **q)
{
    t_xqueue *head;
    t_xqueue *next;

   if ( !q ) goto escape;
   head = *q;

   if ( !head ) goto escape ;

   while (head != XQUEUE__NULL)
   {
        next = head->next;
        head->next = XQUEUE__NULL;
        head->prev = XQUEUE__NULL;
        head->tail = XQUEUE__NULL;
        head->content = XQUEUE__VOID;
        head->priority = 0;
        head = next;
   }
   *q = head;
escape:
   return ;
}




tenXqueueRetCode XQUEUE__StaticEnqueueNewItem(t_xqueue **rscQueue, t_xqueue **opsQueue,
                                                                    void *item,
                                                                    unsigned int priority)
{
    tenXqueueRetCode retCode = XQUEUE__enRetFailure;
    t_xqueue         *head = XQUEUE__NULL;

    /* Check that the args pointer are valid */
    if (!rscQueue || !opsQueue || !item ) goto escape;
    
    head = XQUEUE__Dequeue(rscQueue);

    /* Check that the dequeue operation is successful */
    if (!head) goto escape;

    head->content = item;
    head->priority = priority;
    retCode = XQUEUE__StaticEnqueue(opsQueue, head);

escape: 
    return (retCode);
}




void  *XQUEUE__GetItem(t_xqueue *q)
{
    void *item = XQUEUE__VOID;

    if (!q) goto escape;
    item = q->content;

escape:
    return (item);
}





t_xqueue  *XQUEUE__FindNode(t_xqueue **q, void *item)
{
    t_xqueue *node = XQUEUE__NULL;


    /* Check that the argument pointers are valid */
    if (!q || !item) goto escape;

    if (XQUEUE__GetLevel(q) == 0) goto escape;
    node = *q;

    /* Check the head contains a valid node */
    if (!node) goto escape;

    /* check if item is tail */
    if ( node->tail->content == item )
    {
        node = node->tail;
        goto escape;
    }

    /* check from head until tail */
    while(node != (*q)->tail)
    {
        if ( node->content == item ) goto escape;
        node = node->next;
    }
    node = XQUEUE__NULL;
escape:
    return (node);
}



/**
 * Do not use this function for LIFO queue type - it will return null 
 * Instead use the XQUEUE__Dequeue which is more suitable for LIFO Queue
 * */
t_xqueue  *XQUEUE__DequeueNode(t_xqueue **q, void *item)
{
    t_xqueue *node = XQUEUE__NULL;
    t_xqueue *tail = XQUEUE__NULL;

    if (!q || !item) goto escape;

    /* check if queue is empty */
    if(!(*q)) goto escape;

    if ( (*q)->type == XQUEUE__enLifo ) goto escape;

    /* Retrieve tail addr from queue head */
    tail = (*q)->tail;

    /* Verify that node exist in queue */    
    node = XQUEUE__FindNode(q, item);    
    if (!node) goto escape;

    /* Check if it is the only node remaining in the queue */
    if ( XQUEUE__GetLevel(q) == 1)
    {
       (*q) = XQUEUE__NULL;
       goto removeLinks;
    }

    /* head */
    if ( (node == (*q)) )
    {
        node = XQUEUE__Dequeue(q);
    }
    else if ( node == tail ) /* last node */
    {
        (*q)->tail = node->prev;       /* update the tail addr of queue to prev */
        node->prev->next = node->next; /* rebuild link between prev and next    */
    }
    else /* node must be in the middle node - simply rebuild link */
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
removeLinks:
    /* remove links */
    node->prev = XQUEUE__NULL;
    node->next = XQUEUE__NULL;
escape:
   return (node);
}
