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

# Place a LIMIT BUY order: Buy 100 shares at ₹10 by Trader T1
> LIMIT BUY 100 10 T1

# Place a LIMIT SELL order: Sell 100 shares at ₹10 by Trader T2
> LIMIT SELL 100 10 T2

# Matches both orders (T1 buys from T2). Displays order book and trade history
> SHOW

# Place a MARKET BUY order: Buy 5 shares at the best available price by Trader T3
> MARKET BUY 5 T3

# Cancel an existing order by ID (e.g., order ID 1)
> CANCEL 1

# Modify order with ID 2: change quantity to 20 and price to ₹105
> MODIFY 2 20 105

# Display all open/pending orders with details
> SHOW ORDERS

# Exit the trading engine
> EXIT
```
## 🖼️ Screenshot

![CLI Screenshot](https://github.com/harry9855/-High-Performance-Trading-Engine/blob/main/SampleOutput/Sample.png)  
*Live matching demo via terminal input*

---

## 📚 Use Case

- 🔍 Learn the internal logic of trading platforms like Zerodha, AngelOne, and IB
- 🎓 Use in final year/portfolio projects for job interviews
- ⚙️ Backend core for your Algo Trading bots
- 🧪 Simulate order flow for backtesting

---

## 🏁 Getting Started

### Windows (Visual Studio)
- Open `.sln` or `.vcxproj` file
- Build project (Debug/Release)
- Run `main.cpp`

### Linux/macOS
```bash
g++ src/*.cpp main.cpp -o trading_engine
./trading_engine
```

---
##📂 Folder Structure
```
High_Performance_Trading_Engine/
├── include/
│   ├── order.h
│   ├── order_book.h
│   └── engine.h
├── src/
│   ├── order.cpp
│   ├── order_book.cpp
│   └── engine.cpp
├── main.cpp
├── screenshots/
│   └── cli_demo.png
├── README.md
└── db_config.sql (optional)
```
---

## 🔧 How It Works
🧾 Accepts user input via CLI:
   - Supported commands: BUY, SELL, MODIFY, CANCEL

📚 Maintains separate order books:
   - Uses std::map to keep prices sorted
   - Uses std::queue to preserve FIFO (time priority) within each price level

🤝 Matches incoming orders:
   - Matches BUY with best available SELL (and vice versa)
   - Follows price-time priority to ensure fairness

💰 Tracks:
   - Holdings (net quantity for each side)
   - Realized and Unrealized Profit & Loss (PnL)

🖥️ Outputs:
   - Live status of the order book
   - Executed trades and updated PnL

## ❓ FAQ

**Q1: Is it connected to live markets?**  
No. This project simulates a local matching engine. However, it can be extended to connect with APIs like Angel One SmartAPI or Zerodha Kite.

---

**Q2: How is FIFO maintained?**  
Orders are stored in a `std::queue` at each price level inside a `std::map`, ensuring that the earliest orders (by time) are matched first at the same price — thus preserving Price-Time Priority (FIFO).

---

**Q3: Is PnL real-time?**  
Yes.  
- Realized PnL is calculated and updated with every successful trade.  
- Unrealized PnL is estimated based on the current best market price in the opposite order book.

---

**Q4: Can I build a bot on top of this?**  
Absolutely.  
This engine is the core of a matching system. You can integrate it with:
- Automated trading logic (bots/algos)
- Live API feeds (e.g., Angel One)
- GUI or web dashboards
- Historical data for backtesting

---


