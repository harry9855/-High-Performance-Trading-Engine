# ⚡ High Performance Trading Engine

> A blazing-fast, multi-threaded trading engine written in C++ that simulates how modern stock exchanges process, match, and execute orders.

---

## 📌 What is this project?

The **High Performance Trading Engine** is a real-time simulation of a financial market’s backend infrastructure. It handles both **limit** and **market** orders using **FIFO-based price-time priority**, matching buy and sell orders efficiently — just like real exchanges (e.g., NSE/BSE/NYSE).

---

## 🎯 Why did I create this project?

- 🚀 To dive deep into **system design** and **performance-critical architectures**.
- 💡 To simulate how real-world **trading systems work under the hood**.
- 🧠 To master **C++ STL**, object-oriented programming, and multithreading.
- 📚 To build a standout project for internships, jobs, and interviews (especially FinTech roles).
- 🛠️ To explore concepts like **PnL calculation**, **holdings**, and **real-time matching logic**.

---

## 💡 What does this project do?

- Accepts **Limit** and **Market Orders**
- Executes trades based on **price-time priority (FIFO)**
- Allows:
  - 📌 Order Modification (`MODIFY`)
  - ❌ Order Cancellation (`CANCEL`)
- Tracks:
  - ✅ Trade History
  - 📊 Real-Time Order Book
  - 💼 Holdings per user
  - 📈 Real-Time Profit and Loss (PnL)

---

## ⚙️ Features

- 🏛 Fully functional buy/sell order book
- ⏱ Accurate FIFO matching logic
- 🔁 Multi-threaded matching engine
- 🧠 Clean and modular C++ design
- 📊 PnL and user holdings tracking
- 🧪 Easy test functions for automation

---

## 🛠️ Tech Stack

| Tech             | Purpose                           |
|------------------|------------------------------------|
| `C++17`          | Core application logic             |
| `STL`            | Efficient containers and algorithms|
| `Multithreading` | High-performance concurrent engine |

---

## ✅ Usage Example

```cpp
OrderBook engine;

engine.placeOrder("LIMIT", "BUY", 100, 10);    // Buy 10 units @ 100
engine.placeOrder("LIMIT", "SELL", 101, 5);    // Sell 5 units @ 101
engine.placeOrder("MARKET", "SELL", 5);        // Market Sell

engine.modifyOrder(10001, 20);                 // Modify order quantity
engine.cancelOrder(10002);                     // Cancel an order

engine.printOrderBook();                       // View order book
engine.printTradeHistory();                    // All executed trades
engine.printPnL();                             // Real-time PnL & Holdings
