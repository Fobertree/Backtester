//
// Created by Alexa on 7/4/2024.
//

#ifndef CPP_BACKTESTER_TABLE2_H
#define CPP_BACKTESTER_TABLE2_H

#include <string>
#include <vector>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <stdexcept>

// std insert
/*
 * Features
 * - load from csv
 * - update data
 * - print data
 * LT
 * - Add python bindings for fun?
 */

template<typename RTYPE = std::string,typename CTYPE = std::string, typename DTYPE = float>
class Table
{
public:
    // constructors
    Table(std::string csv_path);
    Table();
    // destructor
    ~Table();
    // insertion
    // Overload for performance enhancement on strings
    // push back
    void append_row(const std::vector<DTYPE>& row, const std::string& rowLabel)
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
    void append_col(const std::vector<DTYPE>& col, const std::string& colLabel)
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
    void insert_row(const std::vector<DTYPE>& row, const std::string& rowLabel)
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
    void insert_col(const std::vector<DTYPE>& col, const std::string& colLabel)
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
    void append_row(const std::vector<DTYPE>& row, RTYPE rowLabel);
    void append_col(const std::vector<DTYPE>& col, CTYPE colLabel);
    // in order
    void insert_row(const std::vector<DTYPE>& row, RTYPE rowLabel);
    void insert_col(const std::vector<DTYPE>& col, CTYPE colLabel);
    // getters
    void size();
    void printTable(); //includes labels
private:
    std::vector<std::vector<DTYPE>> data;
    int rowLen{0};
    int colLen{0};
    // Labels
    std::vector<RTYPE> rowLabels;
    std::vector<CTYPE> colLabels;
    // compare + bisect
    bool static defaultComparator(RTYPE compareItem, RTYPE target)
    {
        try {
            if (std::is_same<RTYPE, std::string>::value || std::is_same<RTYPE, int>::value) {
                return compareItem < target;
            } else {
                throw std::invalid_argument("RTYPE Invalid in default comparator");
            }
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << ": " << typeid(RTYPE).name() << std::endl;
        }

        return false;
    };
    template <typename vec_type>
    int bisect(RTYPE target, const std::vector<RTYPE>& vec, bool (*comp)() = defaultComparator)
    {
        // equivalent bisect_left
        int i{0};
        size_t len{vec.size()};

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

#endif //CPP_BACKTESTER_TABLE2_H
