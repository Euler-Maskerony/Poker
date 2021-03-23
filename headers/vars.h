#ifndef VARS
#define VARS

#include <unordered_map>
#include <iostream>
#include <vector>
#include "card.h"

enum CardValue
{
    two = 'a',
    three = 'b',
    four = 'c',
    five = 'd',
    six = 'e',
    seven = 'f',
    eight = 'g',
    nine = 'h',
    ten = 'i',
    jack = 'j',
    queen = 'k',
    king = 'l',
    ace = 'm'
};

enum CardSuit
{
    club,
    diamond,
    heart,
    spade
};

enum Actions
{
    bet,
    check,
    fold
};

enum States
{
    pflop,
    flop,
    turn,
    river
};

enum Combinations
{
    high_card,
    pair,
    two_pairs,
    triple,
    straight,
    flash,
    fullhouse,
    square,
    straight_flash,
    flash_royale
};

extern std::unordered_map<int,std::string> CombinationsOut;

extern std::unordered_map<int,std::string> CardSuitsOut;

extern std::unordered_map<char,std::string> CardValuesOut;

typedef std::pair<Card,Card> Hand;

#endif