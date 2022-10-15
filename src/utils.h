#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "Order.h"

inline void printOrder(const Order& order)
{
    std::cout << "[Order] Price: " << order.getPrice() << " | Quantity: " << order.getCurrentQuantity() << "| Buy Side: " << order.isBuySide() << std::endl;
}

#endif // UTILS_H
