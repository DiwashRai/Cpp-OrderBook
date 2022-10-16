#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <array>


typedef int64_t price_t;
typedef int64_t quantity_t;
typedef uint64_t order_id_t;

typedef std::array<char, 4> symbol_t;
typedef std::array<char, 4> trader_t;

class Order;
typedef Order execution_t;

#endif
