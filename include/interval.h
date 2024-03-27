/*
Taken from Anthony Morast
https://www.anthonymorast.com/blog/2020/10/09/retrieving-historical-stock-data-in-c/
*/

#include <string.h>
enum Interval
{
    WEEKLY,
    MONTHLY,
    DAILY
};
static const std::string EnumAPIValues[]{"1wk", "1mo", "1d"};
std::string get_api_interval_value(int value)
{
    return EnumAPIValues[value];
}