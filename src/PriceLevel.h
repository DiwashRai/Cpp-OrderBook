#ifndef PRICELEVEL_H
#define PRICELEVEL_H

#include "OrderEntry.h"

struct PriceLevel
{
    void insert(OrderEntry* order)
    {
        if (m_head)
            m_tail->m_next = order;
        else
            m_head = order;
        m_tail = order;
    }

    OrderEntry* m_head = nullptr;
    OrderEntry* m_tail = nullptr;
};

#endif
