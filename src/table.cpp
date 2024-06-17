#include <table.h>
#include <sstream>
#include <vector>
#include <fstream>

// https://stackoverflow.com/questions/24853914/read-2d-array-in-csv-into-a-map-c

// why both rowType and colType??? Should be just dataType?
template<typename rowType, typename colType>
DataTable::Table<rowType, colType>::Table(const std::string& path)
{
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
        float value;
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

        rowIdx = 0;
        int num;
        while (!file.eof())
        {
            // get row from csv
            std::getline(file,line);

            std::istringstream lineStream(line);
            std::string token;

            std::vector<rowType> rowData;
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
}


// Overloading map operator seems complicated and unnecessary rn. Remember map is a binary implementation
// const auto &Table::operator[];