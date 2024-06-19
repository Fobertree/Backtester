# Backtester

Intend to create Python bindings with pybind11

```
pip install "pybind11[global]"
```

Using libcurl to support YFinance API. May replace with another library. Otherwise, can just manually throw in a .csv to pull prices from or have it passed via python wrapper and Pandas DataFrame

## To-Do

- Pull data from csv
  - YF api calls optional (low priority)
  - String response $\rightarrow$ JSON $\rightarrow$ Table (in_sort)
  - Options to pull data into: DataTable, DataFrame, map (with iterator, prob vector with index .find for helper vector of tickers)
- Python bindings (pybind11)
  - Python $\rightarrow$ C++
    - Expose Backtest constructor
    - Expose run_backtest function
  - C++ $\rightarrow$ Python
    - unordered_map to dict
    - vector to list
- Thread pool concurrency
- Rewrite Table

REWRITE run_backtest() function

Modify Cmake to compile dll so we can interface with python

## Run CMake to Generate DLL

`cmake -B build . -G "Visual Studio 17 2022"`

[Build C++ Extension for Python in Visual Studio](https://learn.microsoft.com/en-us/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022)

Other IDEs, run `cmake .` and
`make .`

https://dev.to/uponthesky/c-making-a-simple-json-parser-from-scratch-250g