#include "xqueue.h"

#if 0
t_list	*XQUEUE__CreateNewDyn(tenListType t, void *item)
{
	t_list	*node;

	node = XQUEUE__NULL;
	node = (t_list *)malloc(sizeof(t_list) * 1);
	if ( !node ) goto escape;

    node->type = t;
	if (!item) 
    {
        node->content = XQUEUE__NULL;
    }
    else
    {
        node->content = item;
        node->next = XQUEUE__NULL;
        node->prev = XQUEUE__NULL;
    }
escape:
	return (node);
}
#endif



t_xqueue	*XQUEUE__StaticCreateNew(tenXqueueType t, t_xqueue *queue, 
                                                      void *buf,
                                                      unsigned int itemCnt, 
                                                      unsigned int itemSize)
{
    t_xqueue     *head = XQUEUE__NULL;
    unsigned int idx=0;

    /* Check that the pointer arguments are valid */
    if (!queue || !buf ) goto escape;

    for (idx=0; idx < itemCnt; idx++)
    {
        head = queue + (idx * XQUEUE__NODE_SZ);
        head->capacity = itemCnt;
        head->priority = 0;
        head->type = t;
        head->content = buf + (idx * itemSize);
        head->next = (head + XQUEUE__NODE_SZ);
        head->prev = (head - XQUEUE__NODE_SZ);
    }
    head->next = XQUEUE__NULL;

    head = queue;
    head->prev = XQUEUE__NULL;
escape:
    return (head);
}




unsigned int XQUEUE__GetLevel(t_xqueue **q)
{
    unsigned int entryCnt = 0;
    t_xqueue     *head = XQUEUE__NULL;

    /* Check that the pointer arguments are valid */
    if (!q) goto escape;

    head = q;
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
    if (!q || !(*q)) goto escape;

    head = *q;

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
    if (t = XQUEUE__enLifo ) /* Add item to the queue head (front)*/
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
        if ( head->content == item->content )
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