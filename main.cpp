#include "order_book.h"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    OrderBook orderBook;
    int orderIdCounter = 1;

    std::cout << "\n--- Welcome to High Performance Trading Engine ---\n";
    std::cout << "Commands:\n"
        << "  LIMIT BUY <price> <qty> <traderId>\n"
        << "  LIMIT SELL <price> <qty> <traderId>\n"
        << "  MARKET BUY <qty> <traderId>\n"
        << "  MARKET SELL <qty> <traderId>\n"
        << "  SHOW (to display order book and PnL)\n"
        << "  SHOW ORDERS (to display all unmatched orders)\n"
        << "  CANCEL <orderId> (to cancel unmatched limit order)\n"
        << "  MODIFY <orderId> <newQty> <newPrice> (to modify unmatched order)\n"
        << "  EXIT (to quit)\n\n";

    std::string input;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        std::stringstream ss(input);

        std::string command;
        ss >> command;

        if (command == "EXIT") {
            std::cout << "\nFinal Market Depth:\n";
            orderBook.printMarketDepth();
            break;
        }
        else if (command == "SHOW") {
            std::string sub;
            ss >> sub;
            if (sub == "ORDERS") {
                orderBook.printOpenOrders();
            }
            else {
                orderBook.printMarketDepth();
            }
        }
        else if (command == "CANCEL") {
            int id;
            ss >> id;
            if (orderBook.cancelOrder(id)) {
                std::cout << "Order ID " << id << " successfully cancelled.\n";
            }
            else {
                std::cout << "Order ID " << id << " not found or already matched.\n";
            }
        }
        else if (command == "MODIFY") {
            int id, qty;
            double price;
            ss >> id >> qty >> price;
            if (orderBook.modifyOrder(id, qty, price)) {
                std::cout << "Order ID " << id << " modified to quantity " << qty << " and price " << price << ".\n";
            }
            else {
                std::cout << "Modification failed. Order ID " << id << " not found.\n";
            }
        }
        else if (command == "LIMIT") {
            std::string side;
            double price;
            int qty;
            std::string traderId;
            ss >> side >> price >> qty >> traderId;
            OrderSide orderSide = (side == "BUY") ? OrderSide::BUY : OrderSide::SELL;
            Order newOrder(orderIdCounter++, OrderType::LIMIT, orderSide, price, qty, traderId);
            orderBook.addOrder(newOrder);
        }
        else if (command == "MARKET") {
            std::string side;
            int qty;
            std::string traderId;
            ss >> side >> qty >> traderId;
            OrderSide orderSide = (side == "BUY") ? OrderSide::BUY : OrderSide::SELL;
            Order newOrder(orderIdCounter++, OrderType::MARKET, orderSide, 0.0, qty, traderId);
            orderBook.addOrder(newOrder);
        }
        else {
            std::cout << "Unknown command. Please try again.\n";
        }
    }

    return 0;
}
