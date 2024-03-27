# Backtester

Intend to create Python bindings with pybind11

```
pip install "pybind11[global]"
```

Using libcurl to support YFinance API. May replace with another library. Otherwise, can just manually throw in a .csv to pull prices from or have it passed via python wrapper and Pandas DataFrame

## To-Do

- Pull data from csv
  - YF api calls optional (low priority)
  - Options to pull data into: DataTable, DataFrame, map (with iterator, prob vector with index .find for helper vector of tickers)
- Python bindings (pybind11)
  - Python $\rightarrow$ C++
    - Expose Backtest constructor
    - Expose run_backtest function
  - C++ $\rightarrow$ Python
    - unordered_map to dict
    - vector to list
