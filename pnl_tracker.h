#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>

class PnLTracker {
    std::unordered_map<std::string, double> realizedPnL;
    std::unordered_map<std::string, int> holdings;

public:
    void recordTrade(const std::string& buyerId, const std::string& sellerId, int quantity, double price);

    void printPnL(double marketPrice) const;
};
