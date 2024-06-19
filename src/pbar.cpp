//
// Created by Alexa on 6/12/2024.
//
#include <pbar.h>
#include <iostream>

Pbar::Pbar()
{
    total = 100;
    current = 0;
    size = 50;
    std::string output(size,emp);
    std::cout << output << '\r' << std::flush;
}

Pbar::Pbar(int total)
{
    this->total = total;
    current = 0;
    size = 50;
    std::string output(size,emp);
    std::cout << output << '\r' << std::flush;
}

Pbar::Pbar(int total, int size)
{
    this->total = total;
    current = 0;
    this->size = size;
    std::string output(size,emp);
    std::cout << output << '\r' << std::flush;
}

void Pbar::update()
{
    if (total == current)
    {
        return;
    }
    int oldPos = total/(current * size);

    int pct = total/(++current * this->size); // size chars

    if (oldPos == pct)
        return;

    //output to CLI
    std::string progress(pct-oldPos,full);
    std::cout << progress << std::flush;

}