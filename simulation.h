#ifndef SIM
#define SIM

#include <unordered_map>
#include <iostream>
#include <vector>
#include <set>

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

std::unordered_map<char,std::string> CardValuesOut = 
{
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
};

std::unordered_map<int,std::string> CardSuitsOut = 
{
    {spade, "♤"},
    {heart, "♡"},
    {diamond, "♢"},
    {club, "♧"}
};

std::unordered_map<int,std::string> CombinationsOut =
{
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
};

class Card
{
public:
    char value;
    int suit;

    Card(int value, int suit) : value(value), suit(suit) {};
    Card();

    friend bool operator>(const Card& a, const Card& b);
    friend bool operator<(const Card& a, const Card& b);
    friend bool operator==(const Card& a, const Card& b);
    friend bool operator==(const Card& a, const int b);
    friend int operator-(const Card& a, const Card& b);
};

typedef std::pair<Card,Card> Hand;

class Combination
{
private:
    bool combs[10];  // just for convinience of checking
    void cut(int begin, int len, int pos, std::string& str);

public:
    int c_type;
    std::string info{""};

    Combination() = delete;

    Combination(std::vector<Card> cards);

    friend bool operator<(const Combination& a, const Combination& b);
    friend bool operator>(const Combination& a, const Combination& b);
    friend bool operator==(const Combination& a, const Combination& b);
};

class Player
{
private:
    std::vector<Card> all_cards{7};
    bool next_combination (int mask[5]) ;
public:
    std::set<Combination,std::greater<Combination>> combinations;
    long long stack{-1};
    Hand hand;
    bool host{false};

    Player(Hand hand) : hand(hand) {}
    Player(Hand hand, long long stack) : hand(hand), stack(stack) {}
    void process(std::vector<Card> board);

    friend bool operator>(const Player& a, const Player& b);
    friend bool operator<(const Player& a, const Player& b);
};

class Game
{
private:
    std::vector<Player> players;
    std::vector<Card> board{5};
    int last;
    bool win;
public:
    Game() = delete;
    Game(Hand host_hand, std::vector<Card> board, int players_num);

    friend std::ostream& operator<<(std::ostream& out, const Game& g);
};

#endif