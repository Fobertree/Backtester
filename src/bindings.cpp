/*
Write Python bindings to create a Python interface for our C++ backtest
*/

#include <pybind11.h>
#include "backtester.h"
// https://pybind11.readthedocs.io/en/stable/classes.html
// https://ep2017.europython.eu/media/conference/slides/pybind11-seamless-operability-between-c11-and-python.pdf

namespace py = pybind11;

// Backtester(float cash, int startDate, int endDate, std::vector<order> instructions)
py::class_<Backtester>(m, "Backtester")
    .def(py::init<const int>(), py::init<const int>(), py::init<std::vector>())
    .def("runBacktest", &Backtester::run_backtest)
    .def("__repr__",
         [](const Backtester &b)
         {
             return "<Backtester> with $" + b.cash;
         })
    .def("getPortfolioReturns", &Backtester::getPortfolioReturns());
//.def_property("tracke")