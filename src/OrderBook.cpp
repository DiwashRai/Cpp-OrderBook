
#include <iostream>
#include "OrderBook.h"
#include "types.h"

void OrderBook::init()
{
    m_currentOrderID = 0;
    m_bids.clear();
    m_asks.clear();
    m_ordersTable.clear();
}

void OrderBook::destroy() {}

order_id_t OrderBook::addOrder(Order& order)
{
    std::map<long, PriceLevel>& priceLevelMap = order.isAsk() ? m_asks : m_bids;
    auto [it, bInserted] = priceLevelMap.emplace(order.getPrice(), PriceLevel(order.getPrice()));

    ++m_currentOrderID;
    order.setOrderID(m_currentOrderID);
    auto orderIter = it->second.appendOrder(order);
    m_ordersTable.emplace(m_currentOrderID, std::pair<PriceLevel&, std::list<Order>::iterator>(it->second, orderIter));
    return m_currentOrderID;
}

void OrderBook::printBook() const
{
    std::cout << "=========== Sell side =============\n";
    for (auto it = m_asks.rbegin(); it != m_asks.rend(); ++it)
    {
        std::cout << "Price: " << it->second.getPrice()
                  << " - Quantity: " << it->second.getTotalQuantity()
                  << " - Orders: " << it->second.getTotalOrders()
                  << std::endl;
    }
    std::cout << "=========== Buy side =============\n";
    for (auto it = m_bids.begin(); it != m_bids.end(); ++it)
    {
        std::cout << "Price: " << it->second.getPrice()
                  << " - Quantity: " << it->second.getTotalQuantity()
                  << " - Orders: " << it->second.getTotalOrders()
                  << std::endl;
    }
}

void OrderBook::executeTrade(const symbol_t& symbol, const trader_t& buyTrader, const trader_t& sellTrader, const price_t& price, const quantity_t& quantity)
{
    // buy side execution report
    execution_t executionReport(symbol, buyTrader, false,price, quantity);
    execution(executionReport);

    // sell side execution report
    executionReport.setTrader(sellTrader);
    executionReport.setIsAsk(true);
    execution(executionReport);
}

#ifndef QUANTCUP_TEST
void OrderBook::execution(execution_t exec) const
{
}
#endif

order_id_t OrderBook::limit(Order& order)
{
    if (order.isAsk()) /* sell order */
    {
        // check if sell order crosses with existing buy orders
        while (!m_bids.empty() && m_bids.rbegin()->first >= order.getPrice())
        {
            PriceLevel& priceLevel = m_bids.rbegin()->second;
            while (priceLevel.getTotalOrders() > 0)
            {
                if (order.getCurrentQuantity() > priceLevel.getNextOrder().getCurrentQuantity())
                {
                    // incoming sell order quantity greater than next bid quantity at price level
                    executeTrade(order.getSymbol(), priceLevel.getNextOrder().getTrader(),
                            order.getTrader(), priceLevel.getPrice(),
                            priceLevel.getNextOrder().getCurrentQuantity());
                    order.reduceQuantity(priceLevel.getNextOrder().getCurrentQuantity());
                    m_ordersTable.erase(priceLevel.getNextOrder().getOrderID());
                    priceLevel.removeNextOrder();
                }
                else
                {
                    // incoming sell order quantity less than next bid quantity at price level
                    executeTrade(order.getSymbol(), priceLevel.getNextOrder().getTrader(),
                            order.getTrader(), priceLevel.getPrice(),
                            order.getCurrentQuantity());
                    if (priceLevel.getNextOrder().getCurrentQuantity() > order.getCurrentQuantity())
                    {
                        priceLevel.reduceNextOrderQuantity(order.getCurrentQuantity());
                    }
                    else
                    {
                        m_ordersTable.erase(priceLevel.getNextOrder().getOrderID());
                        priceLevel.removeNextOrder();
                    }
                    if (priceLevel.getTotalOrders() == 0)
                        m_bids.erase(--m_bids.end());
                    return ++m_currentOrderID;
                }
            }
            // no more orders at price level
            m_bids.erase(--m_bids.end());
        }
        // no more crosses. Add order to m_asks
        auto [it, bInserted] = m_asks.emplace(order.getPrice(), PriceLevel(order.getPrice()));

        ++m_currentOrderID;
        order.setOrderID(m_currentOrderID);
        auto orderIter = it->second.appendOrder(order);
        m_ordersTable.emplace(m_currentOrderID, std::pair<PriceLevel&, std::list<Order>::iterator>(it->second, orderIter));
        return m_currentOrderID;
    }
    else /* buy order */
    {
        while (!m_asks.empty() && m_asks.begin()->first <= order.getPrice())
        {
            PriceLevel& priceLevel = m_asks.begin()->second;
            while(priceLevel.getTotalOrders() > 0)
            {
                if (order.getCurrentQuantity() > priceLevel.getNextOrder().getCurrentQuantity())
                {
                    // incoming buy order quantity greater than next ask quantity in current price level
                    executeTrade(order.getSymbol(), order.getTrader(),
                            priceLevel.getNextOrder().getTrader(), priceLevel.getPrice(),
                            priceLevel.getNextOrder().getCurrentQuantity());
                    order.reduceQuantity(priceLevel.getNextOrder().getCurrentQuantity());
                    m_ordersTable.erase(priceLevel.getNextOrder().getOrderID());
                    priceLevel.removeNextOrder();
                }
                else
                {
                    // incoming buy order quantity less than or equal to next ask quantity in current price level
                    executeTrade(order.getSymbol(), order.getTrader(),
                            priceLevel.getNextOrder().getTrader(), priceLevel.getPrice(),
                            order.getCurrentQuantity());
                    if (priceLevel.getNextOrder().getCurrentQuantity() > order.getCurrentQuantity())
                    {
                        priceLevel.reduceNextOrderQuantity(order.getCurrentQuantity());
                    }
                    else
                    {
                        m_ordersTable.erase(priceLevel.getNextOrder().getOrderID());
                        priceLevel.removeNextOrder();
                    }
                    if (priceLevel.getTotalOrders() == 0)
                        m_asks.erase(m_asks.begin());
                    return ++m_currentOrderID;
                }
            }
            // no more orders at price level
            m_asks.erase(m_asks.begin());
        }
        // no more crosses. Add order to m_bids
        auto [it, bInserted] = m_bids.emplace(order.getPrice(), PriceLevel(order.getPrice()));

        ++m_currentOrderID;
        order.setOrderID(m_currentOrderID);
        auto orderIter = it->second.appendOrder(order);
        m_ordersTable.emplace(m_currentOrderID, std::pair<PriceLevel&, std::list<Order>::iterator>(it->second, orderIter));
        return  m_currentOrderID;
    }
}

void OrderBook::cancel(order_id_t orderID)
{
    auto it = m_ordersTable.find(orderID);
    if (it == m_ordersTable.end())
        return;

    it->second.first.cancelOrder(it->second.second);
    m_ordersTable.erase(it);
}

