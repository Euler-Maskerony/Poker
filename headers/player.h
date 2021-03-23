#ifndef PLAY
#define PLAY

#include <set>
#include <vector>
#include "combination.h"
#include "vars.h"

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

#endif