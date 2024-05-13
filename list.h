#ifndef LIST_H
#define LIST_H

/**
 * The queue or list uses a priority scheme with highest 
 * number having the highest priority 
 * 
 */
typedef struct s_list
{
	void			*content;
    unsigned char   priority;
    tenListType     type;
	struct s_list	*next;
	struct s_list	*prev;
    unsigned int    entryCnt;
    unsigned int    size;
}                   t_list;

typedef enum
{
    LIST__enPriority=0,
    LIST__enFifo,
    LIST__enLifo,
}   tenListType;


#define LIST__NULL (t_list *)0
#define LIST__VOID (void *)0
#define LIST__NODE_SZ (sizeof(t_list))


unsigned int LIST__IsEmpty(t_list **lst);
t_list	*LIST__DynCreateNewDyn(tenListType t, void *item);
void	 LIST__DynAddBack(t_list **lst, t_list *new);
t_list	*LIST__StaticCreateNew(tenListType t, t_list *buffer, unsigned int bufSz, void *item);
void	ft_lstadd_front(t_list **lst, t_list *new)
void	ft_lstclear(t_list **lst, void (*del)(void*))
void	ft_lstdelone(t_list *lst, void (*del)(void*))
void	ft_lstiter(t_list *lst, void (*f)(void *))
t_list	*ft_lstlast(t_list *lst)
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
int	ft_lstsize(t_list *lst)

void    LIST__AddToBackOnPriority(void);

#endif /* LIST_H */

/*
 0x00 - x

 */