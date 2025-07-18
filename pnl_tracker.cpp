#include "pnl_tracker.h"

void PnLTracker::recordTrade(const std::string& buyerId, const std::string& sellerId, int quantity, double price) {
    double value = quantity * price;

    // Realized PnL
    realizedPnL[buyerId] -= value;
    realizedPnL[sellerId] += value;

    // Update holdings
    holdings[buyerId] += quantity;
    holdings[sellerId] -= quantity;
}

void PnLTracker::printPnL(double marketPrice) const {
    std::cout << "\n=== Trader PnL ===\n";

    for (const auto& [trader, pnl] : realizedPnL) {
        int position = 0;
        auto it = holdings.find(trader);
        if (it != holdings.end()) {
            position = it->second;
        }

        double unrealized = position * marketPrice;
        double totalPnL = pnl + unrealized;

        std::cout << "Trader " << trader
            << " | Realized: " << std::setw(6) << pnl
            << " | Holdings: " << std::setw(4) << position
            << " | Unrealized: " << std::setw(6) << unrealized
            << " | Total: " << std::setw(6) << totalPnL << "\n";
    }
}
