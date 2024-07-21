/*
Functionalities:
- Take in input of orders from Python
- Take in following instructions: BUY,SELL,HOLD
- Daytracker for maximum holding period: unordered_map of min-heap by tuple (date, shares). Can create cutom class object
    - When sell, heappush from lowest date then heappush with lowered shares if shares sold < shares in tuple
- Sell-stop: can implement custom tree structure under unordered_map with daytracker

We need plotting functionality as well

Metrics
- Sharpe
- Returns
- Avg. % of liquid cash?

*/
#ifndef BACKTESTER
#define BACKTESTER

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <queue>
#include "table.h"

typedef std::tuple<std::string, std::string, float> order; // ticker, order_type, float
typedef std::vector<std::vector<float>> vvf;

typedef struct settings
{
    float commission, flatFee;
} settings;

class Backtester
{
public:
    Backtester();
    Backtester(float cash, const std::string& startDate, const std::string& endDate, const std::vector<std::vector<order>>& instructions, const std::string& path);
    Backtester(float cash, const std::string& startDate, const std::string& endDate, const std::vector<std::vector<order>>& instructions);

    void run_backtest(); // switch case
    // void run_instruction(order order);
    //float getCash() { return cash; }
    [[nodiscard]] int getTimestep() const { return timestep; }
    // std::vector<float> getPortfolioReturns() { return portfolio_values; } // return historical values
    float getPortfolioValue(); // current value
    const std::unordered_map<std::string, int>& getHoldings();
    void buyStock(const std::string& ticker, int quantity);
    void sellStock(const std::string& ticker, int quantity);
    void setStartIndex(const std::string& startDate);
    void evalOrder(order &order);
    float fetchStockPrice(const std::string& ticker, int quantity, int ts);
    const std::string& getStartDate() const { return startDate; }
    const std::string& getEndDate() const { return endDate; }
    // const getters for __repr__
    [[nodiscard]] float getCash() const { return cash; }
    const std::vector<float>& getPortfolioReturns() const { return portfolio_values; }
    // config
    void addConfig();
    void removeConfig();

protected:
private:
    std::vector<std::vector<order>> instructions;
    std::vector<float> portfolio_values; // value of portfolio
    int timestep{0};
    float cash;
    std::unordered_map<std::string, int> holdings;
    // need to figure out how to best handle time objects. Probably std::chrono, but need functionality to find next valid market date
    std::string startDate; // yyyymmdd?
    std::string endDate;
    // Indexing our table/dataframe object
    int offset; // start index for row
    vvf stockData; // holdings
    std::vector<std::string> dates;
    settings config{};
    Table<> table;
};

#endif