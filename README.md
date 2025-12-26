# Console Crypto Trading Simulator (C, Windows)

A simple **console-based crypto trading simulator** written in **C (Windows)**.  
Coin prices change randomly over time, and users can **buy/sell coins** while tracking **cash, total assets, and portfolio performance**.

## Features
- 5 custom coins: `SDJ`, `LSO`, `PDH`, `ADH`, `JIH`
- Random price movement per refresh (range: **-100 ~ +100**)
- Real-time portfolio display:
  - Total assets (cash + holdings)
  - Remaining cash
  - Coin price table with:
    - current price
    - day-over-day % change
    - day-over-day price change
- Buy / Sell system
- Average buy price tracking (`totalCost / quantity`)
- Profit & profit rate calculation (per holding)
- Colorized output:
  - **Red** for 상승(+)
  - **Blue** for 하락(-)
  - **Yellow** for portfolio lines
- Delisting rule:
  - If a coin price becomes near 0 (`<= 0.000001`), the coin is removed from the list

## How it Works
1. User inputs **initial assets**
2. Program repeatedly:
   - clears screen
   - updates each coin price (random change)
   - prints tables & portfolio status
   - asks user to choose:
     - **1) Buy**
     - **2) Sell**
     - **3) Exit**
3. After each action, the program waits **1 second** (`Sleep(1000)`)

## Build (Windows)

This project uses:
- `windows.h` (for `Sleep()` and console colors)
- `system("cls")` for screen clearing

### Option 1) Visual Studio (Recommended)
1. Create a **Console App (C)** project
2. Paste the code into `main.c`
3. Build & Run

### Option 2) MinGW (GCC) on Windows
```bash
gcc main.c -o trading_sim.exe
./trading_sim.exe
