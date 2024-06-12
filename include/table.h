#ifndef TABLE
#define TABLE

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>

/*
 * Redesign
 *
 * Datetime objects? boost::date, std::chrono::time_point. Can just leave it as a string?
 * unordered_map of (date-indexed type, std::chrono?) pointers for rows (each row being a vector), index vector by column number.
 * Shift to unordered_map for O(logn)->O(1) operations
 * Can extend values to be templated but not relevant for our use case
 * Data: 2d vector of floats
 * Row access: unordered_map <chrono, *std::vector<float>>
 * Column access: unordered_map <std::string, int>
 *
 * Instance variables:
 * - cur date index (int): pseudo-iterator
 *
 * To-Do:
 * - Support yfinance Curl requests if csv not detected (and save downloaded data to csv)
 * - Get all valid market dates? May not be necessary if we can just parse the dates from yfinance api or input csv
 */

// https://stackoverflow.com/questions/24853914/read-2d-array-in-csv-into-a-map-c

#define in(val,x) (x.find(val) != x.end())

namespace datatable
{
    //typedef std::map<std::string, std::map<std::string, float>> datatable; // to remove
    const static char *DELIMS = "\t ,";
    const int MAX_LINE_LENGTH = 1024;

    typedef struct dataSlice {
        std::vector<float> values;
        std::string label;
    } dataSlice;

    template<typename rowType = std::string, typename colType = std::string>
    class Table {
        typedef std::vector<rowType> row;
        typedef std::vector<colType> col;
        typedef std::vector<std::vector<float>> vvf; // data
    public:
        explicit Table(std::string path); // open file and read from path
        //Table(std::string data);
        Table();

        // int &Table::operator[];
        void printData();

        std::vector<std::string> getDates() { return rowLabels; }

        std::vector<std::string> getTickers() { return colLabels; }

        void setOutputFileName(std::string newName) {outputFileName = newName;}
        std::string getOutputFileName() const {return outputFileName;}

        vvf &getData() { return data; }

        // write getRow and getColumn methods, also fetch specific cell
        dataSlice *getRow(rowType row);

        dataSlice *getCol(colType col);

        float getCell(rowType row, colType col);

        void addRow();

        void addCol();

    protected:
    private:
        vvf data;
        std::unordered_map<rowType, row *> rowLabels;
        std::unordered_map<colType, int> colLabels;
        std::string outputFileName;
    };

}// namespace
#endif