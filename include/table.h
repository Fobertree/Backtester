#include <cstring>
#include <fstream>
#include <string>
#include <map>
#include <iostream>

// https://stackoverflow.com/questions/24853914/read-2d-array-in-csv-into-a-map-c

const char *DELIMS = "\t ,";
const int MAX_LINE_LENGTH = 1024;

class Table
{
public:
    Table(std::string path);
    int &Array::operator[];
    void printData();

protected:
private:
    std::map<std::string, std::map<std::string, float>> data;
    std::vector<string> rowLabels;
    std::vector<string> colLabels;
}