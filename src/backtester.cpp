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
    startDate = 0;
    endDate = 0;
    instructions = std::vector<order>;
}

Backtester::Backtester(float cash, int startDate, int endDate, std::vector<order> instructions)
{
    this->cash = cash;
    this->startDate = startDate;
    this->endDate = endDate;
    this->instructions = instructions;
}

void Backtester::run_backtest()
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

        portfolio_values.push_back(getPortfolioValue());
        timestep += 1;
    }
}

float Backtester::getPortfolioValue()
{
    float equityValue;

    for (auto &holding : holdings)
    {
        equityValue += fetchStockPrice(holding.first, holding.second, timestep);
    }
    return cash + equityValue;
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
    cost = fetchStockPrice(ticker, quantity, timestep);

    if (cost < cash)
    {
        std::cout << "Failure to purchase " << quantity << "shares of stock: " << ticker << " with $" << cash << "." << std::endl;
    }

    cash -= cost;
    holdings.find(ticker) != holdings.end() ? holdings[ticker] = holdings[ticker] + quantity : holdings[ticker];
}

void Backtester::sellStock(std::string ticker, int quantity)
{
    if (holdings.find(ticker) != holdings.end())
    {
        std::cout << "Ticker" << ticker << " not found in sellStock order";
        return;
    }

    // could use spaceship operator + case/switch to make code look cleaner, C++20

    if (holdings[ticker] < quantity)
    {
        std::cout << "Insufficient shares in " << ticker << "Requested to sell: " << quantity << "shares. Only have " << holdings[ticker] << " shares." << std::endl;
        return;
    }

    else if (holdings == quantity)
    {
        holdings.erase(ticker);
    }

    else
    {
        holdings[ticker] = holdings[ticker] - quantity;
    }

    cash += cost;
}

float fetchStockPrice(std::string ticker, int quantity, int timestep)
{
    // TODO
}