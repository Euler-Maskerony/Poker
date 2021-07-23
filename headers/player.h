#ifndef PLAY
#define PLAY

#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include "combination.h"
#include "vars.h"

namespace pkr
{
    class Player
    {
    private:
        int state{0};
        Combination* combination{nullptr};
        long long last_bet;

        bool host,folded{false};
        long long stack{-1};
        Hand hand;
        std::string nickname;
        std::string game_log{" "};

        bool ends_with(const std::string& value, const std::string& ending);
    public:
        Player();
        explicit Player(Hand hand, bool host=false);
        explicit Player(Hand hand, long long stack, bool host=false);

        void process(std::vector<Card> board);
        void changeGameState() noexcept;
        std::string getLog() const noexcept;

        void check() noexcept;
        void bet(long long stake) noexcept;
        void fold() noexcept;
        void skip() noexcept;

        bool isHost() const noexcept;
        bool isFolded() const noexcept;
        long long getLastBet() const noexcept;

        friend bool operator>(const Player& a, const Player& b);
        friend bool operator<(const Player& a, const Player& b);
    };
}

#endif