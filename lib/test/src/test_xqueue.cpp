#include "../inc/catch.hpp" 
//# include "../inc/TestFrameWork.hpp"
# include <iostream>
# include <iomanip>

extern "C" {
    #include "../../xqueue/xqueue.h"
    
}

TEST_CASE("XQUEUE: StaticCreateNew", "[xqueue]")
{
        uint16_t data[5] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
        t_xqueue qPool[5];
        t_xqueue *dataQueueFree = XQUEUE__NULL;
        t_xqueue *dataQueueBusy = XQUEUE__NULL;
    SECTION("001 - testing Queue creation - check entries")
    {
        REQUIRE(data[0] == 0xAAAA);
        REQUIRE(&(qPool[0]) != nullptr);

        dataQueueFree = XQUEUE__StaticCreateNew(XQUEUE__enPriorityFifo, &(qPool[0]), 
                                                                    (void *)&(data[0]),
                                                                     5,
                                                                     sizeof(uint16_t));
        REQUIRE(dataQueueFree != XQUEUE__NULL);
        //REQUIRE(sizeof(dataQueueFree->priority) == sizeof(struct s_xqueue));
#if 1
        REQUIRE(dataQueueFree->capacity == 5);
        REQUIRE(dataQueueFree->content != (void *)0);
        REQUIRE(*((uint16_t *)dataQueueFree->content) == 0xAAAA);
        REQUIRE(dataQueueFree->next != XQUEUE__NULL);
        REQUIRE(dataQueueFree->prev == XQUEUE__NULL);
        REQUIRE(dataQueueFree->priority == 0);
        REQUIRE(dataQueueFree->type == XQUEUE__enPriorityFifo);
        std::cout << "================================================================\n";
        std::cout << "XQUEUE: Check the next queue item points to the right data index\n";
        std::cout << "----------------------------------------------------------------\n";
        REQUIRE(((uint16_t *)dataQueueFree->next->content) == &(data[1]));
        REQUIRE(*((uint16_t *)dataQueueFree->next->content) == 0xBBBB);
        std::cout << "================================================================\n";
        REQUIRE(((uint16_t *)dataQueueFree->next->next->content) == &(data[2]));
        REQUIRE(*((uint16_t *)dataQueueFree->next->next->content) == 0xCCCC);
        REQUIRE(*((uint16_t *)dataQueueFree->next->next->next->content) == 0xDDDD);
        REQUIRE(*((uint16_t *)dataQueueFree->next->next->next->next->content) == 0xEEEE);
        REQUIRE(dataQueueFree->next->next->next->next->next == XQUEUE__NULL);

        /* GET LEVEL */

        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 5 );
        dataQueueBusy = XQUEUE__Dequeue(&dataQueueFree);
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 1 );
        REQUIRE(dataQueueBusy != XQUEUE__NULL);
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
        XQUEUE__StaticEnqueue(&dataQueueBusy, XQUEUE__Dequeue(&dataQueueFree));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 0 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 5 );
        XQUEUE__StaticEnqueue(&dataQueueFree, XQUEUE__Dequeue(&dataQueueBusy));
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 1 );
        REQUIRE( XQUEUE__GetLevel(&dataQueueBusy) == 4 );
        /* attempt to add duplicate */
        REQUIRE( XQUEUE__StaticEnqueue(&dataQueueFree, dataQueueFree) == XQUEUE__enRetErrDupEnqueueAttempt );
        REQUIRE( XQUEUE__GetLevel(&dataQueueFree) == 1 );

#endif
    }
#if 0       
    SECTION("002 - determine type cast behaviour int to unsigned char")
    {
        static int     varInt;
//        static uint16_t varUint8;
        uint16_t        testVal;
        int unsigned   res;


        testVal = 0xF0;

        std::cout << "before: " << varInt << std::endl;
        varInt -= testVal; 
        std::cout << "after: " << varInt << std::endl;
        varInt = (int)testVal;
        std::cout << "typecast testVal to int : " << std::hex 
        << std::showbase << std::setfill('0') << varInt << std::endl;

        varInt = ((int)testVal & 0x7F);
        std::cout << "typecast testVal to int : " << std::hex 
        << std::showbase << std::setfill('0') << varInt << std::endl;

        varInt = -((int)testVal & 0x7F);
        std::cout << "typecast testVal to int : " << std::hex 
        << std::showbase << std::setfill('0') << varInt << std::endl;

        res = (unsigned int)varInt;
        std::cout << "typecast varInt to unsigned int : " << std::hex 
        << std::showbase << std::setfill('0') << res << std::endl;
        std::cout << "typecast varInt to unsigned int (dec): " << std::dec << res << std::endl;

        res -= 1;
        std::cout << "unsigned int (res)- 1(dec): " << std::dec << res << std::endl;
        std::cout << "unsigned int (res)- 1: " << std::hex 
        << std::showbase << std::setfill('0') << res << std::endl;

        res = ~(res);
        std::cout << "unsigned int (res) complimented (dec): " << std::dec << res << std::endl;
        std::cout << "unsigned int (res) complimented : " << std::hex 
        << std::showbase << std::setfill('0') << res << std::endl;


        REQUIRE(varInt == (int signed)0xFFFFFF09);
    }
#endif
}
