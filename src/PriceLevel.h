#ifndef PRICELEVEL_H
#define PRICELEVEL_H

#include <list>
#include "Order.h"

class PriceLevel
{
public:
    PriceLevel(price_t price)
        : m_price(price),
          m_totalQuantity{0} {};

    // getters
    price_t getPrice() const { return m_price; }
    std::size_t getTotalOrders() const { return m_orders.size(); };
    quantity_t getTotalQuantity() const { return m_totalQuantity; };

    // methods
    std::list<Order>::iterator appendOrder(const Order& order);
    const Order& getNextOrder() { return m_orders.front(); };
    void reduceNextOrderQuantity(quantity_t quantityDelta);
    void removeNextOrder();
    void cancelOrder(std::list<Order>::iterator orderIter);

private:
    price_t m_price;
    quantity_t m_totalQuantity;
    std::list<Order> m_orders;
};

#endif

