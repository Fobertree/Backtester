#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

// https://stackoverflow.com/questions/24853914/read-2d-array-in-csv-into-a-map-c

typedef std::map<std::string, std::map<std::string, float>> datatable;
const char *DELIMS = "\t ,";
const int MAX_LINE_LENGTH = 1024;

class Table
{
public:
    Table(std::string path);
    // int &Table::operator[];
    void printData();
    std::vector<std::string> getDates() { return rowLabels; }
    std::vector<std::string> getTickers() { return colLabels; }
    datatable& getData() { return data; }

protected:
private:
    datatable data;
    std::vector<std::string> rowLabels;
    std::vector<std::string> colLabels;
};