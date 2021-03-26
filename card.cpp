#include <cstdlib>
#include "headers/vars.h"
#include "headers/card.h"

Card::Card()
{
    this->value = std::rand() % ((int)CardValue::ace-(int)CardValue::two+1) + (int)CardValue::two;
    this->suit = std::rand() % ((int)CardSuit::spade+1);
}

Card::Card(const int value, const int suit) : value(value), suit(suit) {}

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

int Card::getSuit() { return suit; }

char Card::getValue() { return value; }