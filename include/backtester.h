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
typedef std::unordered_map<std::string, std::map<std::string, float>> datatable;
typedef std::vector<std::vector<float>> vvf;

typedef struct settings
{
    float commission, flatFee;
} settings;

class Backtester
{
public:
    Backtester();
    Backtester(float cash, std::string startDate, std::string endDate, std::vector<std::vector<order>> instructions, std::string path);
    Backtester(float cash, std::string startDate, std::string endDate, std::vector<std::vector<order>> instructions);

    void run_backtest(); // switch case
    // void run_instruction(order order);
    //float getCash() { return cash; }
    int getTimestep() const { return timestep; }
    // std::vector<float> getPortfolioReturns() { return portfolio_values; } // return historical values
    float getPortfolioValue(); // current value
    std::unordered_map<std::string, int> getHoldings();
    void buyStock(const std::string& ticker, int quantity);
    void sellStock(std::string ticker, int quantity);
    void setStartIndex(std::string startDate);
    void evalOrder(order &order);
    float fetchStockPrice(const std::string& ticker, int quantity, int timestep);
    std::string getStartDate() const { return startDate; }
    std::string getEndDate() const { return endDate; }
    // const getters for __repr__
    float getCash() const { return cash; }
    std::vector<float> getPortfolioReturns() const { return portfolio_values; }
    // config
    void addConfig();
    void removeConfig();

protected:
private:
    std::vector<std::vector<order>> instructions;
    std::vector<float> portfolio_values; // value of portfolio
    int timestep{0};
    // template <T, U>
    // unordered_map<T, U> tracker; // tbd exact structure
    float cash;
    std::unordered_map<std::string, int> holdings;
    // need to figure out how to best handle time objects. Probably std::chrono, but need functionality to find next valid market date
    std::string startDate; // yyyymmdd?
    std::string endDate;
    // Indexing our table/dataframe object
    int offset; // start index for row

    static DataTable::Table <> dataTable; // stock price data
    static vvf stockData; // holdings
    static std::vector<std::string> dates;
    settings config{};
};

#endif