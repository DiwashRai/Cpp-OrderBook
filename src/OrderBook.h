#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <set>
#include <map>
#include <unordered_map>
#include <string>

#include "Order.h"
#include "PriceLevel.h"

class OrderBook
{
public:
    OrderBook(std::string security)
        : m_security(security) {};

    void limit(Order& order);
    void addOrder(const Order& order);
    void printBook() const;

private:
    void execute_trade(int64_t price, int32_t size);

    std::string m_security;
    std::map<long, PriceLevel> m_bids;
    std::map<long, PriceLevel> m_asks;
};

#endif //ORDERBOOK_H
