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

// TODO: Refactor into adding .iloc and .loc

template<typename rowType,typename colType> float DataTable::Table<rowType,colType>::getValue(int rIdx, int cIdx)
{
    return data.at(rIdx).at(cIdx);
}

template<typename rowType, typename colType> float DataTable::Table<rowType,colType>::getValue(rowType rName, int cIdx)
{
    return rowLabels[rName].at(cIdx);
}

template<typename rowType, typename colType> float DataTable::Table<rowType,colType>::getValue(int rIdx, colType cName)
{
    return data.at(rIdx).at(colLabels[cName]);
}

template<typename rowType, typename colType> float DataTable::Table<rowType,colType>::getValue(rowType rName, colType cName)
{
    return rowLabels[cName].at(colLabels[rName]);
}

template<typename rowType, typename colType> int DataTable::Table<rowType,colType>::getRowIdx(rowType rowLabel,
                                                                                              bool closest)
{
    // bisect left
    int leap,len,cmp_res,cur_pos;
    cur_pos = 0;

    len = static_cast<int>(rowLabels.size());

    // switch range 1x to 2.6x slower than immediate if statements

    for (leap = len-1; leap > 0; leap /= 2)
    {
        if ((cmp_res = rowLabel.compare(rowVals[leap+cur_pos])) == 0)
        {
            return rowVals[leap+cur_pos];
        }
        else if (cmp_res < 0)
        {
            cur_pos += leap;
        }
    }

    // could not find precise date/string;
    if (closest)
        return rowVals[cur_pos];

    throw 2;
};

// Overloading map operator seems complicated and unnecessary rn. Remember map is a binary implementation
// const auto &Table::operator[];