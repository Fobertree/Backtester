/*
At each timestep, we want to allocate threads to process each order at the specific timestep (may need to care about race conditions in edge case where 1 order fails and other doesn't?)
- Orders processed at same timestep will not interfere with each other, barring some edge cases (etc. buy and sell same stock at same timestep)

Use Pybind11. Boost.Python has too many installation requirements and doesn't seem very portable
*/

#include <thread>
#include <iostream>
#include "backtester.h"
constexpr auto MAX_THREADS = 4;
constexpr auto PATH = "data.csv";

Table Backtester::dataTable = Table(PATH);
datatable Backtester::stockData = dataTable.getData();
std::vector<std::string> Backtester::dates = dataTable.getDates();

Backtester::Backtester()
{
    cash = 100000;
    startDate = "";
    endDate = "";
    offset = 0;
}

Backtester::Backtester(float cash, std::string startDate, std::string endDate, std::vector<std::vector<order>> instructions, std::string path)
{
    this->cash = cash;
    this->startDate = startDate;
    this->endDate = endDate;
    this->instructions = instructions;
    offset = 0;

    Backtester::dataTable = Table("data.csv");
    Backtester::stockData = dataTable.getData();
    Backtester::dates = dataTable.getDates();
}

Backtester::Backtester(float cash, std::string startDate, std::string endDate, std::vector<std::vector<order>> instructions)
{
    this->cash = cash;
    this->startDate = startDate;
    this->endDate = endDate;
    this->instructions = instructions;
    offset = 0;
}

void Backtester::evalOrder(order &order)
{
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
    //tqdm::tqdm(instructions.begin(), instructions.end())
    for (auto &orderTimestep : instructions)
    {
        for (auto &order : orderTimestep)
        {
            // look into thread pool

            evalOrder(order);
        }

        portfolio_values.push_back(getPortfolioValue());
        timestep += 1;
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

    float cost;
    cost = fetchStockPrice(ticker, quantity, timestep);

    if (holdings[ticker] == quantity)
    {
        holdings.erase(ticker);
    }

    else
    {
        holdings[ticker] = holdings[ticker] - quantity;
    }

    cash += cost;
}

void Backtester::setStartIndex(std::string startDate)
{
    dates = dataTable.getDates();
    for (int i = 0; i < dates.size(); i++)
    {
        if (startDate == dates[i])
        {
            offset = i;
        }
    }
    std::cerr << "Failed to locate valid startDate in data: " << startDate << std::endl;
}

float Backtester::fetchStockPrice(std::string ticker, int quantity, int timestep)
{
    // rewrite or overload to be a lot more dynamic and flexible beyond pseudo-indexed map
    // TODO: Support and make flexible Yfinance curl requests
    std::string date = dates[offset + timestep];
    return stockData[ticker][date];
}