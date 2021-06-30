#include "headers/game.h"

namespace pkr
{
    Game::Game(Hand host_hand, std::vector<Card> board, int players_num, int BB) : BB(BB)
    {
        assert(players_num < 24);
        std::srand(std::time(NULL));
        deck.take(host_hand.first);
        deck.take(host_hand.second);
        for(Card el : board)
        {
            this->board[last] = el;
            deck.take(el);
            ++last;
        }
        while(last < 5)
        {
            this->board[last] = deck.take();
            ++last;
        }
        players.push_back(Player(host_hand,true));
        for(int i(1); i < players_num; ++i)
            players.push_back(Player(std::make_pair(deck.take(),deck.take())));
    }

    std::vector<Win> Game::simulate()
    {
        while(last < 5)
        {
            this->board[last] = deck.take();
            ++last;
        }
        players[0].process(this->board);
        for(Player &p : players)
            p.process(this->board);
        auto winner(std::max_element(players.begin(),players.end()));
        std::vector<Win> res;
        for(Player p : players)
            res.push_back(not (p < *winner));

        return res;
    }


    Game::Game(std::vector<Player> players, std::vector<Card> board, int BB) : players(players), BB(BB)
    {
        assert(BB > 0);
        state = std::max(0,(int)board.size()-2);
        for(Card el : board)
        {
            this->board[last] = el;
            deck.take(el);
            ++last;
        }
    }

    void Game::playerAction(int player_num, int action, int stake)
    {
        if(action == bet)
            assert(stake > BB);
        if(not players[player_num].isFolded())
        {

        }
    }

    void Game::changeGameState() 
    { 
        assert(state < river);
        for(Player& p : players)
            p.changeGameState();
        pot_sizes.push_back(pot_size);
        ++state;
    }

    std::string Game::getLog() const
    {
        std::string res;
        for(Card c : board)
            res += CardValuesOut[c.getValue()] + CardSuitsOut[c.getSuit()] + ' ';
        res += '\n';
        for(Player p : players)
            res += p.getLog() + '\n';
        for(long long ps : pot_sizes)
            res += std::to_string(ps) + ' ';
        res += '\n';

        return res;
    }

    std::ostream& operator<<(std::ostream& out, const Game& g)
    {
        out << g.getLog();
        return out;
    }
}