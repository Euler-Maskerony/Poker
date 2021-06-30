#include <unordered_map>
#include "headers/vars.h"

namespace pkr
{
    std::unordered_map<int,std::string> CombinationsOut({
        {high_card, "High Card"},
        {pair, "Pair"},
        {two_pairs, "Two Pairs"},
        {triple, "Trips"},
        {straight, "Straight"},
        {flash, "Flash"},
        {fullhouse, "Fullhouse"},
        {square, "Square"},
        {straight_flash, "Straight-Flash"},
        {flash_royale, "Flash Royale"}
    });

    std::unordered_map<int,std::string> CardSuitsOut({
        {spade, "s"},
        {heart, "h"},
        {diamond, "d"},
        {club, "c"}
    });

    std::unordered_map<char,std::string> CardValuesOut({
        {'a', "2"},
        {'b', "3"},
        {'c', "4"},
        {'d', "5"},
        {'e', "6"},
        {'f', "7"},
        {'g', "8"},
        {'h', "9"},
        {'i', "10"},
        {'j', "J"},
        {'k', "Q"},
        {'l', "K"},
        {'m', "A"}
    });
}