#include "order_book.h"
#include <iostream>
#include <algorithm>

OrderBook::OrderBook() {}

void OrderBook::addOrder(const Order& order) {
    std::lock_guard<std::mutex> lock(mtx);

    if (order.getOrderType() == OrderType::MARKET) {
        std::cout << "Market order received: "
            << (order.getOrderSide() == OrderSide::BUY ? "BUY" : "SELL")
            << " quantity " << order.getQuantity() << "\n";

        Order marketOrder = order;

        if (marketOrder.getOrderSide() == OrderSide::BUY) {
            while (!sellOrders.empty() && marketOrder.getQuantity() > 0) {
                auto bestSell = sellOrders.begin();
                Order sellOrder = bestSell->second.front();

                int tradedQty = std::min(marketOrder.getQuantity(), sellOrder.getQuantity());
                double tradePrice = sellOrder.getPrice();

                std::cout << "Matched " << tradedQty << " @ " << tradePrice
                    << " between " << marketOrder.getTraderId()
                    << " and " << sellOrder.getTraderId() << "\n";

                pnlTracker.recordTrade(marketOrder.getTraderId(), sellOrder.getTraderId(), tradedQty, tradePrice);

                bestSell->second.pop();
                if (sellOrder.getQuantity() > tradedQty) {
                    sellOrder.setQuantity(sellOrder.getQuantity() - tradedQty);
                    bestSell->second.push(sellOrder);
                }

                if (bestSell->second.empty()) sellOrders.erase(bestSell);
                marketOrder.setQuantity(marketOrder.getQuantity() - tradedQty);
            }
        }
        else {
            while (!buyOrders.empty() && marketOrder.getQuantity() > 0) {
                auto bestBuy = buyOrders.rbegin();
                Order buyOrder = bestBuy->second.front();

                int tradedQty = std::min(marketOrder.getQuantity(), buyOrder.getQuantity());
                double tradePrice = buyOrder.getPrice();

                std::cout << "Matched " << tradedQty << " @ " << tradePrice
                    << " between " << buyOrder.getTraderId()
                    << " and " << marketOrder.getTraderId() << "\n";

                pnlTracker.recordTrade(buyOrder.getTraderId(), marketOrder.getTraderId(), tradedQty, tradePrice);

                bestBuy->second.pop();
                if (buyOrder.getQuantity() > tradedQty) {
                    buyOrder.setQuantity(buyOrder.getQuantity() - tradedQty);
                    bestBuy->second.push(buyOrder);
                }

                if (bestBuy->second.empty()) buyOrders.erase(std::next(bestBuy).base());
                marketOrder.setQuantity(marketOrder.getQuantity() - tradedQty);
            }
        }

        if (marketOrder.getQuantity() > 0) {
            std::cout << "Market order not fully filled. Remaining quantity: "
                << marketOrder.getQuantity() << "\n";
        }

    }
    else {
        if (order.getOrderSide() == OrderSide::BUY) {
            buyOrders[order.getPrice()].push(order);
        }
        else {
            sellOrders[order.getPrice()].push(order);
        }

        allOrders[order.getOrderId()] = order;

        std::cout << "Limit order added: "
            << (order.getOrderSide() == OrderSide::BUY ? "BUY" : "SELL")
            << " at price " << order.getPrice()
            << " quantity " << order.getQuantity() << "\n";

        matchOrders();
    }
}

