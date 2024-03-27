/*
At each timestep, we want to allocate threads to process each order at a specific timestep
- Orders processed at same timestep will not interfere with each other, barring some edge cases (etc. buy and sell same stock at same timestep)

Use Pybind11. Boost.Python has too many installation requirements and doesn't seem very portable
*/

#include <thread>
#include <iostream>
#include "backtester.h"

Backtester::Backtester()
{
    cash = 100000;
}

Backtester::Backtester(float cash)
{
    this->cash = cash;
}

void Backtester::run_daily_instructions()
{
    // order is a size-3 tuple declared in header
    for (auto &order : tqdm::tqdm(instructions))
    {
        std::string order_type, ticker;
        int quantity;

        std::tie(order_type, ticker, quantity) = order;

        switch (order_type)
        {
        case "BUY":
            buyStock(ticker, quantity);
            break;
        case "SELL":
            sellStock(ticker, quantity);
            break;
        case "HOLD":
            break;

        default:
            // printf("Order Type: %s, Ticker: %s, Quantity: %d FAILED", order_type, ticker, quantity);
            std::cerr << "Order Type: " << order_type << ", Ticker: " << ticker << ", Quantity: " << quantity << " FAILED!" << std::endl;
        }
    }
}

float Backtester::getPortfolioValue()
{
    float illiquidValue;

    for 
}

std::unordered_map<std::string, int> Backtester::getHoldings()
{
    // add print functionality
    return holdings;
}

void Backtester::buyStock(std::string ticker, int quantity)
{
    // check if we have the funds
    float cost;
    cost =
        if (cash >=)
}

void Backtester::sellStock(std::string ticker, int quantity)
{
}