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
#include <unordered_map>

typedef str std::string;
typedef order std::vector<str, str, float>; // ticker, order_type, float

class Backtester
{
public:
    Backtester();
    Backtester(float cash);

    void run_daily_instructions(); // swtich case
    // void run_instruction(order order);
    float getCash();
    int getTimestep();
    std::vector<float> getPortfolioReturns();
    float getPortfolioValue();
    unordered_map<std::string, int> getHoldings();
    void buyStock();
    void sellStock();
    float getCurrentPortfolioValue();

protected:
private:
    std::vector<order>
        instructions;
    std::vector<float> portfolio_values; // value of portfolio
    int timestep;
    template <T, U>
    unordered_map<T, U> tracker; // tbd exact structure
    float cash;
    unordered_map<std::string, int>
}