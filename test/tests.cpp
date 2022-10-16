
#include <catch2/catch_test_macros.hpp>

#include "../src/Order.h"
#include "../src/PriceLevel.h"
#include "../src/OrderBook.h"
#include "test_data.h"
#include <iostream>

TEST_CASE("Order class construction", "[Order]")
{
    int64_t price = 100;
    int32_t quantity = 50;
    bool isAsk = true;
    Order first(price, quantity, isAsk);

    REQUIRE(first.getPrice() == price);
    REQUIRE(first.getInitialQuantity() == quantity);
    REQUIRE(first.getCurrentQuantity() == quantity);
    REQUIRE(first.isAsk());
}

TEST_CASE("Reduce quantity from order", "[Order]")
{
    int64_t price = 100;
    int32_t quantity = 50;
    bool isBuy = true;
    Order order(price, quantity, isBuy);

    SECTION("Basic quantity reduction")
    {
        int32_t reduction = 25;
        order.reduceQuantity(reduction);
        REQUIRE(order.getCurrentQuantity() == (quantity - reduction));
    }
}

TEST_CASE("Test PriceLevel addOrder", "[PriceLevel]")
{
    PriceLevel priceLevel(100);
    REQUIRE(priceLevel.getTotalOrders() == 0);

    Order order(100, 50, true);
    priceLevel.appendOrder(order);
    REQUIRE(priceLevel.getTotalOrders() == 1);

    priceLevel.appendOrder(order);
    priceLevel.appendOrder(order);
    priceLevel.appendOrder(order);
    priceLevel.appendOrder(order);
    REQUIRE(priceLevel.getTotalOrders() == 5);
}

TEST_CASE("Test PriceLevel getTotalQuantity", "[PriceLevel]")
{
    PriceLevel priceLevel(100);

    int64_t quantity = 50;
    int64_t total = quantity;
    priceLevel.appendOrder(Order(100, quantity, true));
    REQUIRE(priceLevel.getTotalQuantity() == total);

    quantity = 60;
    total += quantity;
    priceLevel.appendOrder(Order(100, quantity, true));
    REQUIRE(priceLevel.getTotalQuantity() == total);

    quantity = 70;
    total += quantity;
    priceLevel.appendOrder(Order(100, quantity, true));
    REQUIRE(priceLevel.getTotalQuantity() == total);
}

TEST_CASE("Test removeOrder reduces quantity of priceLevel","[PriceLevel]")
{
    int64_t price = 100;
    PriceLevel priceLevel(price);
    priceLevel.appendOrder(Order(price, 10, true));
    priceLevel.appendOrder(Order(price, 20, true));
    priceLevel.appendOrder(Order(price, 30, true));
    priceLevel.appendOrder(Order(price, 40, true));
    priceLevel.appendOrder(Order(price, 50, true));
    auto quantity = priceLevel.getTotalQuantity();

    priceLevel.removeNextOrder();
    quantity -= 10;
    REQUIRE(priceLevel.getTotalQuantity() == quantity);
    priceLevel.removeNextOrder();
    quantity -= 20;
    priceLevel.removeNextOrder();
    quantity -= 30;
    REQUIRE(priceLevel.getTotalQuantity() == quantity);
    priceLevel.removeNextOrder();
    quantity -= 40;
    priceLevel.removeNextOrder();
    quantity -= 50;
    REQUIRE(priceLevel.getTotalQuantity() == quantity);
}

TEST_CASE("Test reduceNextOrderQuantity reduces priceLevel quantity", "[PriceLevel]")
{
    int64_t price = 100;
    PriceLevel priceLevel(price);
    priceLevel.appendOrder(Order(price, 1000, true));
    priceLevel.appendOrder(Order(price, 10, true));
    auto quantity = priceLevel.getTotalQuantity();

    priceLevel.reduceNextOrderQuantity(100);
    quantity -= 100;
    REQUIRE(priceLevel.getTotalQuantity() == quantity);

    priceLevel.reduceNextOrderQuantity(200);
    quantity -= 200;
    REQUIRE(priceLevel.getTotalQuantity() == quantity);

    priceLevel.reduceNextOrderQuantity(300);
    quantity -= 300;
    REQUIRE(priceLevel.getTotalQuantity() == quantity);
}

class OrderBookTest : public OrderBook
{
public:
    using OrderBook::OrderBook;
    std::vector<Order> m_executedOrders;

    void executeTrade(const symbol_t& symbol, const trader_t& buyTrader, const trader_t& sellTrader, const price_t& price, const quantity_t& quantity)
    {
        m_executedOrders.emplace_back(symbol, buyTrader, false, price, quantity);
    };
};

TEST_CASE("Test matching with IBM input", "[Limit]")
{
    OrderBookTest orderBook("IBM");
    REQUIRE(orderBook.m_executedOrders.empty());

    std::vector<Order> orders = TestData::getIBMOrders();
    for(auto& order : orders)
        orderBook.limit(order);

    std::vector<execution_t> executedOrders = TestData::getIBMExecutions();
    REQUIRE(orderBook.m_executedOrders.size() == executedOrders.size());

    for (std::size_t i = 0; i < executedOrders.size(); ++i)
    {
        REQUIRE(orderBook.m_executedOrders[i].getSymbol() == executedOrders[i].getSymbol());
        REQUIRE(orderBook.m_executedOrders[i].getTrader() == executedOrders[i].getTrader());
        REQUIRE(orderBook.m_executedOrders[i].getPrice() == executedOrders[i].getPrice());
        REQUIRE(orderBook.m_executedOrders[i].getInitialQuantity() == executedOrders[i].getInitialQuantity());
        REQUIRE(orderBook.m_executedOrders[i].getCurrentQuantity() == executedOrders[i].getCurrentQuantity());
        REQUIRE(orderBook.m_executedOrders[i].isAsk() == executedOrders[i].isAsk());
    }
}

