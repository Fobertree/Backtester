/*
Write Python bindings to create a Python interface for our C++ backtest

For Backtester
-

*/

//#include <pybind11.h>
#include "../extern/pybind11/include/pybind11/pybind11.h"
#include "backtester.h"
// https://pybind11.readthedocs.io/en/stable/classes.html
// https://ep2017.europython.eu/media/conference/slides/pybind11-seamless-operability-between-c11-and-python.pdf

namespace py = pybind11;

// Backtester(float cash, int startDate, int endDate, std::vector<order> instructions)
PYBIND11_MODULE(backtester_py,m){
py::class_<Backtester>(m, "Backtester")
    .def(py::init<float,std::string,std::string,std::vector<std::vector<order>>,std::string>())
    .def("runBacktest", &Backtester::run_backtest)
    .def("__repr__",
         [](const Backtester &b)
         {
            return "<Backtester> with $" + std::to_string(b.getCash()) + "Start-Date: " + b.getStartDate();
         })
    .def("getPortfolioReturns", &Backtester::getPortfolioReturns);
//.def_property("tracke")
}