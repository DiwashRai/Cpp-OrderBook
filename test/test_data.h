
#include "../src/Order.h"
#include <vector>

struct TestData
{
    static std::vector<Order> getIBMOrders()
    {
        symbol_t ibm{"IBM"};
        trader_t bob{"BOB"};
        trader_t mark{"MRK"};
        trader_t phil{"PHL"};
        trader_t jane{"JNE"};
        trader_t chris{"CHR"};

        std::vector<Order> IBMOrders{
            {ibm,bob,true,121,40},
            {ibm,mark,true,128,140},
            {ibm,phil,true,117,70},
            {ibm,jane,true,125,160},
            {ibm,chris,true,96,180},
            {ibm,mark,true,127,100},
            {ibm,chris,true,96,170},
            {ibm,mark,true,127,200},
            {ibm,bob,false,125,180},
            {ibm,bob,true,120,20},
            {ibm,chris,true,112,160},
            {ibm,bob,false,125,30},
            {ibm,chris,false,91,30},
            {ibm,mark,true,107,80},
            {ibm,jane,true,94,100},
            {ibm,bob,true,100,170},
            {ibm,mark,false,114,100},
            {ibm,chris,true,104,10},
            {ibm,bob,true,110,190},
            {ibm,mark,true,103,50},
            {ibm,chris,true,106,190},
            {ibm,bob,false,103,190},
            {ibm,jane,false,124,60},
            {ibm,chris,true,127,50},
            {ibm,phil,true,116,20},
            {ibm,bob,true,108,120},
            {ibm,phil,true,96,90},
            {ibm,bob,false,117,40},
            {ibm,jane,true,91,20},
            {ibm,phil,true,105,70},
            {ibm,chris,true,118,30},
            {ibm,chris,true,124,40},
            {ibm,phil,false,91,80},
            {ibm,jane,false,108,10},
            {ibm,bob,false,109,110},
            {ibm,phil,true,125,200},
            {ibm,bob,true,124,140},
            {ibm,bob,false,94,90},
            {ibm,phil,false,127,120},
            {ibm,bob,false,107,90},
            {ibm,mark,false,115,160},
            {ibm,phil,true,107,40},
            {ibm,jane,false,105,150},
        };

        return IBMOrders;
    }

    static std::vector<Order> getIBMExecutions()
    {
        symbol_t ibm{"IBM"};
        trader_t bob{"BOB"};
        trader_t mark{"MRK"};
        trader_t phil{"PHL"};
        trader_t jane{"JNE"};
        trader_t chris{"CHR"};

        std::vector<Order> IBMExecutions{
            {ibm,bob,false,96,180},
            {ibm,bob,false,96,30},
            {ibm,mark,false,94,100},
            {ibm,bob,false,96,140},
            {ibm,bob,false,100,50},
            {ibm,jane,false,100,60},
            {ibm,bob,false,96,40},
            {ibm,chris,false,91,20},
            {ibm,jane,false,96,10},
            {ibm,bob,false,96,40},
            {ibm,bob,false,100,60},
            {ibm,bob,false,103,10},
            {ibm,phil,false,103,40},
            {ibm,phil,false,104,10},
            {ibm,phil,false,105,70},
            {ibm,bob,false,106,90},
            {ibm,mark,false,106,100},
            {ibm,mark,false,107,60},
       };

        return IBMExecutions;
    }
};

