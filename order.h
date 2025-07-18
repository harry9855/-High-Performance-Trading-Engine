#pragma once
#include <string>

enum class OrderType { LIMIT, MARKET };
enum class OrderSide { BUY, SELL };

class Order {
    int orderId;
    OrderType type;
    OrderSide side;
    double price;
    int quantity;
    std::string traderId;

public:
    Order() = default;

    Order(int id, OrderType type, OrderSide side, double price, int qty, const std::string& traderId)
        : orderId(id), type(type), side(side), price(price), quantity(qty), traderId(traderId) {
    }

    int getOrderId() const { return orderId; }
    OrderType getOrderType() const { return type; }
    OrderSide getOrderSide() const { return side; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    std::string getTraderId() const { return traderId; }

    void setQuantity(int qty) { quantity = qty; }
    void setPrice(double newPrice) { price = newPrice; }
};
