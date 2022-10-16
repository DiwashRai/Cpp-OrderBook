
#include "PriceLevel.h"

std::list<Order>::iterator PriceLevel::appendOrder(const Order& order)
{
    m_totalQuantity += order.getCurrentQuantity();
    m_orders.emplace_back(order);
    return std::prev(m_orders.end());
}

void PriceLevel::removeNextOrder()
{
    m_totalQuantity -= m_orders.front().getCurrentQuantity();
    m_orders.pop_front();
}

void PriceLevel::reduceNextOrderQuantity(quantity_t quantityDelta)
{
    m_orders.front().reduceQuantity(quantityDelta);
    m_totalQuantity -= quantityDelta;
}

void PriceLevel::cancelOrder(std::list<Order>::iterator orderIter)
{
    m_totalQuantity -= orderIter->getCurrentQuantity();
    m_orders.erase(orderIter);
}
