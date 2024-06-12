//
// Created by Alex on 5/19/2024.
//

#ifndef CPP_BACKTESTER_YFAPI_H
#define CPP_BACKTESTER_YFAPI_H

#include <string>
#include <curl/curl.h>


class YfApi
{
public:
    YfApi(std::string url);
protected:
private:
    std::string const concat (const std::string tkn...); // variadic function
    // Use strtok here?
    const char* yf_url = "https://query1.finance.yahoo.com/v7/finance/chart/AAPL?range=2y&interval=1d&indicators=quote&includeTimestamps=true";
    std::string build_api_url(std::string ticker, std::string start_date, std::string end_date);
    std::string unix_to_date(uint32_t ts);
    CURL *curl = curl_easy_init();
};

#endif //CPP_BACKTESTER_YFAPI_H
