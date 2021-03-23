#ifndef LIB
#define LIB

#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <unordered_map>

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

extern std::unordered_map<int,std::string> CombinationsOut;

extern std::unordered_map<int,std::string> CardSuitsOut;

extern std::unordered_map<char,std::string> CardValuesOut;

typedef bool Win;

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

class Deck
{
private:
    std::multiset<Card> cards;
public:
    Deck();
    Card take();
    void take(Card card);
};

class Player
{
private:
    std::vector<Card> all_cards{7};
    bool next_combination (int mask[5]);
    long long stack{-1};
    std::string nickname;
    std::string game_log{" "};
public:
    std::set<Combination,std::greater<Combination>> combinations;
    Hand hand;
    bool host{false},folded{false};

    Player() = delete;
    Player(Hand hand);
    Player(Hand hand, long long stack);

    void process(std::vector<Card> board);
    bool ends_with(const std::string& value, const std::string& ending);
    void changeGameState();
    std::string getLog();

    void check();
    void bet(int stake);
    void fold();

    friend bool operator>(const Player& a, const Player& b);
    friend bool operator<(const Player& a, const Player& b);
};

class Game
{
private:
    std::vector<Card> board{5};
    long long pot_size;
    int last,BB{0},state{pflop};
    Deck deck;
    std::vector<Player> players;
    std::vector<long long> pot_sizes;
public:
    Game() = delete;
    Game(Hand host_hand, std::vector<Card> board, int players_num, int BB);
    Game(std::vector<Player> players, std::vector<Card> board, int BB);// first player must be SB, second BB and so on

    std::vector<Win> simulate();
    void playerAction(int player_num, int action, int stake=0);
    void changeGameState();
    std::string getLog() const;

    friend std::ostream& operator<<(std::ostream& out, const Game& g);
};

bool Player::ends_with(const std::string& value, const std::string& ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

Player::Player(Hand hand) : hand(hand) {}

Player::Player(Hand hand, long long stack) : hand(hand), stack(stack) {}

void Player::check()
{
    if(*game_log.rbegin() != 'c')
    {
        if(not *game_log.rbegin() == ' ')
            game_log += "/";
        game_log += "c";
    }
}

void Player::fold()
{
    if(not *game_log.rbegin() == ' ')
        game_log += "/";
    game_log += "f";
    folded = true;
}

void Player::bet(int stake)
{
    if(not ends_with(game_log,"b("+std::to_string(stake)+")"))
    {
        if(not *game_log.rbegin() == ' ')
            game_log += "/";
        game_log += "b(" + std::to_string(stake) + ")";
        this->stack -= stake;
    }
}

void Player::changeGameState()
{
    game_log += ' ';
}

std::string Player::getLog() { return nickname + ' ' + game_log; }

#endif