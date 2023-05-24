#ifndef UTILS_H
#define UTILS_H

#include "OrderEntry.h"

inline void COPY_STRING(char* dest, const char* src)
{
    std::memcpy(dest, src, 4);
}

#endif // UTILS_H
