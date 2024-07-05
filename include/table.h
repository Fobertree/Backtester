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

// replace macro with contains since C++20
#define in(val,x) (x.find(val) != x.end())

namespace DataTable
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
        explicit Table(const std::string& path); // open file and read from path
        //Table(std::string data);
        Table();

        // int &Table::operator[];
        void printData();

        std::vector<std::string>& getDates() { return rowVals; }

        std::vector<std::string> & getTickers() { return colVals; }

        void setOutputFileName(std::string newName) {outputFileName = newName;}
        std::string getOutputFileName() const {return outputFileName;}

        // get value from array
        float getValue(int rIdx,int cIdx);
        float getValue(rowType rName, colType cName);
        float getValue(int rIdx, colType cName);
        float getValue(rowType rName, int cIdx);

        // write getRow and getColumn methods, also fetch specific cell
        dataSlice *getRow(rowType row);

        dataSlice *getCol(colType col);

        float getCell(rowType row, colType col);

        void addRow();

        void addCol();

        // add one data value to table
        void insert_data(rowType rowName, colType colName, float val);
        // can add custom comparator to generalize. Below methods are key insort
        void insert_row(rowType rowName, const std::vector<float>& rowValues);
        void insert_col(colType colName,std::vector<float> colValues);

    protected:
    private:
        // access data via labels in unordered_map
        // O(1) access and insertion. weaknesses: linked list insertion times, memory, hash collisions.
        vvf data;
        // row labels, ordered
        std::vector<rowType> rowVals;
        std::vector<colType> colVals;
        // store pointers/indices
        std::unordered_map<rowType, row *> rowLabels;
        std::unordered_map<colType, int> colLabels; // do we need this to be ordered
        std::string outputFileName;
        int getRowIdx(rowType rowLabel, bool closest = false);
        template<typename itemType>
        std::vector<itemType> copyVector(std::vector<itemType> & otherVec);

        // change to size_t?
        int numRows{0};
        int numCols{0};
    };

    template<typename rowType, typename colType>
    void Table<rowType, colType>::printData() {
//        for (auto &columnMap : data)
//        {
//            for (auto cell : columnMap.second)
//            {
//                std::cout << columnMap.first /*First label*/ << " "
//                          << cell.first /*Second label*/ << " "
//                          << cell.second /*Value*/ << std::endl;
//            }
//        }
    }

    template<typename rowType, typename colType>
    template<typename itemType>
    std::vector<itemType> Table<rowType, colType>::copyVector(std::vector<itemType> &otherVec)
    {
        // slower than swapping
        std::vector<itemType> newVec;
        newVec.reserve(otherVec.size());
        std::copy(newVec,otherVec.begin(),otherVec.end());

        return std::vector<itemType>();
    }

}// namespace
#endif