#include "../inc/catch.hpp" 
# include "../inc/test_utils.hpp"
# define XQUEUE__SIZE 5
extern "C" {
    #include "../../xqueue/xqueue.h"
    
}

TEST_CASE("XQUEUE: FIFO PRIORITY", "[xqueue][fifo]")
{

    SECTION("001 - testing Queue creation - check inputs")
    {
        uint16_t data[XQUEUE__SIZE] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
        t_xqueue qPool[XQUEUE__SIZE];
        t_xqueue *dataQueueFree = XQUEUE__NULL;

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 000 */
                                           XQUEUE__NULL,
                                           0,
                                           XQUEUE__VOID);
        REQUIRE(dataQueueFree == XQUEUE__NULL);
        XQUEUE__StaticDeInit(&dataQueueFree);

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 001 */
                                           XQUEUE__NULL,
                                           0,
                                           (void *)&data[0]);
        REQUIRE(dataQueueFree == XQUEUE__NULL);
        XQUEUE__StaticDeInit(&dataQueueFree);

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 010 */
                                           XQUEUE__NULL,
                                           XQUEUE__SIZE,
                                           XQUEUE__VOID);
        REQUIRE(dataQueueFree == XQUEUE__NULL);
        XQUEUE__StaticDeInit(&dataQueueFree);

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 011 */
                                           XQUEUE__NULL,
                                           XQUEUE__SIZE,
                                           (void *)&data[0]);
        REQUIRE(dataQueueFree == XQUEUE__NULL);
        XQUEUE__StaticDeInit(&dataQueueFree);

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 100 */
                                           &(qPool[0]),
                                           0,
                                           XQUEUE__VOID);
        REQUIRE(dataQueueFree == XQUEUE__NULL);
        XQUEUE__StaticDeInit(&dataQueueFree);

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 101 */
                                           &(qPool[0]),
                                           0,
                                           (void *)&data[0]);
        REQUIRE(dataQueueFree == XQUEUE__NULL);
        XQUEUE__StaticDeInit(&dataQueueFree);

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 110 */
                                           &(qPool[0]),
                                           XQUEUE__SIZE,
                                           XQUEUE__VOID);
        REQUIRE(dataQueueFree != XQUEUE__NULL);
        REQUIRE(dataQueueFree->tail == &(qPool[XQUEUE__SIZE-1]));
        XQUEUE__StaticDeInit(&dataQueueFree);

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 111 */
                                           &(qPool[0]),
                                           XQUEUE__SIZE,
                                           (void *)&data[0]);
        REQUIRE(dataQueueFree != XQUEUE__NULL);
        REQUIRE(dataQueueFree->tail == &(qPool[XQUEUE__SIZE-1]));
        XQUEUE__StaticDeInit(&dataQueueFree);
    }

    SECTION("002 - testing Queue creation - check entries - input 110 ")
    {
        //uint16_t data[5] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
        t_xqueue qPool[5];
        t_xqueue *dataQueueFree = XQUEUE__NULL;
        //t_xqueue *dataQueueBusy = XQUEUE__NULL;
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), 5, XQUEUE__VOID);

        REQUIRE(dataQueueFree->tail == &(qPool[XQUEUE__SIZE-1]));
        REQUIRE(dataQueueFree->capacity == 5);
        REQUIRE(dataQueueFree->content == XQUEUE__VOID);
        REQUIRE(dataQueueFree->prev == XQUEUE__NULL);
        REQUIRE(dataQueueFree->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next->next->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next->next->next->next == XQUEUE__NULL);

        /* GET LEVEL */
        TESTER_SECTION_S(XQUEUE: (arg input -> 110 - GetLevel));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 5 );
        TESTER_SECTION_E();
    }

    SECTION("003 - testing Queue creation - check entries - input 111 ")
    {
        uint16_t data[5] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
        t_xqueue qPool[5];
        t_xqueue *dataQueueFree = XQUEUE__NULL;
        //t_xqueue *dataQueueBusy = XQUEUE__NULL;
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), 5, &(data[0]));

        REQUIRE(dataQueueFree->tail == &(qPool[XQUEUE__SIZE-1]));
        REQUIRE(dataQueueFree->capacity == 5);
        REQUIRE(dataQueueFree->content == &(data[0]));
        REQUIRE(dataQueueFree->prev == XQUEUE__NULL);
        REQUIRE(dataQueueFree->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next->next->next != XQUEUE__NULL);
        REQUIRE_FALSE(dataQueueFree->next->next->next->next->next != XQUEUE__NULL);

        /* GET LEVEL */
        TESTER_SECTION_S(XQUEUE: (arg input -> 111 - GetLevel));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 5 );
        TESTER_SECTION_E();
    }



    SECTION("004 - testing Queue creation - check enqueue and dequeue - input 110 ")
    {
        //uint16_t data[5] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
        t_xqueue qPool[5];
        t_xqueue *dataQueueFree = XQUEUE__NULL;
        t_xqueue *dataQueueBusy = XQUEUE__NULL;

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), 5, XQUEUE__VOID);
        REQUIRE(dataQueueFree->tail == &(qPool[XQUEUE__SIZE-1]));
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE(dataQueueBusy->tail == &(qPool[0]));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 4 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 1 );
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE(dataQueueBusy->tail == &(qPool[1]));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 3 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 2 );
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE(dataQueueBusy->tail == &(qPool[2]));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 2 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 3 );
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE(dataQueueBusy->tail == &(qPool[3]));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 1 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 4 );
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE(dataQueueBusy->tail == &(qPool[4]));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 0 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 5 );
    }


    SECTION("005 - testing Queue creation - check enqueue and dequeue - input 111 ")
    {
        uint16_t data[5] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
        t_xqueue qPool[5];
        t_xqueue *dataQueueFree = XQUEUE__NULL;
        t_xqueue *dataQueueBusy = XQUEUE__NULL;

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), 5,
                                           (void *)&(data[0]));
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 4 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 1 );
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 3 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 2 );
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 2 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 3 );
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 1 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 4 );
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 0 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 5 );
    }

    SECTION("006 - testing Queue creation - check data validity - input 110 ")
    {
        uint16_t data[5] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
        t_xqueue qPool[5];
        t_xqueue *dataQueueFree = XQUEUE__NULL;
        t_xqueue *dataQueueBusy = XQUEUE__NULL;

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), 5, XQUEUE__VOID);

        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[0]), 0);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[1]), 0);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[2]), 0);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[3]), 0);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[4]), 0);
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 0 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 5 );
        TESTER_SECTION_S(XQUEUE: Check the next queue item points to the right data index);
        REQUIRE(((uint16_t *)dataQueueBusy->content) == &(data[0]));
        REQUIRE(*((uint16_t *)dataQueueBusy->content) == 0xAAAA);
        REQUIRE(((uint16_t *)dataQueueBusy->next->content) == &(data[1]));
        REQUIRE(*((uint16_t *)dataQueueBusy->next->content) == 0xBBBB);
        REQUIRE(((uint16_t *)dataQueueBusy->next->next->content) == &(data[2]));
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->content) == 0xCCCC);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->next->content) == 0xDDDD);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->next->next->content) == 0xEEEE);
        REQUIRE(dataQueueBusy->next->next->next->next->next == XQUEUE__NULL);
        TESTER_SECTION_E();

        TESTER_SECTION_S(XQUEUE: Check the priority feature );
        XQUEUE__StaticDeInit(&dataQueueBusy);
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), XQUEUE__SIZE,
                                           XQUEUE__VOID);
        
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[0]), 5);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[1]), 3);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[2]), 8);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[3]), 3);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[4]), 9);
        REQUIRE(*((uint16_t *)dataQueueBusy->content) == 0xEEEE);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->content) == 0xCCCC);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->content) == 0xAAAA);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->next->content) == 0xBBBB);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->next->next->content) == 0xDDDD);
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 0 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 5 );
        TESTER_SECTION_E();
        
        TESTER_SECTION_S(XQUEUE: Check the stack feature );
        XQUEUE__StaticDeInit(&dataQueueBusy);
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enLifo, &(qPool[0]), XQUEUE__SIZE,
                                           XQUEUE__VOID);
        
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[0]), 5);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[1]), 3);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[2]), 8);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[3]), 3);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[4]), 9);
        REQUIRE(*((uint16_t *)dataQueueBusy->content) == 0xEEEE);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->content) == 0xDDDD);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->content) == 0xCCCC);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->next->content) == 0xBBBB);
        REQUIRE(*((uint16_t *)dataQueueBusy->next->next->next->next->content) == 0xAAAA);
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 0 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 5 );
        TESTER_SECTION_E();

        TESTER_SECTION_S(XQUEUE: Check the that the queue node id is valid );
        XQUEUE__StaticDeInit(&dataQueueBusy);
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enLifo, &(qPool[0]), XQUEUE__SIZE,
                                           XQUEUE__VOID);
        
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[0]), 5);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[1]), 3);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[2]), 8);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[3]), 3);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[4]), 9);
        REQUIRE(dataQueueBusy->qid == 4);
        REQUIRE(dataQueueBusy->next->qid == 3);
        REQUIRE(dataQueueBusy->next->next->qid == 2);
        REQUIRE(dataQueueBusy->next->next->next->qid == 1);
        REQUIRE(dataQueueBusy->next->next->next->next->qid == 0);
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 0 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 5 );
        TESTER_SECTION_E();

        TESTER_SECTION_S(XQUEUE: Check the getItem method);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy)) == 0xEEEE);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy->next)) == 0xDDDD);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy->next->next)) == 0xCCCC);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy->next->next->next)) == 0xBBBB);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy->next->next->next->next)) == 0xAAAA);
        TESTER_SECTION_E();

        TESTER_SECTION_S(XQUEUE: Attempt to add a null xqueue instance on queue );
        REQUIRE( XQUEUE__StaticEnqueue(&dataQueueFree, dataQueueFree) == XQUEUE__enRetFailure );
        REQUIRE( dataQueueFree == XQUEUE__NULL );
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 0 );
        TESTER_SECTION_E();

        TESTER_SECTION_S(XQUEUE: Attempt to add a xqueue instance twice - duplicate);
        XQUEUE__StaticDeInit(&dataQueueBusy);
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enLifo, &(qPool[0]), XQUEUE__SIZE,
                                           XQUEUE__VOID);
        
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[0]), 2);
        REQUIRE( dataQueueBusy->tail == &(qPool[0]));
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 1 );
        REQUIRE( XQUEUE__StaticEnqueue(&dataQueueBusy, dataQueueBusy) == XQUEUE__enRetErrDupEnqueueAttempt );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 1 );
        TESTER_SECTION_E();


        TESTER_SECTION_S(XQUEUE: Test the findNode method);
        REQUIRE( XQUEUE__FindNode((t_xqueue **)0, XQUEUE__VOID ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__FindNode((t_xqueue **)0, (void *)&(data[0]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__FindNode(&dataQueueBusy, XQUEUE__VOID ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__FindNode(&dataQueueBusy, (void *)&(data[0]) ) == &(qPool[0]));
        REQUIRE( XQUEUE__FindNode(&dataQueueBusy, (void *)&(data[1]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 4 );
        REQUIRE( XQUEUE__FindNode(&dataQueueFree, (void *)&(data[1]) ) == XQUEUE__NULL);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[1]), 2);
        REQUIRE( dataQueueBusy->tail == &(qPool[0]));
        REQUIRE( XQUEUE__FindNode(&dataQueueBusy, (void *)&(data[1]) ) == &(qPool[1]));
        REQUIRE( XQUEUE__FindNode(&dataQueueFree, (void *)&(data[0]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 3 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 2);
        TESTER_SECTION_E();
#if 1
        TESTER_SECTION_S(XQUEUE: DequeueNode  );
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[2]), 2);
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 2 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 3);
        REQUIRE( dataQueueBusy == &(qPool[2]));
        REQUIRE( dataQueueBusy->next == &(qPool[1]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[0]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[1]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[2]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__Dequeue(&dataQueueBusy) == &(qPool[2]));
        REQUIRE( XQUEUE__Dequeue(&dataQueueBusy) == &(qPool[1]));
        REQUIRE( XQUEUE__Dequeue(&dataQueueBusy) == &(qPool[0]));
        REQUIRE( XQUEUE__Dequeue(&dataQueueBusy) == XQUEUE__NULL);
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 2 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 0);
        TESTER_SECTION_E();
#endif
        TESTER_SECTION_S(XQUEUE: DequeueNode - HEAD );
        XQUEUE__StaticDeInit(&dataQueueBusy);
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), XQUEUE__SIZE,
                                           XQUEUE__VOID);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[0]), 2);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[1]), 2);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[2]), 2);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[3]), 2);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[4]), 2);
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[0]) ) == &(qPool[0]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[0]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[4]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[1]) ) == &(qPool[1]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[1]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[4]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[2]) ) == &(qPool[2]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[2]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[4]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[3]) ) == &(qPool[3]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[3]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[4]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[4]) ) == &(qPool[4]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[4]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 0);
        REQUIRE( XQUEUE__Dequeue(&dataQueueBusy) == XQUEUE__NULL);
        TESTER_SECTION_E();
        TESTER_SECTION_S( XQUEUE: DequeueNode - TAIL );
        XQUEUE__StaticDeInit(&dataQueueBusy);
        XQUEUE__StaticDeInit(&dataQueueFree);
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), XQUEUE__SIZE,
                                           XQUEUE__VOID);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[0]), 2);
        COUT << dataQueueBusy << " idx 0 - data-> " << &(data[0]) << " : Pool-> " << &(qPool[0]) << ENDL;
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[1]), 2);
        COUT << dataQueueBusy->next << " idx 1 - data-> " << &(data[1]) << " : Pool-> " << &(qPool[1]) << ENDL;
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[2]), 2);
        COUT << dataQueueBusy->next->next << " idx 2 - data-> " << &(data[2]) << " : Pool-> " << &(qPool[2]) << ENDL;
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[3]), 2);
        COUT << dataQueueBusy->next->next->next << " idx 3 - data-> " << &(data[3]) << " : Pool-> " << &(qPool[3]) << ENDL;
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[4]), 2);
        COUT << dataQueueBusy->next->next->next->next << " idx 4 - data-> " << &(data[4]) << " : Pool-> " << &(qPool[4]) << ENDL;
        COUT << dataQueueBusy << ENDL;
        COUT << dataQueueBusy->next << ENDL;
        COUT << dataQueueBusy->next->next << ENDL;
        COUT << dataQueueBusy->next->next->next << ENDL;
        COUT << dataQueueBusy->next->next->next->next << ENDL;
        COUT << XQUEUE__FindNode(&dataQueueBusy, (void *)&(data[4])) << ENDL;
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[4]) ) == &(qPool[4]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[4]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[3]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[3]) ) == &(qPool[3]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[3]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[2]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[2]) ) == &(qPool[2]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[2]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[1]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[1]) ) == &(qPool[1]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[1]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[0]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[0]) ) == &(qPool[0]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[0]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 0);
        REQUIRE( XQUEUE__Dequeue(&dataQueueBusy) == XQUEUE__NULL);
        TESTER_SECTION_E();
        TESTER_SECTION_S(XQUEUE: DequeueNode - CIRCULAR - TAIL);
        XQUEUE__StaticDeInit(&dataQueueBusy);
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), XQUEUE__SIZE,
                                           XQUEUE__VOID);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[0]), 2);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[1]), 2);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[2]), 2);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[3]), 2);
        XQUEUE__StaticEnqueueNewItem(&dataQueueFree, &dataQueueBusy, &(data[4]), 2);
        /* Create the circular link */
        dataQueueBusy->prev = dataQueueBusy->tail;
        dataQueueBusy->tail->next = dataQueueBusy;
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[4]) ) == &(qPool[4]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[4]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[3]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[3]) ) == &(qPool[3]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[3]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[2]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[2]) ) == &(qPool[2]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[2]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[1]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[1]) ) == &(qPool[1]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[1]) ) == XQUEUE__NULL);
        REQUIRE( dataQueueBusy->tail == &(qPool[0]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[0]) ) == &(qPool[0]));
        REQUIRE( XQUEUE__DequeueNode(&dataQueueBusy, (void *)&(data[0]) ) == XQUEUE__NULL);
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 0);
        REQUIRE( XQUEUE__Dequeue(&dataQueueBusy) == XQUEUE__NULL);
        TESTER_SECTION_E();
    }
}
