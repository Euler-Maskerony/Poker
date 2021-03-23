#include <cstdlib>
#include "headers/vars.h"
#include "headers/card.h"

Card::Card()
{
    this->value = std::rand() % (ace-two+1) + two;
    this->suit = std::rand() % (spade+1);
}

bool operator>(const Card& a, const Card& b)
{
        return a.value > b.value;
}

bool operator<(const Card& a, const Card& b)
{
        return a.value < b.value;
}

bool operator==(const Card& a, const Card& b)
{
        return a.value == b.value;
}

bool operator==(const Card& a, const int b)
{
        return a.value == b;
}

int operator-(const Card& a, const Card& b)
{
        return a.value - b.value;
}