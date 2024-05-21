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
        XQUEUE__StaticDeInit(&dataQueueFree);

        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo,            /* 111 */
                                           &(qPool[0]),
                                           XQUEUE__SIZE,
                                           (void *)&data[0]);
        REQUIRE(dataQueueFree != XQUEUE__NULL);
        XQUEUE__StaticDeInit(&dataQueueFree);
    }

    SECTION("002 - testing Queue creation - check entries - input 110 ")
    {
        //uint16_t data[5] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
        t_xqueue qPool[5];
        t_xqueue *dataQueueFree = XQUEUE__NULL;
        //t_xqueue *dataQueueBusy = XQUEUE__NULL;
        dataQueueFree = XQUEUE__StaticInit(XQUEUE__enPriorityFifo, &(qPool[0]), 5, XQUEUE__VOID);

        REQUIRE(dataQueueFree->capacity == 5);
        REQUIRE(dataQueueFree->content == XQUEUE__VOID);
        REQUIRE(dataQueueFree->prev == XQUEUE__NULL);
        REQUIRE(dataQueueFree->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->next->next->next->next != XQUEUE__NULL);
        REQUIRE_FALSE(dataQueueFree->next->next->next->next->next != XQUEUE__NULL);

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

        TESTER_SECTION_S(XQUEUE: Check the getItem method);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy)) == 0xEEEE);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy->next)) == 0xDDDD);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy->next->next)) == 0xCCCC);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy->next->next->next)) == 0xBBBB);
        REQUIRE(*((uint16_t *)XQUEUE__GetItem(dataQueueBusy->next->next->next->next)) == 0xAAAA);
        TESTER_SECTION_E();

        TESTER_SECTION_S(XQUEUE: Attempt to add a xqueue instance twice in a queue);
        TESTER_SECTION_S(XQUEUE: (arg input -> 1110) - Attempt to add duplicate);
        REQUIRE( XQUEUE__StaticEnqueue(&dataQueueFree, dataQueueFree) == XQUEUE__enRetErrDupEnqueueAttempt );
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 1 );
        TESTER_SECTION_E();
    }
}
