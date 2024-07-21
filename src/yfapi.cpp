//
// Created by Alex on 5/19/2024.
//

#include <yfapi.h>
#include <cstdarg>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <curl/curl.h>
#include <stdlib.h> // realloc

typedef struct memory {
    char *response;
    size_t size;
} memory;

//const std::string& YfApi::concat(const std::string& tkn, ...)
//{
//    va_list args;
//    references don't work with va_start. Prob remove this anyways.
//    va_start(args,tkn);
//    std::string arg;
//    std::string res{""};
//
//    while (!((arg = va_arg(args,std::string)).empty()))
//    {
//        res += arg;
//    }
//    return res;
//}

//
//std::string YfApi::build_api_url(std::string ticker, std::string start_date, std::string end_date)
//{
//    // switch to move assignment over copying
//    std::vector<std::string> data = { ticker,start_date,end_date };
//    std::string res{""};
//    std::istringstream iss(yf_url);
//    std::string tmp;
//    int i{0};
//    int n{static_cast<int>(data.size())};
//
//    std::string token;
//    while (std::getline(iss,token,'*'))
//    {
//        res += token;
//        if (i < n)
//            res += data[i++];
//        //res += data.back();
//    }
//
//    // deallocate
//    data.clear();
//    data.shrink_to_fit();
//
//    return res;
//}

std::string YfApi::unix_to_date(uint32_t ts)
{
    std::time_t tmp = ts;
    // std::gmtime is deprecated (possible issues with thread safety?), but don't know any alternative really
    std::tm* t = std::gmtime(&tmp);
    std::stringstream ss;
    ss << std::put_time(t, "%Y-%m-%d %I:%M:%S");
    std::string output = ss.str();

    return output;
}

static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
    /*
     * Callback function for CURLLIB request
     * nmemb: number of members. json parsing needed
     */
    size_t realsize = size * nmemb;
    memory *mem = (struct memory *)clientp;

    char *ptr = static_cast<char *>(realloc(mem->response, mem->size + realsize + 1));
    if (!ptr)
        return 0; // out of memory
    mem-> response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
}

YfApi::YfApi(std::string url) {
    struct memory chunk = {0};
    if (curl) {
        CURLcode res;
        // send data to function (callback)
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,cb);

        // Pass 'chunk' struct to the callback function
        curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void * ) &chunk);
        // Set URL
        curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
        // Send request
        res = curl_easy_perform(curl);

        // free buffer
        free(chunk.response);

        curl_easy_cleanup(curl);
    }
}

const std::string& YfApi::build_api_url(const std::string& ticker, const std::string& start_date, const std::string& end_date)
{
    // interval 1D for now
    // std::search?
    // std::string base_url = "https://query1.finance.yahoo.com/v7/finance/download/{ticker}?period1={start_time}&period2={end_time}&interval={interval}&events=history";
    const std::vector<std::string>& items = {ticker,start_date,end_date};
    std::string res = base_url;
    size_t start_pos{0};

    for (int idx = 0; idx < items.size(); idx++)
    {
        const std::string& item = items[idx], from = build_url_order[idx];
        // replace std::search with boyer-moore in future?
        // use std::find or std::search?
        if ((start_pos = res.find(from,start_pos) != std::string::npos))
        {
            res.replace(start_pos,from.length(), item);
            start_pos += item.length();
        }
    }

    return res;

}
