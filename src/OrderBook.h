#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <set>
#include <map>
#include <unordered_map>
#include <string>

#include "Order.h"
#include "PriceLevel.h"

#ifdef UNIT_TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif

class OrderBook
{
public:
    OrderBook(std::string security)
        : m_security{security},
          m_currentOrderID{0},
          m_currentExecutionID{0} {};

    void init();
    void destroy();

    order_id_t limit(Order& order);
    void cancel(order_id_t orderID);
    order_id_t addOrder(Order& order);
    void printBook() const;

private:
    VIRTUAL void executeTrade(const symbol_t& symbol, const trader_t& buyTrader, const trader_t& sellTrader, const price_t& price, const quantity_t& quantity);
    void execution(execution_t exec) const;

    std::string m_security;
    order_id_t m_currentOrderID;
    order_id_t m_currentExecutionID;
    std::map<price_t, PriceLevel> m_bids;
    std::map<price_t, PriceLevel> m_asks;
    std::unordered_map<order_id_t, std::pair<PriceLevel&, std::list<Order>::iterator>> m_ordersTable;
};

#endif //ORDERBOOK_H
