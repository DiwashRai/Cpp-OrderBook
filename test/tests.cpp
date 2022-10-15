
#include <catch2/catch_test_macros.hpp>

#include "../src/Order.h"
#include "../src/PriceLevel.h"

TEST_CASE("Order class construction", "[Order]")
{
    int64_t price = 100;
    int32_t quantity = 50;
    bool isBuy = true;
    Order first(price, quantity, isBuy);

    REQUIRE(first.getPrice() == price);
    REQUIRE(first.getInitialQuantity() == quantity);
    REQUIRE(first.getCurrentQuantity() == quantity);
    REQUIRE(first.isBuySide());
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
    priceLevel.addOrder(order);
    REQUIRE(priceLevel.getTotalOrders() == 1);

    priceLevel.addOrder(order);
    priceLevel.addOrder(order);
    priceLevel.addOrder(order);
    priceLevel.addOrder(order);
    REQUIRE(priceLevel.getTotalOrders() == 5);
}

TEST_CASE("Test PriceLevel getTotalQuantity", "[PriceLevel]")
{
    PriceLevel priceLevel(100);

    int64_t quantity = 50;
    int64_t total = quantity;
    priceLevel.addOrder(Order(100, quantity, true));
    REQUIRE(priceLevel.getTotalQuantity() == total);

    quantity = 60;
    total += quantity;
    priceLevel.addOrder(Order(100, quantity, true));
    REQUIRE(priceLevel.getTotalQuantity() == total);

    quantity = 70;
    total += quantity;
    priceLevel.addOrder(Order(100, quantity, true));
    REQUIRE(priceLevel.getTotalQuantity() == total);
}

TEST_CASE("Test removeOrder reduces quantity of priceLevel","[PriceLevel]")
{
    int64_t price = 100;
    PriceLevel priceLevel(price);
    priceLevel.addOrder(Order(price, 10, true));
    priceLevel.addOrder(Order(price, 20, true));
    priceLevel.addOrder(Order(price, 30, true));
    priceLevel.addOrder(Order(price, 40, true));
    priceLevel.addOrder(Order(price, 50, true));
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
    priceLevel.addOrder(Order(price, 1000, true));
    priceLevel.addOrder(Order(price, 10, true));
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

