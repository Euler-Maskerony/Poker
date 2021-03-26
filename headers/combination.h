#ifndef COMB
#define COMB

#include <iostream>
#include <vector>
#include "card.h"

class Combination
{
private:
    bool combs[10];  // just for convinience of checking
    void cut(int begin, int len, int pos, std::string& str);
    int c_type;
    std::string info{""};
public:

    Combination() = delete;

    Combination(std::vector<Card> cards);

    friend bool operator<(const Combination& a, const Combination& b);
    friend bool operator>(const Combination& a, const Combination& b);
    friend bool operator==(const Combination& a, const Combination& b);
};

#endif