void OrderBook::matchOrders() {
    while (!buyOrders.empty() && !sellOrders.empty()) {
        auto bestBuy = buyOrders.rbegin();
        auto bestSell = sellOrders.begin();

        if (bestBuy->first >= bestSell->first) {
            Order buyOrder = bestBuy->second.front();
            Order sellOrder = bestSell->second.front();

            int tradedQty = std::min(buyOrder.getQuantity(), sellOrder.getQuantity());
            double tradePrice = sellOrder.getPrice();

            std::cout << "Matched " << tradedQty << " @ " << tradePrice
                << " between " << buyOrder.getTraderId()
                << " and " << sellOrder.getTraderId() << "\n";

            pnlTracker.recordTrade(buyOrder.getTraderId(), sellOrder.getTraderId(), tradedQty, tradePrice);

            bestBuy->second.pop();
            bestSell->second.pop();

            if (buyOrder.getQuantity() > tradedQty) {
                buyOrder.setQuantity(buyOrder.getQuantity() - tradedQty);
                bestBuy->second.push(buyOrder);
            }

            if (sellOrder.getQuantity() > tradedQty) {
                sellOrder.setQuantity(sellOrder.getQuantity() - tradedQty);
                bestSell->second.push(sellOrder);
            }

            if (bestBuy->second.empty()) buyOrders.erase(std::next(bestBuy).base());
            if (bestSell->second.empty()) sellOrders.erase(bestSell);

            allOrders.erase(buyOrder.getOrderId());
            allOrders.erase(sellOrder.getOrderId());
        }
        else {
            break;
        }
    }
}

void OrderBook::printMarketDepth() {
    std::lock_guard<std::mutex> lock(mtx);

    std::cout << "\n=== Market Depth ===\n";
    std::cout << "Asks:\n";
    for (const auto& [price, queue] : sellOrders) {
        if (!queue.empty()) {
            std::cout << "  " << price << " (" << queue.size() << " orders)\n";
        }
    }

    std::cout << "Bids:\n";
    for (auto it = buyOrders.rbegin(); it != buyOrders.rend(); ++it) {
        if (!it->second.empty()) {
            std::cout << "  " << it->first << " (" << it->second.size() << " orders)\n";
        }
    }

    double marketPrice = getCurrentMarketPrice();  // ✅ FIXED
    pnlTracker.printPnL(marketPrice);              // ✅ FIXED
}

void OrderBook::printOpenOrders() {
    std::cout << "\n=== Open Orders ===\n";
    for (const auto& [id, order] : allOrders) {
        std::cout << "ID: " << id
            << " | Trader: " << order.getTraderId()
            << " | Type: " << (order.getOrderType() == OrderType::LIMIT ? "LIMIT" : "MARKET")
            << " | Side: " << (order.getOrderSide() == OrderSide::BUY ? "BUY" : "SELL")
            << " | Price: " << order.getPrice()
            << " | Quantity: " << order.getQuantity() << "\n";
    }
}

bool OrderBook::cancelOrder(int orderId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = allOrders.find(orderId);
    if (it == allOrders.end()) return false;

    Order ord = it->second;
    auto& book = (ord.getOrderSide() == OrderSide::BUY) ? buyOrders : sellOrders;

    auto priceIt = book.find(ord.getPrice());
    if (priceIt != book.end()) {
        std::queue<Order>& q = priceIt->second;
        std::queue<Order> newQ;
        while (!q.empty()) {
            if (q.front().getOrderId() != orderId) {
                newQ.push(q.front());
            }
            q.pop();
        }
        priceIt->second = newQ;
        if (newQ.empty()) {
            book.erase(priceIt);
        }
    }

    allOrders.erase(it);
    return true;
}

bool OrderBook::modifyOrder(int orderId, int newQty, double newPrice) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = allOrders.find(orderId);
    if (it == allOrders.end()) return false;

    Order ord = it->second;
    if (!cancelOrder(orderId)) return false;

    ord.setQuantity(newQty);
    ord.setPrice(newPrice);
    addOrder(ord);
    return true;
}

double OrderBook::getCurrentMarketPrice() const {
    if (!buyOrders.empty() && !sellOrders.empty()) {
        return (buyOrders.rbegin()->first + sellOrders.begin()->first) / 2.0;
    }
    else if (!buyOrders.empty()) {
        return buyOrders.rbegin()->first;
    }
    else if (!sellOrders.empty()) {
        return sellOrders.begin()->first;
    }
    return -1.0;
}
