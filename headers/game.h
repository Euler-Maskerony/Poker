#ifndef GAME
#define GAME

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <stdexcept>
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
        std::vector<Card> board;  // Current board state
        long long pot_size{0};  // Current pot size (sum of all bets)
        int BB,  // BB value
        state,  // Current game state
        last_action{-1};  // Last player acted
        Deck deck;  // Deck of cards
        std::vector<Player> players;  // All players container
        std::vector<long long> pot_sizes;  // History of pot sizes at the end of each state
        std::vector<int> order;  // Number of players wich are in game
    public:
        Game() = delete;
        Game(Hand host_hand, std::vector<Card> board, int players_num, int BB=-1);
        Game(std::vector<Player> players, std::vector<Card> board, int BB=-1);  // first player must be SB, second BB and so on

        void betsEqulization();
        std::vector<Win> simulate();
        void playerAction(int player_num, int action, int stake=0);
        void changeGameState(std::vector<pkr::Card> new_cards,long long pot_size=0);
        void end();
        std::string getLog() const;
        int getState() const;

        friend std::ostream& operator<<(std::ostream& out, const Game& g);
    };
}

#endif