#ifndef PLAY
#define PLAY

#include <set>
#include <vector>
#include "combination.h"
#include "vars.h"

namespace pkr
{
    class Player
    {
    private:
        std::set<Combination,std::greater<Combination>> combinations;
        std::vector<Card> all_cards{7};
        bool host{false},folded{false};
        long long stack{-1};
        Hand hand;
        std::string nickname;
        std::string game_log{" "};

        bool next_combination (int mask[5]);
        bool ends_with(const std::string& value, const std::string& ending);
    public:
        Player() = delete;
        Player(Hand hand, bool host=false);
        Player(Hand hand, long long stack, bool host=false);

        void process(std::vector<Card> board);
        void changeGameState();
        std::string getLog();

        void check();
        void bet(int stake);
        void fold();

        bool isHost();
        bool isFolded();

        friend bool operator>(const Player& a, const Player& b);
        friend bool operator<(const Player& a, const Player& b);
    };
}

#endif