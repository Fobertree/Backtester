//
// Created by Alexa on 7/5/2024.
//

#ifndef CPP_BACKTESTER_READCSV_H
#define CPP_BACKTESTER_READCSV_H

#include <tuple>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace CSVReader
{
    using std::vector;
    template<typename RTYPE, typename CTYPE, typename DTYPE>
    [[nodiscard]] const std::tuple<vector<RTYPE>,vector<CTYPE>,vector<DTYPE>>& CSVToTable(const std::string& path)
    {
        static std::tuple<vector<RTYPE>,vector<CTYPE>,vector<DTYPE>> res;
        vector<RTYPE> rowLabels;
        vector<CTYPE> colLabels;
        vector<vector<DTYPE>> data;

        try
        {
            // Relative path
            std::ifstream file("./data/" + path);

            if (!file)
            {
                std::cout << "Error, could not open file";
                throw -1;
            }

            // row: ticker. col: date
            // is interchanged row/col more efficient?
            std::string row, col;
            int rowIdx, colIdx;
            std::string line;

            if (!file.eof())
            {
                // handle column names from csv
                std::getline(file,line);
                std::istringstream lineStream(line);
                std::string token;
                colIdx = 0;

                while (std::getline(lineStream,token,','))
                {
                    // CPP alternative to strtok, but with one delimiter

                    colLabels[colIdx++] = token;
                }
            }

            int num;
            while (!file.eof())
            {
                // get row from csv
                std::getline(file,line);

                std::istringstream lineStream(line);
                std::string token;

                std::vector<RTYPE> rowData;
                std::string rowLabel;
                rowIdx = 0;

                if (std::getline(lineStream,token,','))
                {
                    rowLabel = token;
                    rowLabels[rowLabel] = nullptr;
                }

                while (std::getline(lineStream,token,','))
                {
                    // CPP alternative to strtok, but with one delimiter

                    try {
                        num = std::stoi(token);
                        rowData.push_back(num);
                    }
                    catch (std::invalid_argument const &ex)
                    {
                        std::cout << "std::invalid_argument::what(): " << ex.what() << "token:  " << token << '\n';
                    }
                } // finish iterating row

                data.push_back(rowData);
                rowLabels[rowLabel] = &(data[rowIdx++]);
            } // finish iterating file

            file.close();

        }
        catch (const std::exception &e)
        {
            std::cerr << "General exception" << e.what() << '\n';
        }
        res[0] = rowLabels;
        res[1] = colLabels;
        res[2] = data;
        return res;
    } // endCSVToTable

} // end namespace

#endif //CPP_BACKTESTER_READCSV_H
