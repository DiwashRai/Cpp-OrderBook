#ifndef ORDER_H
#define ORDER_H

#include <cstdint>
#include "types.h"

class Order
{
public:
    Order(){}
    Order(price_t price, quantity_t quantity, bool bIsAsk)
            : m_price{price},
              m_initialQuantity{quantity},
              m_currentQuantity{quantity},
              m_bIsAsk{bIsAsk} {}

    Order(symbol_t symbol, trader_t trader, bool bIsAsk, price_t price, quantity_t quantity)
            : m_symbol{symbol},
              m_trader{trader},
              m_price{price},
              m_initialQuantity{quantity},
              m_currentQuantity{quantity},
              m_bIsAsk{bIsAsk} {}

    Order(symbol_t symbol, trader_t trader, bool bIsAsk, price_t price, quantity_t quantity, order_id_t orderId)
            : m_symbol{symbol},
              m_trader{trader},
              m_price{price},
              m_initialQuantity{quantity},
              m_currentQuantity{quantity},
              m_bIsAsk{bIsAsk},
              m_orderID{orderId} {}

    // Getters
    symbol_t getSymbol() const { return m_symbol; }
    trader_t getTrader() const { return m_trader; }
    order_id_t getOrderID() const { return m_orderID; }
    price_t getPrice() const { return m_price; }
    quantity_t getInitialQuantity() const { return m_initialQuantity; }
    quantity_t getCurrentQuantity() const { return m_currentQuantity; }
    bool isAsk() const { return m_bIsAsk; }

    // Setters
    void setOrderID(order_id_t orderId) { m_orderID = orderId; }
    void setTrader(trader_t trader) { m_trader = trader; }
    void setIsAsk(bool isAsk) { m_bIsAsk = isAsk; }

    // Methods
    void reduceQuantity(const quantity_t& quantityDelta);

private:
    symbol_t m_symbol;
    trader_t m_trader;
    order_id_t m_orderID;
    price_t m_price;
    quantity_t m_initialQuantity;
    quantity_t m_currentQuantity;
    bool m_bIsAsk;
};

#endif // ORDER_H
