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
        int last;
        
        std::vector<Card> board{5};  // Current board state
        long long pot_size{0};  // Current pot size (sum of all bets)
        int BB{0},state{pflop};  // BB value and current game state
        Deck deck;  // Deck of cards
        std::vector<Player> players;  // All players
        std::vector<long long> pot_sizes;  // History of pot sizes at the end of each state
    public:
        Game() = delete;
        Game(Hand host_hand, std::vector<Card> board, int players_num, int BB);
        Game(std::vector<Player> players, std::vector<Card> board, int BB);  // first player must be SB, second BB and so on

        std::vector<Win> simulate();
        void playerAction(int player_num, int action, int stake=0);
        void changeGameState(std::vector<pkr::Card> new_cards,long long pot_size=0);
        std::string getLog() const;

        friend std::ostream& operator<<(std::ostream& out, const Game& g);
    };
}

#endif