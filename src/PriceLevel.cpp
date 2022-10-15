
#include "PriceLevel.h"

void PriceLevel::addOrder(const Order& order)
{
    m_totalQuantity += order.getCurrentQuantity();
    m_orders.emplace_back(order);
}

void PriceLevel::removeNextOrder()
{
    m_totalQuantity -= m_orders.front().getCurrentQuantity();
    m_orders.pop_front();
}

void PriceLevel::reduceNextOrderQuantity(int32_t quantityDelta)
{
    m_orders.front().reduceQuantity(quantityDelta);
    m_totalQuantity -= quantityDelta;
}
