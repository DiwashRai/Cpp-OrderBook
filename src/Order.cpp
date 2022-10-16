
#include "Order.h"
#include <cassert>

void Order::reduceQuantity(const quantity_t& quantityDelta)
{
    assert(m_currentQuantity >= quantityDelta);
    m_currentQuantity -= quantityDelta;
}

