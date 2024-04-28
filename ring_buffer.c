
#include "ring_buffer.h"


void RING_BUFFER__Init(t_ringBuffer * const me, unsigned char *b, unsigned char s)
{
    if ( me != (void *)0 )
    {
        me->buf = b;
        me->size = s;
        me->room = s;
        me->idxPut = 0;
        me->idxGet = 0;
        me->dataCnt = 0;
    }
    return ;
}



unsigned char RING_BUFFER__Get(t_ringBuffer * const me)
{
    unsigned char c = 0;

    if ( me != (void *)0 )
    {
        if (me->dataCnt)
        {
            c = me->buf[me->idxGet];
            me->dataCnt--;
            me->room++;
            me->idxGet = (me->idxGet + 1) % (me->size);
        }
    }
    return (c);
}


unsigned char RING_BUFFER__Peek(t_ringBuffer * const me)
{
    unsigned char c = 0;

    if ( me != (void *)0 )
    {
        c = *(me->buf + me->idxGet);
    }
    return (c);
}




void RING_BUFFER__Put(t_ringBuffer * const me, unsigned char c)
{
    if ( me != (void *)0 )
    {
        if (me->room)
        {
            me->buf[me->idxPut] = c;
            me->room--;
            me->dataCnt++;
            me->idxPut = (me->idxPut + 1) % (me->size);
        }
    }
    return ;
}

char RING_BUFFER__IsEmpty(t_ringBuffer * const me)
{
    char ret = 0;

    if ( me != (void *)0 )
    {
        if (me->room == me->size)
        {
            ret = 1;
        }
    }
    return (ret);
}



char RING_BUFFER__IsFull(t_ringBuffer * const me)
{
    char ret = 0;

    if ( me != (void *)0 )
    {
        if (me->dataCnt == me->size)
        {
            ret = 1;
        }
    }
    return (ret);
}