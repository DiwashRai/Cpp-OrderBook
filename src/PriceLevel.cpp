
#include "PriceLevel.h"

my::ListNode<Order>* PriceLevel::appendOrder(const Order& order)
{
    m_totalQuantity += order.getCurrentQuantity();
    m_orders.insert(order);
    return m_orders.back();
}

void PriceLevel::removeNextOrder()
{
    m_totalQuantity -= m_orders.front()->data.getCurrentQuantity();
    m_orders.pop_front();
}

void PriceLevel::reduceNextOrderQuantity(quantity_t quantityDelta)
{
    m_orders.front()->data.reduceQuantity(quantityDelta);
    m_totalQuantity -= quantityDelta;
}

void PriceLevel::cancelOrder(my::ListNode<Order>* nodePtr)
{
    m_totalQuantity -= nodePtr->data.getCurrentQuantity();
    m_orders.erase(nodePtr);
}
