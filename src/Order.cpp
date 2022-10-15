
#include "Order.h"
#include <cassert>

void Order::reduceQuantity(const int32_t& quantityDelta)
{
    assert(m_currentQuantity >= quantityDelta);
    m_currentQuantity -= quantityDelta;
}

