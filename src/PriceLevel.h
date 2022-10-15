#ifndef PRICELEVEL_H
#define PRICELEVEL_H

#include <list>
#include "Order.h"

class PriceLevel
{
public:
    PriceLevel(int64_t price)
        : m_price(price),
          m_totalQuantity{0} {};

    // getters
    int64_t getPrice() const { return m_price; }
    std::size_t getTotalOrders() const { return m_orders.size(); };
    int64_t getTotalQuantity() const { return m_totalQuantity; };

    // methods
    void addOrder(const Order& order);
    const Order& getNextOrder() { return m_orders.front(); };
    void reduceNextOrderQuantity(int32_t quantityDelta);
    void removeNextOrder();

private:
    int64_t m_price;
    int64_t m_totalQuantity;
    std::list<Order> m_orders;
};

#endif

