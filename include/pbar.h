//
// Created by Alexa on 6/12/2024.
//

#ifndef CPP_BACKTESTER_PBAR_H
#define CPP_BACKTESTER_PBAR_H

// cli tqdm
// carriage return character

const constexpr char emp = '1';
const constexpr char full = '1';

class Pbar
{
public:
    Pbar();
    Pbar(int total);
    Pbar(int total, int size);
    void update();
protected:
private:
    void createPBar();
    int total;
    int current;
    int size;
};

#endif //CPP_BACKTESTER_PBAR_H
