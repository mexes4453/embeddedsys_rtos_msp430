#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef struct s_ringBuffer
{
    /* data */
    unsigned char *buf;
    unsigned char room;
    unsigned char idxPut;
    unsigned char idxGet;
    unsigned char dataCnt;
    unsigned char size;
}   t_ringBuffer;


void          RING_BUFFER__Init(t_ringBuffer * const me, unsigned char *b, unsigned char s);
unsigned char RING_BUFFER__Get(t_ringBuffer * const me);
unsigned char RING_BUFFER__Peek(t_ringBuffer * const me);
void          RING_BUFFER__Put(t_ringBuffer * const me, unsigned char c);
char          RING_BUFFER__IsEmpty(t_ringBuffer * const me);
char          RING_BUFFER__IsFull(t_ringBuffer * const me);




#if 0 /* Test code */
#include "ring_buffer.h"
#include <stdio.h>


int main(void)
{
    t_ringBuffer m;
    unsigned char buf[5];
    RING_BUFFER__Init(&m, buf, 5);
    RING_BUFFER__Put(&m, 'a');
    RING_BUFFER__Put(&m, 'b');
    RING_BUFFER__Put(&m, 'c');
    RING_BUFFER__Put(&m, 'd');
    RING_BUFFER__Put(&m, 'e');
    RING_BUFFER__Put(&m, 'f');
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    RING_BUFFER__Put(&m, 'f');
    RING_BUFFER__Put(&m, 'g');
    RING_BUFFER__Put(&m, 'h');
    RING_BUFFER__Put(&m, 'i');
    RING_BUFFER__Put(&m, 'j');
    RING_BUFFER__Put(&m, 'k');
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));
    RING_BUFFER__Put(&m, 'k');
    printf("Data from queue: %c\n", RING_BUFFER__Get(&m));

    return (0);
}
#endif 

#endif  /*  RING_BUFFER_H  */