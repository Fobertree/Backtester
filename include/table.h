//
// Created by Alexa on 7/4/2024.
//

#ifndef CPP_BACKTESTER_TABLE_H
#define CPP_BACKTESTER_TABLE_H

#include <string>
#include <vector>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <stdexcept>
#include <unordered_map>
#include "readcsv.h"

// std insert
/*
 * Features
 * - load from csv
 * - update data
 * - print data
 * LT
 * - Add python bindings for fun?
 */

// TODO: add unordered_map utility

template<typename R = std::string,typename C = std::string, typename D = float>
class Table
{
public:
    // constructors
    explicit Table(const std::string& csv_path)
    {
        auto csvData = CSVReader<std::string,std::string,float> (csv_path);
        rowLabels = csvData.getRowLabels();
        colLabels = csvData.getColLabels();
        data = csvData.getData();
        //std::tie(rowLabels,colLabels,data) = csvData.getAll();
    }
    Table()
    {
        Table("./data.csv");
    };
    // destructor
    // ~Table();
    // insertion
    // Overload for performance enhancement on strings
    // push back
    void append_row(const std::vector<D>& row, const std::string& rowLabel)
    {
        int tmpLen;
        try
        {
            if ((tmpLen = row.size()) != colLen)
                throw std::invalid_argument("Incorrect length of column");
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << ": " << "Num Rows: " << rowLen << "\nLength of column: " << tmpLen << std::endl;
        }
        data.insert(data.end(),row.begin(),row.end());
        rowLabels.insert(rowLabels.back(),rowLabel);
    }
    void append_col(const std::vector<D>& col, const std::string& colLabel)
    {
        int tmpLen;
        try
        {
            if ((tmpLen = col.size()) != rowLen)
                throw std::invalid_argument("Incorrect length of column");
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << ": " << "Num Rows: " << rowLen << "\nLength of column: " << tmpLen << std::endl;
        }
        data.insert(data.end(),col.begin(),col.end());
        colLabels.insert(rowLabels.back(),colLabel);
    }
    // in order
    void insert_row(const std::vector<D>& row, const std::string& rowLabel)
    {
        int tmpLen;
        try
        {
            if ((tmpLen = row.size()) != colLen)
                throw std::invalid_argument("Incorrect length of column");
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << ": " << "Num Rows: " << rowLen << "\nLength of column: " << tmpLen << std::endl;
        }
        int offset = bisect<std::string>(rowLabel,rowLabels);
        data.insert(data.begin() + offset,row.begin(),row.end());
        rowLabels.insert(rowLabels.begin() + offset, row.begin(),row.end());
    }
    void insert_col(const std::vector<D>& col, const std::string& colLabel)
    {
        int tmpLen;
        try
        {
            if ((tmpLen = col.size()) != colLen)
                throw std::invalid_argument("Incorrect length of column");
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << ": " << "Num Rows: " << rowLen << "\nLength of column: " << tmpLen << std::endl;
        }
        int offset = bisect<std::string>(colLabel,rowLabels);
        data.insert(data.begin() + offset,col.begin(),col.end());
        rowLabels.insert(rowLabels.begin() + offset, col.begin(),col.end());
    }
    // Generic
    // push back
    void append_row(const std::vector<D>& row, R rowLabel);
    void append_col(const std::vector<D>& col, C colLabel);
    // in order
    void insert_row(const std::vector<D>& row, R rowLabel);
    void insert_col(const std::vector<D>& col, C colLabel);
    // getters
    std::vector<int> size()
    {
        return {rowLen,colLen,rowLen * colLen};
    };
    void printTable(); //includes labels

    D operator ()(R row, C col)
    {
        if (!(mapRowLabels.contains(row)) || !(mapColLabels.contains(col)))
            throw std::out_of_range("Cannot find row and column.");

        vector<D> tmpRow = *mapRowLabels[row];
        return tmpRow[mapColLabels[col]];
    }

    const std::vector<R>& getRow(R row)
    {
        if (!(mapRowLabels.contains(row)))
            throw std::out_of_range("Cannot find row.");

        return mapRowLabels[row];
    }

    const std::vector<C>& getCol(C col)
    {
        if (!(mapColLabels.contains(col)))
            throw std::out_of_range("Cannot find col.");

        std::vector<D> colData;
        int colIdx{mapColLabels[col]};

        for (const std::vector<D>& row : data)
        {
            colData.push_back(row[colIdx]);
        }

        return colData;
    }

    const std::vector<R>& getRowLabels()
    {
        return rowLabels;
    }

    const std::vector<C>& getColLabels()
    {
        return colLabels;
    }

    R getRowLabelByIdx(int i)
    {
        return rowLabels[i];
    }

    C getColLabelByIdx(int i)
    {
        return colLabels[i];
    }
private:
    std::vector<std::vector<D>> data;
    int rowLen{0};
    int colLen{0};
    // Labels
    std::vector<R> rowLabels;
    std::vector<C> colLabels;
    // Label Addresses for access
    std::unordered_map<R,std::vector<D>*> mapRowLabels;
    std::unordered_map<C, int> mapColLabels;
    // compare + bisect
    bool static defaultComparator(R compareItem, R target)
    {
        try {
            if (std::is_same<R, std::string>::value || std::is_same<R, int>::value) {
                return compareItem < target;
            } else {
                throw std::invalid_argument("R Invalid in default comparator");
            }
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << ": " << typeid(R).name() << std::endl;
        }

        return false;
    };
    template <typename vec_type>
    int bisect(R target, const std::vector<R>& vec, bool (*comp)() = defaultComparator)
    {
        // equivalent bisect_left
        int i{0};
        int len{vec.size()};

        for (int leap = len / 2; leap > 0; leap /= 2)
        {
            if (vec.at(i+leap) == target)
                return i;

            // comparator must return true if we leap, false otherwise.
            if (comp(vec.at(i+leap),target))
                i += leap;
        }
        return i;
    }

}; // Table class

#endif //CPP_BACKTESTER_TABLE_H
