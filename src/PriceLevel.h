#ifndef PRICELEVEL_H
#define PRICELEVEL_H

#include <list>
#include "Order.h"
#include "my_list.h"

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
    my::ListNode<Order>* appendOrder(const Order& order);
    const Order& getNextOrder() { return m_orders.front()->data; };
    void reduceNextOrderQuantity(quantity_t quantityDelta);
    void removeNextOrder();
    void cancelOrder(my::ListNode<Order>* nodePtr);

private:
    price_t m_price;
    quantity_t m_totalQuantity;
    //std::list<Order> m_orders;
    my::DoublyLinkedList<Order> m_orders;
};

#endif

