#ifndef ORDER_H
#define ORDER_H

#include "../quantcup/types.h"

struct OrderEntry
{
    OrderEntry* m_next;
    char m_trader[4];
    t_size m_size;
};

#endif // ORDER_H
