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
        assert(board.size() <= 5 and board.size() != 2 and board.size() != 1);
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
            assert(stake > BB or BB < 0);
        if(not players[player_num].isFolded())
        {
            switch(action)
            {
                case bet:
                    players[player_num].bet(stake);
                    this->pot_size += stake;
                    for(int i(player_num-1); i >= 0; --i)
                    {
                        std::string log(players[player_num].getLog());
                        if(*players[player_num].getLog().rbegin() == ' ')
                            players[i].check();
                    }
                    break;
                case fold:
                    players[player_num].fold();
                    break;
                case check:
                    players[player_num].check();
                    break;
            }
        }
    }

    void Game::betsEqulization()
    {
        long long max_bet(0);
        for(Player p : this->players)
        {
            if(p.getLastBet() > max_bet and not p.isFolded())
                max_bet = p.getLastBet();
        }
        for(Player& p : this->players)
        {
            if(max_bet == 0 and not p.isFolded())
                p.check();
            else if(p.getLastBet() < max_bet and not p.isFolded())
                p.bet(max_bet);
        }
    }

    void Game::changeGameState(std::vector<pkr::Card> new_cards,long long pot_size) 
    { 
        ++state;
        assert(state <= river and "River is the last state");
        assert(((state == flop and (new_cards.size() == 3 or new_cards.size() == 0)) or
        (state == turn and (new_cards.size() == 1 or new_cards.size() == 0)) or
        (state == river and (new_cards.size() == 1 or new_cards.size() == 0))) and "Invalid card size for this state");

        betsEqulization();

        for(Player& p : players)  // Call players
            p.changeGameState();
    
        if(pot_size == 0)  // Change pot size
            this->pot_sizes.push_back(this->pot_size);
        else
        {
            this->pot_sizes.push_back(pot_size);
            this->pot_size = pot_size;
        }

        if(new_cards.size() == 0)  // Push new cards to the board
        {
            this->board.push_back(pkr::Card());
            if(state == flop)
            {
                this->board.push_back(pkr::Card());
                this->board.push_back(pkr::Card());
            }
        }
        else
        {
            for(pkr::Card card : new_cards)
                this->board.push_back(card);
        }
    }

    std::string Game::getLog() const
    {
        std::string res;
        res += "Board: ";
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

    int Game::getState() const { return this->state; }

    std::ostream& operator<<(std::ostream& out, const Game& g)
    {
        out << g.getLog();
        return out;
    }
}