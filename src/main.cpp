
#include <iostream>
#include <set>
#include <vector>

#include "Order.h"
#include "PriceLevel.h"
#include "OrderBook.h"
#include "utils.h"

std::vector<Order> getOrdersList()
{
    std::vector<Order> ordersList;
    ordersList.emplace_back(121,40,false);
    ordersList.emplace_back(128,140,false);
    ordersList.emplace_back(117,70,false);
    ordersList.emplace_back(125,160,false);
    ordersList.emplace_back(96,180,false);
    ordersList.emplace_back(127,100,false);
    ordersList.emplace_back(96,170,false);
    ordersList.emplace_back(127,200,false);
    ordersList.emplace_back(125,180,true);
    ordersList.emplace_back(120,20,false);
    ordersList.emplace_back(112,160,false);
    ordersList.emplace_back(125,30,true);
    ordersList.emplace_back(91,30,true);
    ordersList.emplace_back(107,80,false);
    ordersList.emplace_back(94,100,false);
    ordersList.emplace_back(100,170,false);
    ordersList.emplace_back(114,100,true);
    ordersList.emplace_back(104,10,false);
    ordersList.emplace_back(110,190,false);
    ordersList.emplace_back(103,50,false);
    ordersList.emplace_back(106,190,false);
    ordersList.emplace_back(103,190,true);
    ordersList.emplace_back(124,60,true);
    ordersList.emplace_back(127,50,false);
    ordersList.emplace_back(116,20,false);
    ordersList.emplace_back(108,120,false);
    ordersList.emplace_back(96,90,false);
    ordersList.emplace_back(117,40,true);
    ordersList.emplace_back(91,20,false);
    ordersList.emplace_back(105,70,false);
    ordersList.emplace_back(118,30,false);
    ordersList.emplace_back(124,40,false);
    ordersList.emplace_back(91,80,true);
    ordersList.emplace_back(108,10,true);
    ordersList.emplace_back(109,110,true);
    ordersList.emplace_back(125,200,false);
    ordersList.emplace_back(124,140,false);
    ordersList.emplace_back(94,90,true);
    ordersList.emplace_back(127,120,true);
    ordersList.emplace_back(107,90,true);
    ordersList.emplace_back(115,160,true);
    ordersList.emplace_back(107,40,false);
    ordersList.emplace_back(105,150,true);

    return ordersList;
}

int main()
{
    OrderBook orderBook("IBM");
    std::vector<Order> orders = getOrdersList();
    for (auto& order : orders)
        orderBook.limit(order);
    orderBook.printBook();

    return 0;
}
