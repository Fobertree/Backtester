/*
At each timestep, we want to allocate threads to process each order at the specific timestep (may need to care about race conditions in edge case where 1 order fails and other doesn't?)
- Orders processed at same timestep will not interfere with each other, barring some edge cases (etc. buy and sell same stock at same timestep)

Use Pybind11. Boost.Python has too many installation requirements and doesn't seem very portable
*/

#include <thread>
#include <iostream>
#include "backtester.h"
#include <table.h>
constexpr auto MAX_THREADS = 4;
constexpr auto PATH = "data.csv";

Backtester::Backtester()
{
    cash = 100000;
    startDate = "";
    endDate = "";
    offset = 0;
}

Backtester::Backtester(float cash, const std::string& startDate, const std::string& endDate, const std::vector<std::vector<order>>& instructions, const std::string& path=PATH)
{
    this->cash = cash;
    this->startDate = startDate;
    this->endDate = endDate;
    this->instructions = instructions;
    offset = 0;

    table = Table<>(path);

}

Backtester::Backtester(float cash, const std::string& startDate, const std::string& endDate, const std::vector<std::vector<order>>& instructions)
{
    this->cash = cash;
    this->startDate = startDate;
    this->endDate = endDate;
    this->instructions = instructions;
    offset = 0;

    table = Table<>();
}

void Backtester::evalOrder(order &order)
{
    //process order
    std::string order_type, ticker;
    int quantity;

    std::tie(order_type, ticker, quantity) = order;

    if (order_type == "BUY")
    {
        buyStock(ticker, quantity);
    }
    else if (order_type == "SELL")
    {
        sellStock(ticker, quantity);
    }
    else
    {
        // printf("Order Type: %s, Ticker: %s, Quantity: %d FAILED", order_type, ticker, quantity);
        std::cerr << "Order Type: " << order_type << ", Ticker: " << ticker << ", Quantity: " << quantity << " FAILED!" << std::endl;
    }
}

void Backtester::run_backtest()
{
    // order is a size-3 tuple declared in header
    //#pragma unroll
    // threads for each order at a specific timestep.
    // mutex on cash or localize variable
    for (auto &orderTimestep : instructions)
    {
        // orderTimestep (vector<order>): all orders to be purchased at specific timestep
        for (auto &order : orderTimestep)
        {
            // order: ticker, order_type, float
            // look into thread pool

            // tiny redundancy in evalOrder where we fetch price in evalOrder and fetch again in getPortFolioValue()
            evalOrder(order);
        }

        portfolio_values.emplace_back(getPortfolioValue()); //AUM
        timestep++;
    }
}

float Backtester::getPortfolioValue()
{
    float equityValue{ 0 };

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

void Backtester::buyStock(const std::string& ticker, int quantity)
{
    // check if we have the funds
    float cost;
    cost = fetchStockPrice(ticker, quantity, timestep);

    if (cost < cash)
    {
        std::cout << "Failure to purchase " << quantity << "shares of stock: " << ticker << " with $" << cash << "." << std::endl;
    }

    cash -= cost;
    holdings[ticker] += quantity;
}

void Backtester::sellStock(const std::string& ticker, int quantity)
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

    float cost;
    cost = fetchStockPrice(ticker, quantity, timestep);

    if (holdings[ticker] == quantity)
    {
        holdings.erase(ticker);
    }

    else
    {
        holdings[ticker] -= quantity;
    }

    cash += cost;
}

void Backtester::setStartIndex(const std::string& stDate)
{
    // optimizable via strcmp and binary search
    dates = table.getRowLabels();
    for (int i = 0; i < dates.size(); i++)
    {
        if (stDate == dates[i])
        {
            offset = i;
        }
    }
    std::cerr << "Failed to locate valid startDate in data: " << startDate << std::endl;
}

float Backtester::fetchStockPrice(const std::string& ticker, int quantity, int ts)
{
    // rewrite or overload to be a lot more dynamic and flexible beyond pseudo-indexed map
    // TODO: Support and make flexible Yfinance curl requests
    // TODO: Binary search for keys for lookup
    // Maybe trade off space with extra unordered_map of ticker, colIdx
    std::string date = table.getRowLabelByIdx(ts);

    return table(date,ticker);
}