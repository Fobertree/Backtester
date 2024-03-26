/*
At each timestep, we want to allocate threads to process each order at a specific timestep
- Orders processed at same timestep will not interfere with each other, barring some edge cases (etc. buy and sell same stock at same timestep)

Use Pybind11. Boost.Python has too many installation requirements and doesn't seem very portable
*/

#include <thread>