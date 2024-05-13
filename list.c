#include "list.h"


t_list	*LIST__CreateNewDyn(tenListType t, void *item)
{
	t_list	*node;

	node = LIST__NULL;
	node = (t_list *)malloc(sizeof(t_list) * 1);
	if ( !node ) goto escape;

    node->type = t;
	if (!item) 
    {
        node->content = LIST__NULL;
    }
    else
    {
        node->content = item;
        node->next = LIST__NULL;
        node->prev = LIST__NULL;
    }
escape:
	return (node);
}




t_list	*LIST__StaticCreateNew(tenListType t, t_list *buffer, unsigned int bufSz, void *item)
{
    t_list       *node;
    unsigned int idxNode = 0;

    node = LIST__NULL;

	if ( !buffer ) goto escape;
    node = buffer;
    node->type = t;

   

   /* Update the node index to point to the last node */
    idxNode = ((bufSz - 1) * LIST__NODE_SZ);
    /**
     * Initialise the last node in the list 
     * - Set content to null
     * - set the next attribute to null
     * - set the prev attribute to the preceeding node */
    (buffer + idxNode) ->content = LIST__VOID;
    (buffer + idxNode)->next = LIST__NULL;
    (buffer + idxNode)->prev = (buffer + idxNode - LIST__NODE_SZ);
    



    /* Update the index to point to the second to last node */
    idxNode = (bufSz - 2) * LIST__NODE_SZ ;

    /**
     * Initialise the nodes between last and the first -> exclude the first node
     * - Set content to null
     * - set the next attribute to the subsequent node 
     * - set the prev attribute to the preceeding node */
    while (idxNode > 0)
    {
        (buffer + idxNode)->content = LIST__VOID;
        (buffer + idxNode)->next = (buffer + idxNode + LIST__NODE_SZ);
        (buffer + idxNode)->next = (buffer + idxNode - LIST__NODE_SZ);
        idxNode -= LIST__NODE_SZ; /* Decrement the node index towards the top node */
    }

    /**
     * Initialise the nodes first node.
     * - Set content to null
     * - set the next attribute to the subsequent node 
     * - set the prev attribute to null */
    if (item)
    {
        node->entryCnt = bufSz - 1;
        node->content = item;
    }
    else
    {
        node->entryCnt = 0;  /* No item in the list - empty */
        node->content = LIST__VOID;
    }
    node->next = (node + LIST__NODE_SZ); /* Point to the next node after the first */
    node->prev = LIST__NULL;

escape:
	return (node);
}


unsigned int LIST__IsEmpty(t_list **lst)
{
    unsigned int entryCnt = 0;

    /* Check that list head address is valid 
     * Check that it contains a valid node */
    if (!lst || !(*lst)) goto escape;

    /* Retrieve information from the first node in the list */
    entryCnt = (*lst)->entryCnt;
escape:
    return (entryCnt);
}


void	LIST__DynAddBack(t_list **lst, t_list *new)
{
	t_list	*idx;

	if ( (!lst) || (!new) ) goto escape;

	idx = *lst;

    /* Check if the list contains no node */
	if (idx == LIST__NULL)
	{
		*lst = new;
		goto escape ;
	}	

    /* List contains some nodes */
	while (idx->next != LIST__NULL)
	{
		idx = idx->next;
	}
	idx->next = new;
escape:
    return ;
}



void    LIST__AddOnPriority(t_list **lst, t_list *new)
{
    t_list *idx = *lst;
    unsigned int entryCnt = idx->entryCnt;

    /* May be the head is the right position */
    if (new->priority > idx->priority)
    {
        *lst = new;
        new->prev = LIST__NULL;
        new->next = idx;
        goto escape;
    }
    /* Find the right spot within the list of nodes */
    while ( idx->next != LIST__NULL )
    {
        idx = idx->next;
        if ( idx->priority < new->priority )
        {
            new->prev = idx->prev;
            new->next = idx;
            idx->prev = new;
            goto escape;
        }
    }
    /* last item in the list - New item takes last position */
    idx->next = new;
    idx->next = LIST__NULL;

escape:
    (*lst)->entryCnt = (entryCnt + 1);
    return ;
}

void    LIST__StaticAddBack(t_list **lst, void *new)
{
	t_list	*idxNode;
    unsigned int entryCnt;
    unsigned int lstSize;

	if ( (!lst) || (!new) ) goto escape;


    
escape:    
    return ;
}

void    LIST__StaticAddFront(t_list **lst, t_list new)
{

}



void	LIST__StaticAdd(t_list **lst, t_list *new)
{
	t_list	*idxNode;
    tenListType t;
    unsigned int entryCnt;
    unsigned int lstSize;

	if ( (!lst) || (!new) ) goto escape;

    /* Retrieve the list properties from the first node */
	idxNode = *lst;
    t = idxNode->type;
    entryCnt = LIST__IsEmpty(lst);
    lstSize = idxNode->size;

    /* List contains some nodes */
    if (entryCnt < lstSize)
    
    {{
        switch (t)
        {
            case LIST__enPriority:
            {
                LIST__AddOnPriority(lst, new);
                break ;
    
            }
            case LIST__enLifo:
            {
    
                break ;
            }
            default: /* fifo */
            {
    
                break ;
            }
        }
    }

escape:
    return ;
}


void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = (*lst);
	(*lst) = new;
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






int	ft_lstsize(t_list *lst)
{
	int		cnt;
	t_list	*adr_node;

	cnt = 0;
	adr_node = lst;
	while (adr_node != 0)
	{
		adr_node = adr_node->next;
		cnt++;
	}
	return (cnt);
}