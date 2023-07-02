
#include "utils.h"
#include <cstring>

#include "OrderBook.h"

void OrderBook::init()
{
    m_current_order_id = 0;
    m_ask_min = MAX_PRICE + 1;
    m_bid_max = MIN_PRICE - 1;
    std::memset(m_price_levels.data(), 0, sizeof(PriceLevel) * (MAX_PRICE + 1));
    std::memset(m_orders.data(), 0, sizeof(OrderEntry) * 1'010'000);
}

void OrderBook::destroy()
{
}

void OrderBook::executeTrade(const char* symbol, const char* buyTrader, const char* sellTrader,
                             const t_price& tradePrice, const t_size& tradeSize)
{
    if (tradeSize == 0)
        return;

    t_execution executionReport;
    COPY_STRING(executionReport.symbol, symbol);
    executionReport.price = tradePrice;
    executionReport.size = tradeSize;

    executionReport.side = Bid;
    COPY_STRING(executionReport.trader, buyTrader);
    execution(executionReport);

    executionReport.side = Ask;
    COPY_STRING(executionReport.trader, sellTrader);
    execution(executionReport);
}

#ifndef QUANTCUP_TEST
void OrderBook::execution(t_execution exec __attribute__((unused))) const
{
    /*
    const char* Side = exec.isAsk() ? "BUY " : "SELL";
    printf("[ORDER EXECUTION]: ID: %lu | Side: %s | Price: $%.2f | Quantity: %ld | Symbol: %s | Trader: %s\n",
    m_currentExecutionID, Side, exec.getPrice()/100.0, exec.getCurrentQuantity(), exec.getSymbol().cbegin(),
    exec.getTrader().cbegin());
    */
}
#endif

t_orderid OrderBook::limit(t_order& order)
{
    if (order.side == Bid)
    {
        // check bid price crosses ask price
        if (order.price >= m_ask_min)
        {
            PriceLevel* pPriceLevel = &m_price_levels[m_ask_min];
            while (order.price >= m_ask_min)
            {
                OrderEntry* pCurrOrderEntry = pPriceLevel->m_head;
                while (pCurrOrderEntry != nullptr)
                {
                    if (pCurrOrderEntry->m_size >= order.size)
                    {
                        executeTrade(order.symbol, order.trader, pCurrOrderEntry->m_trader, m_ask_min, order.size);
                        if (pCurrOrderEntry->m_size > order.size)
                            pCurrOrderEntry->m_size -= order.size;
                        else
                            pCurrOrderEntry = pCurrOrderEntry->m_next;

                        pPriceLevel->m_head = pCurrOrderEntry;
                        return ++m_current_order_id;
                    }
                    executeTrade(order.symbol, order.trader, pCurrOrderEntry->m_trader, m_ask_min,
                                 pCurrOrderEntry->m_size);
                    order.size -= pCurrOrderEntry->m_size;
                    pCurrOrderEntry->m_size = 0;
                    pCurrOrderEntry = pCurrOrderEntry->m_next;
                }
                pPriceLevel->m_head = nullptr;
                ++m_ask_min;
                ++pPriceLevel;
            }
        }

        // add order to bid side
        OrderEntry* pNewOrderEntry = &m_orders[++m_current_order_id];
        pNewOrderEntry->m_size = order.size;
        COPY_STRING(pNewOrderEntry->m_trader, order.trader);
        m_price_levels[order.price].insert(pNewOrderEntry);
        if (order.price > m_bid_max)
            m_bid_max = order.price;
        return m_current_order_id;
    }
    else
    {
        // check ask price crosses bid price
        if (order.price <= m_bid_max)
        {
            PriceLevel* pPriceLevel = &m_price_levels[m_bid_max];
            while (order.price <= m_bid_max)
            {
                OrderEntry* pCurrOrderEntry = pPriceLevel->m_head;
                while (pCurrOrderEntry != nullptr)
                {
                    if (pCurrOrderEntry->m_size >= order.size)
                    {
                        executeTrade(order.symbol, order.trader, pCurrOrderEntry->m_trader, m_bid_max, order.size);
                        if (pCurrOrderEntry->m_size > order.size)
                            pCurrOrderEntry->m_size -= order.size;
                        else
                            pCurrOrderEntry = pCurrOrderEntry->m_next;

                        pPriceLevel->m_head = pCurrOrderEntry;
                        return ++m_current_order_id;
                    }
                    executeTrade(order.symbol, order.trader, pCurrOrderEntry->m_trader, m_bid_max,
                                 pCurrOrderEntry->m_size);
                    order.size -= pCurrOrderEntry->m_size;
                    pCurrOrderEntry->m_size = 0;
                    pCurrOrderEntry = pCurrOrderEntry->m_next;
                }
                pPriceLevel->m_head = nullptr;
                --m_bid_max;
                --pPriceLevel;
            }
        }

        // add order to ask side
        OrderEntry* pNewOrderEntry = &m_orders[++m_current_order_id];
        pNewOrderEntry->m_size = order.size;
        COPY_STRING(pNewOrderEntry->m_trader, order.trader);
        m_price_levels[order.price].insert(pNewOrderEntry);
        if (order.price > m_ask_min)
            m_ask_min = order.price;
        return m_current_order_id;
    }
}

void OrderBook::cancel(t_orderid orderID)
{
    m_orders[orderID].m_size = 0;
}
