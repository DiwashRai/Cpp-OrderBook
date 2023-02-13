
#include <iostream>
#include "OrderBook.h"
#include "types.h"

void OrderBook::init()
{
    m_currentOrderID = 0;
    m_bids.clear();
    m_bids_table.clear();
    m_asks.clear();
    m_asks_table.clear();
    m_orders_table.clear();
}

void OrderBook::destroy() {}

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
    for (const auto & m_bid : m_bids)
    {
        std::cout << "Price: " << m_bid.second.getPrice()
                  << " - Quantity: " << m_bid.second.getTotalQuantity()
                  << " - Orders: " << m_bid.second.getTotalOrders()
                  << std::endl;
    }
}

void OrderBook::executeTrade(const symbol_t& symbol, const trader_t& buyTrader, const trader_t& sellTrader, const price_t& price, const quantity_t& quantity)
{
    // buy side execution report
    execution_t executionReport(symbol, buyTrader, false,price, quantity);
    //++m_currentExecutionID;
    execution(executionReport);

    // sell side execution report
    executionReport.setTrader(sellTrader);
    executionReport.setIsAsk(true);
    //++m_currentExecutionID;
    execution(executionReport);
}

#ifndef QUANTCUP_TEST
void OrderBook::execution(execution_t exec __attribute__((unused))) const
{
    /*
    const char* Side = exec.isAsk() ? "BUY " : "SELL";
    printf("[ORDER EXECUTION]: ID: %lu | Side: %s | Price: $%.2f | Quantity: %ld | Symbol: %s | Trader: %s\n", m_currentExecutionID, Side, exec.getPrice()/100.0, exec.getCurrentQuantity(), exec.getSymbol().cbegin(), exec.getTrader().cbegin());
    */
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
                    m_orders_table.erase(priceLevel.getNextOrder().getOrderID());
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
                        m_orders_table.erase(priceLevel.getNextOrder().getOrderID());
                        priceLevel.removeNextOrder();
                    }
                    if (priceLevel.getTotalOrders() == 0)
                    {
                        m_bids_table.erase((--m_bids.end())->first);
                        m_bids.erase(--m_bids.end());
                    }
                    return ++m_currentOrderID;
                }
            }
            // no more orders at price level
            m_bids_table.erase((--m_bids.end())->first);
            m_bids.erase(--m_bids.end());
        }

        // no more crosses. Add order to m_asks
        ++m_currentOrderID;
        order.setOrderID(m_currentOrderID);
        if (m_asks_table.find(order.getPrice()) != m_asks_table.end())
        {
            // price level exists. Access it from table and append order
            auto nodePtr = m_asks_table.find(order.getPrice())->second->appendOrder(order);
            m_orders_table.emplace(m_currentOrderID, std::pair<PriceLevel*, my::ListNode<Order>*>(m_asks_table.find(order.getPrice())->second, nodePtr));
        }
        else
        {
            // insert new price level and append order
            auto [it, bInserted] = m_asks.emplace(order.getPrice(), PriceLevel(order.getPrice()));
            auto nodePtr = it->second.appendOrder(order);

            m_asks_table.insert(std::make_pair(order.getPrice(), &(it->second)));
            m_orders_table.emplace(m_currentOrderID, std::pair<PriceLevel*, my::ListNode<Order>*>(&(it->second), nodePtr));
        }
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
                    m_orders_table.erase(priceLevel.getNextOrder().getOrderID());
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
                        m_orders_table.erase(priceLevel.getNextOrder().getOrderID());
                        priceLevel.removeNextOrder();
                    }
                    if (priceLevel.getTotalOrders() == 0)
                    {
                        m_asks_table.erase(m_asks.begin()->first);
                        m_asks.erase(m_asks.begin());
                    }
                    return ++m_currentOrderID;
                }
            }
            // no more orders at price level
            m_asks_table.erase(m_asks.begin()->first);
            m_asks.erase(m_asks.begin());
        }
        // no more crosses. Add order to m_bids
        ++m_currentOrderID;
        order.setOrderID(m_currentOrderID);
        if (m_bids_table.find(order.getPrice()) != m_bids_table.end())
        {
            // price level exists. Access it from table and append order
            auto nodePtr = m_bids_table.find(order.getPrice())->second->appendOrder(order);
            m_orders_table.emplace(m_currentOrderID, std::pair<PriceLevel*, my::ListNode<Order>*>(m_bids_table.find(order.getPrice())->second, nodePtr));
        }
        else
        {
            // insert new price level and append order
            auto [it, bInserted] = m_bids.emplace(order.getPrice(), PriceLevel(order.getPrice()));

            auto nodePtr = it->second.appendOrder(order);
            m_bids_table.insert(std::make_pair(order.getPrice(), &(it->second)));
            m_orders_table.emplace(m_currentOrderID, std::pair<PriceLevel*, my::ListNode<Order>*>(&(it->second), nodePtr));
        }

        return  m_currentOrderID;
    }
}

void OrderBook::cancel(order_id_t orderID)
{
    auto it = m_orders_table.find(orderID);
    if (it == m_orders_table.end())
        return;

    it->second.first->cancelOrder(it->second.second);
    m_orders_table.erase(it);
}

