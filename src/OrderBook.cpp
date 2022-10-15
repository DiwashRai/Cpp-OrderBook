
#include <iostream>
#include "OrderBook.h"

void OrderBook::addOrder(const Order& order)
{
    std::map<long, PriceLevel>& priceLevelMap = order.isBuySide() ? m_bids : m_asks;
    auto [it, bInserted] = priceLevelMap.emplace(order.getPrice(), PriceLevel(order.getPrice()));
    it->second.addOrder(order);
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

void OrderBook::execute_trade(int64_t price, int32_t size)
{
    std::cout << "Size: " << size << " - Price: " << price << std::endl;
}

void OrderBook::limit(Order& order)
{
    if (order.isBuySide())
    {
        while (!m_asks.empty() && m_asks.begin()->first <= order.getPrice())
        {
            PriceLevel& priceLevel = m_asks.begin()->second;
            while(priceLevel.getTotalOrders() > 0)
            {
                if (order.getCurrentQuantity() > priceLevel.getNextOrder().getCurrentQuantity())
                {
                    // incoming buy order quantity greater than next ask quantity in current price level
                    execute_trade(priceLevel.getPrice(), priceLevel.getNextOrder().getCurrentQuantity());
                    order.reduceQuantity(priceLevel.getNextOrder().getCurrentQuantity());
                    priceLevel.removeNextOrder();
                }
                else
                {
                    // incoming buy order quantity less than or equal to next ask quantity in current price level
                    execute_trade(priceLevel.getPrice(), order.getCurrentQuantity());
                    if (priceLevel.getNextOrder().getCurrentQuantity() > order.getCurrentQuantity())
                        priceLevel.reduceNextOrderQuantity(order.getCurrentQuantity());
                    else
                        priceLevel.removeNextOrder();
                    if (priceLevel.getTotalOrders() == 0)
                        m_asks.erase(m_asks.begin());
                    return;
                }
            }
            // no more orders at price level
            m_asks.erase(m_asks.begin());
        }
        // no more crosses. Add order to m_bids
        auto [it, bInserted] = m_bids.emplace(order.getPrice(), PriceLevel(order.getPrice()));
        it->second.addOrder(order);
        return;
    }
    else /* incoming order = sell */
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
                    execute_trade(priceLevel.getPrice(), priceLevel.getNextOrder().getCurrentQuantity());
                    order.reduceQuantity(priceLevel.getNextOrder().getCurrentQuantity());
                    priceLevel.removeNextOrder();
                }
                else
                {
                    // incoming sell order quantity less than next bid quantity at price level
                    execute_trade(priceLevel.getPrice(), order.getCurrentQuantity());
                    if (priceLevel.getNextOrder().getCurrentQuantity() > order.getCurrentQuantity())
                        priceLevel.reduceNextOrderQuantity(order.getCurrentQuantity());
                    else
                        priceLevel.removeNextOrder();
                    if (priceLevel.getTotalOrders() == 0)
                        m_bids.erase(m_bids.end());
                    return;
                }
            }
            // no more orders at price level
            m_bids.erase(m_bids.end());
        }
        // no more crosses. Add order to m_asks
        auto [it, bInserted] = m_asks.emplace(order.getPrice(), PriceLevel(order.getPrice()));
        it->second.addOrder(order);
        return;
    }
}
