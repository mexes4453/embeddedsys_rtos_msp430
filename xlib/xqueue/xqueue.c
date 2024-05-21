#include "xqueue.h"



t_xqueue *XQUEUE__StaticInit(tenXqueueType t, t_xqueue *queue,
                                              unsigned int queueSize,
                                              void *item)
{
    t_xqueue     *head = XQUEUE__NULL;
    unsigned int idx=0;

    /* Check that the pointer arguments are valid */
    if (!queue) goto escape;
    
    /* Check that nbr of item is greater than 0 */
    if (!queueSize) goto escape;

    /* Initialise all the nodes */
    for (idx=0; idx < queueSize; idx++)
    {
        head = queue + (idx);            /* next item in array of queue item */
        head->capacity = queueSize;
        head->priority = 0;
        head->type = t;
        head->content = XQUEUE__VOID;
        head->next = (head + 1);
        head->prev = (head - 1);
    }
    /**
     * At this point, the last item in the queue array has its next pointing
     * to an location outside the queue array size range.
     * The last item should point to NULL*/
    head->next = XQUEUE__NULL;

    /**
     * The head points back to the top the queue and its prev is reset to
     * NULL.*/
    head = queue;
    head->prev = XQUEUE__NULL;

    /* Load data (item) into the queue head if available */
    head->content = (item ? item : XQUEUE__VOID);
escape:
    return (head);
}




unsigned int XQUEUE__GetLevel(t_xqueue **q)
{
    unsigned int entryCnt = 0;
    t_xqueue     *head = XQUEUE__NULL;

    /* Check that the pointer arguments are valid */
    if (!q) goto escape;

    head = *q;
    while (head != XQUEUE__NULL)
    {
        entryCnt++;
        head = head->next;
    }
escape:
    return (entryCnt);
}




t_xqueue *XQUEUE__Dequeue(t_xqueue **q)
{
    t_xqueue *head = XQUEUE__NULL;

    /* Check that the pointer arguments are valid */
    if (!q || !(*q)) goto escape;

    head = *q;
    *q = head->next; /* update queue head with next item */
    head->next = XQUEUE__NULL; /* remove link */
    head->prev = XQUEUE__NULL; /* remove link */

escape:
    return (head);
}




tenXqueueRetCode XQUEUE__StaticEnqueue(t_xqueue **q, t_xqueue *item)
{
    tenXqueueType t;
    tenXqueueRetCode retCode = XQUEUE__enRetErrFull;
    unsigned int  level;
    t_xqueue *head;

    /* Check that the pointer arguments are valid */
    if (!q) goto escape;

    head = *q;
    /* Add immediately if the queue is empty */
    if (!head)
    {
        *q = item;
        item->next = XQUEUE__NULL;
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
    

    while (head != XQUEUE__NULL)
    {
        if ( head == item )
        {
            retCode = 1;
            break ;
        }
        head = head->next;
    }
    return (retCode);
}



void    XQUEUE__EnqueueOnPriority(t_xqueue **q, t_xqueue *item)
{
    t_xqueue *head = *q;

    item->next = XQUEUE__NULL;
    item->prev = XQUEUE__NULL;

    /* Check if the correct spot is at the head */
    if (item->priority > head->priority)
    {
        item->next = head;
        item->prev = head->prev;
        *q = item;
        goto escape;
    }
    /* Find the right spot within the list of nodes */
    while ( head != XQUEUE__NULL )
    {
        if ( item->priority > head->priority )
        {
            item->next = head;
            item->prev = head->prev;
            goto escape;
        }
        /* make a sneak peek to see if head is last item in queue */
        if (head->next == XQUEUE__NULL) break ;
        head = head->next;
    }
    /* last item in the list - New item takes last position */
    head->next = item;
    item->prev = head;

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
        head->content = XQUEUE__VOID;
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

#if 0

void	XQUEUE__DynAddBack(t_list **lst, t_list *new)
{
	t_list	*idx;

	if ( (!lst) || (!new) ) goto escape;

	idx = *lst;

    /* Check if the list contains no node */
	if (idx == XQUEUE__NULL)
	{
		*lst = new;
		goto escape ;
	}	

    /* List contains some nodes */
	while (idx->next != XQUEUE__NULL)
	{
		idx = idx->next;
	}
	idx->next = new;
escape:
    return ;
}




void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*curr;
	t_list	*prev;

	if (*lst != NULL)
	{
		curr = *lst;
		while (curr != 0)
		{
			prev = curr;
			curr = curr->next;
			del((void *)prev->content);
			free(prev);
		}
	}
	*lst = NULL;
}



void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	del(lst->content);
	free(lst);
}


void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*node;

	node = lst;
	if (node != 0)
	{
		while (node != 0)
		{
			f(node->content);
			node = node->next;
		}
	}
}



t_list	*ft_lstlast(t_list *lst)
{
	t_list	*adr_node;

	adr_node = lst;
	if (adr_node == 0)
		return (0);
	while (adr_node->next != 0)
	{
		adr_node = adr_node->next;
	}
	return (adr_node);
}



t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*curr;
	t_list	*new_node;
	t_list	*prev;

	if (lst != 0)
	{
		new_lst = (void *)0;
		curr = lst;
		while (curr != NULL)
		{
			new_node = ft_lstnew(f(curr->content));
			if (new_lst == NULL)
				new_lst = new_node;
			else
				ft_lstadd_back(&new_lst, new_node);
			prev = curr;
			curr = curr->next;
			if (prev == NULL)
				del(prev);
		}
		lst = NULL;
		return (new_lst);
	}
	return (0);
}

#endif