#ifndef ORDER_H
#define ORDER_H

#include <cstdint>

class Order
{
public:
    Order(int64_t price, int32_t quantity, bool bBuy_side)
            : m_price(price),
              m_initialQuantity(quantity),
              m_currentQuantity(quantity),
              m_bBuySide(bBuy_side),
              m_orderID(0) {}

    // Getters
    int64_t getPrice() const { return m_price; }
    int32_t getInitialQuantity() const { return m_initialQuantity; }
    int32_t getCurrentQuantity() const { return m_currentQuantity; }
    bool isBuySide() const { return m_bBuySide; }
    uint64_t getOrderID() const { return m_orderID; }

    // Methods
    void reduceQuantity(const int32_t& quantityDelta);

private:
    int64_t m_price;
    int32_t m_initialQuantity;
    int32_t m_currentQuantity;
    bool m_bBuySide;
    uint64_t m_orderID;
};

#endif // ORDER_H
