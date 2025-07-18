#pragma once

#include "order.h"
#include "pnl_tracker.h"
#include <map>
#include <queue>
#include <mutex>
#include <unordered_map>

class OrderBook {
public:
    OrderBook();

    void addOrder(const Order& order);
    void matchOrders();
    void printMarketDepth();
    void printOpenOrders();
    bool cancelOrder(int orderId);
    bool modifyOrder(int orderId, int newQty, double newPrice);

private:
    std::map<double, std::queue<Order>> buyOrders;  // Highest price last (for reverse iteration)
    std::map<double, std::queue<Order>> sellOrders; // Lowest price first
    std::unordered_map<int, Order> allOrders;       // Tracks unmatched orders by ID

    PnLTracker pnlTracker;
    std::mutex mtx;

    double getCurrentMarketPrice() const;
};
