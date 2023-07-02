#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <array>
#include <set>
#include <string>

#include "../quantcup/limits.h"
#include "../quantcup/types.h"
#include "OrderEntry.h"
#include "PriceLevel.h"

class OrderBook
{
  public:
    void init();
    void destroy();

    t_orderid limit(t_order& order);
    void cancel(t_orderid orderID);

  private:
    void executeTrade(const char* symbol,
                      const char* buyTrader,
                      const char* sellTrader,
                      const t_price& tradePrice,
                      const t_size& tradeSize);
    void execution(t_execution exec) const;

    t_orderid m_current_order_id = 0;
    t_price m_ask_min = MAX_PRICE + 1;
    t_price m_bid_max = MIN_PRICE - 1;
    std::array<PriceLevel, MAX_PRICE + 1> m_price_levels;
    std::array<OrderEntry, 1'010'000> m_orders;
};

#endif // ORDERBOOK_H
