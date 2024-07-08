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

using std::vector;

template<typename R = std::string, typename C = std::string, typename D = float>
class CSVReader
{
public:
    explicit CSVReader(const std::string& path = "./Data/data.csv", char delim = ',')
    {
        csv_path = path;
        try
        {
            // Relative path
            std::ifstream file(path);

            if (!file)
            {
                std::cout << "Error, could not open file";
                throw std::invalid_argument("INVALID PATH: ");
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

                while (std::getline(lineStream,token,delim))
                {
                    // CPP alternative to strtok, but with one delimiter
                    mapColLabels[(C)(token)] = colIdx++;
                    colLabels.push_back((C)(token));
                }
            }

            int num;
            while (!file.eof())
            {
                // get row from csv
                std::getline(file,line);

                std::istringstream lineStream(line);
                std::string token;

                std::vector<D> rowData;
                std::string rowLabel;
                rowIdx = 0;

                if (std::getline(lineStream,token,','))
                {
                    rowLabel = token;
                    rowLabels.push_back(rowLabel);
                }

                while (std::getline(lineStream,token,','))
                {
                    // CPP alternative to strtok, but with one delimiter

                    try {
                        num = std::stoi(token);
                        rowData.push_back((D)(num));
                    }
                    catch (std::invalid_argument const &e)
                    {
                        std::cout << "std::invalid_argument::what(): " << e.what() << "token:  " << token << '\n';
                    }
                } // finish iterating row

                data.push_back(rowData);
                mapRowLabels[(R)(rowLabel)] = &(data[rowIdx++]);
            } // finish iterating file

            file.close();

        }
        catch (std::invalid_argument const &e)
        {
            std::cerr << "Invalid Argument Exception: " << e.what() << "\nPath: " << path;
        }
        catch (const std::exception &e)
        {
            std::cerr << "General exception: " << e.what() << '\n';
        }
        res = std::make_tuple(rowLabels,colLabels,data);
    }

    std::tuple<vector<R>,vector<C>,vector<D>>& getAll() const
    {
        return res;
    };

    const vector<R>& getRowLabels() const
    {
        return rowLabels;
    }

    const vector<C>& getColLabels() const
    {
        return colLabels;
    }

    const vector<vector<D>>& getData() const
    {
        return data;
    }

    [[nodiscard]] const std::string& getCSVPath() const
    {
        return csv_path;
    }

    //~CSVReader();
private:
    std::tuple<vector<R>,vector<C>,vector<vector<D>>> res;
    vector<R> rowLabels;
    vector<C> colLabels;
    vector<vector<D>> data;
    std::string csv_path;
    // mapped labels
    std::unordered_map<R,std::vector<D>*> mapRowLabels;
    std::unordered_map<C, int> mapColLabels;
}; // end CSVReader class

#endif //CPP_BACKTESTER_READCSV_H
