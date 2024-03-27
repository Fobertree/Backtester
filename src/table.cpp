#include <table.h>
// https://stackoverflow.com/questions/24853914/read-2d-array-in-csv-into-a-map-c

Table::Table(std::string path)
{
    try
    {
        // Relative path
        std::ifstream fin("./data/" + path);

        if (!fin)
        {
            std::cout << "Error, could not open file";
            throw -1;
        }

        std::string row, column;
        float value;
        while (fin >> row >> column >> value)
        {
            data[row][col] = value;
        }

        fin.close();

        // Push rowLabels and colLabels

        for (auto &itR : data)
        {
            rowLabels.push_back(itR.first);
        }

        for (auto &itC : data.begin())
        {
            colLabels.push_back(itC.first);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Table::printData()
{
    for (auto &columnMap : data)
    {
        for (auto cell : columnMap.second)
        {
            std::cout << columnMap->first /*First label*/ << " "
                      << cell->first /*Second label*/ << " "
                      << cell->second /*Value*/ << std::endl;
        }
    }
}