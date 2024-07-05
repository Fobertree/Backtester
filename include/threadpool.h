//
// Created by Alexa on 6/22/2024.
//

#ifndef CPP_BACKTESTER_THREADPOOL_H
#define CPP_BACKTESTER_THREADPOOL_H

#include <thread>
#include <vector>
#include <functional>
#include <condition_variable>

// busy waiting vs conditional variable in mutex

using namespace std;

class ThreadPool
{
public:
    ThreadPool(size_t threads = std::thread::hardware_concurrency())
    {
        vector<thread> Pool;

        for (size_t ii = 0; ii < threads; ii++)
        {
            // use emplace_back over push_back because it constructs at the end rather than copying
            Pool.emplace_back([this] {
                void* test;
            });
        }
    }
protected:
private:
    std::condition_variable cv;
};

#endif //CPP_BACKTESTER_THREADPOOL_H
