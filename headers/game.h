#ifndef GAME
#define GAME

#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <opencv2/core.hpp>
#include "card.h"
#include "deck.h"
#include "player.h"
#include "vars.h"

namespace pkr
{
    typedef bool Win;

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
}

#endif