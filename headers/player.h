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
        int state{0};
        std::set<Combination,std::greater<Combination>> combinations;
        std::vector<Card> all_cards{7};
        long long last_bet;

        bool host{false},folded{false};
        long long stack{-1};
        Hand hand;
        std::string nickname;
        std::string game_log{" "};

        bool next_combination (int mask[5]);
        bool ends_with(const std::string& value, const std::string& ending);
    public:
        Player() = delete;
        explicit Player(Hand hand, bool host=false);
        explicit Player(Hand hand, long long stack, bool host=false);

        void process(std::vector<Card> board);
        void changeGameState();
        std::string getLog();

        void check();
        void bet(long long stake);
        void fold();

        bool isHost();
        bool isFolded();
        long long getLastBet();

        friend bool operator>(const Player& a, const Player& b);
        friend bool operator<(const Player& a, const Player& b);
    };
}

#endif