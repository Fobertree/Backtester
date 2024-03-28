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
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <queue>
#include <tqdm/tqdm.h>
#include <utils.h>
#include "table.h";

typedef std::tuple<std::string, std::string, float> order; // ticker, order_type, float
typedef std::map<std::string, std::map<std::string, float>> datatable;

struct settings
{
    float commmsion, flatFee;
};

class Backtester
{
public:
    Backtester();
    Backtester(float cash, std::string startDate, std::string endDate, std::vector<std::vector<order>> instructions, std::string path);
    Backtester(float cash, std::string startDate, std::string endDate, std::vector<std::vector<order>> instructions);

    void run_backtest(); // switch case
    // void run_instruction(order order);
    float getCash() { return cash; }
    int getTimestep() { return timestep; }
    std::vector<float> getPortfolioReturns() { return portfolio_values; } // return historical values
    float getPortfolioValue();                                            // current value
    std::unordered_map<std::string, int> getHoldings();
    void buyStock(std::string ticker, int quantity);
    void sellStock(std::string ticker, int quantity);
    float setStartIndex(std::string startDate);
    void evalOrder(order &order);
    float fetchStockPrice(std::string ticker, int quantity, int timestep);

protected:
private:
    std::vector<std::vector<order>> instructions;
    std::vector<float> portfolio_values; // value of portfolio
    int timestep{0};
    // template <T, U>
    // unordered_map<T, U> tracker; // tbd exact structure
    float cash;
    unordered_map<std::string, int> holdings;
    // need to figure out how to best handle time objects
    std::string startDate; // yyyymmdd?
    std::string endDate;
    // Indexing our table/dataframe object
    int startIndex;
    std::mutex _m;
    // template <T>
    // std::queue<T> _q;
    int offset;

    static Table dataTable;
    static datatable stockData;
    static std::vector<std::string> dates;
};