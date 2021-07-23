#include "headers/game.h"

namespace pkr
{
    Game::Game(Hand host_hand, std::vector<Card> board, int players_num, int BB) : BB(BB)
    {
        // Create active players list
        this->order.resize(players_num);
        std::iota(this->order.begin(),this->order.end(),0);
        // Reserve space for cards
        this->board.reserve(5);
        assert(board.size() <= 5 and board.size() != 2 and board.size() != 1);
        assert(players_num < 24);
        // Find out state
        this->state = std::max(0,(int)board.size()-2);
        // Take host's cards from the deck
        this->deck.take(host_hand.first);
        this->deck.take(host_hand.second);
        // Skip missed states in logs
        for(int i(0); i < state; ++i)
            std::for_each(this->players.begin(),this->players.end(),[](Player& p) { p.skip(); p.changeGameState(); });
        // Fill the board
        std::for_each(board.begin(),board.end(),[&](Card c) { this->deck.take(c); this->board.push_back(c); });
        // Create players
        players.push_back(Player(host_hand,true));
        for(int i(1); i < players_num; ++i)
            players.push_back(Player(std::make_pair(deck.take(),deck.take())));
    }

    Game::Game(std::vector<Player> players, std::vector<Card> board, int BB) : players(players), BB(BB)
    {
        // Create active players list
        for(int i(0); i < players.size(); ++i)
        {
            if(not players[i].isFolded())
                this->order.push_back(i);
        }
        // Reserve space for cards
        this->board.reserve(5);
        assert(board.size() <= 5 and board.size() != 2 and board.size() != 1);
        // Find out state
        this->state = std::max(0,(int)board.size()-2);
        // Skip unknown states
        for(int i(0); i < state; ++i)
            std::for_each(this->players.begin(),this->players.end(),[](Player& p) { p.skip(); p.changeGameState(); });
        // Fill the board
        std::for_each(board.begin(),board.end(),[&](Card c) { this->deck.take(c); this->board.push_back(c); });
    }

    std::vector<Win> Game::simulate()
    {
        // Create copies of board and deck
        std::vector<Card> board_copy(this->board);
        Deck deck_copy(this->deck);
        // Fill the board
        while(this->board.size() < 5)
            board_copy.push_back(deck_copy.take());
        // Process all players
        for(Player &p : players)
            p.process(board_copy);
        // Find out the winner
        auto winner(std::max(players.begin(),players.end()));
        // Print the result
        std::vector<Win> res;
        for(Player p : players)
            res.push_back(not (p < *winner));

        return res;
    }

    void Game::playerAction(int player_num, int action, int stake)
    {
        // Bet must be greater than BB, except small blind
        if(action == bet)
            assert((stake >= BB) or BB < 0);
        // If player has not folded and not acted yet
        if(not players[player_num].isFolded() and (player_num > this->last_action or this->last_action == *this->order.rbegin() or true))
        {
            // check what action he do
            switch(action)
            {
                case bet:
                    this->last_action = player_num;
                    // Trigger bet method
                    players[player_num].bet(stake);
                    // Update pot size
                    this->pot_size += stake;
                    // All previous players who didn't bet and fold checks
                    for(int i(player_num-1); i >= 0; --i)
                    {
                        std::string log(players[player_num].getLog());
                        if(*players[player_num].getLog().rbegin() == ' ')
                            players[i].check();
                    }
                    break;
                case fold:
                    {
                        this->last_action = player_num;
                        auto p_it(std::find(this->order.begin(),this->order.end(),player_num));
                        if(p_it != this->order.end())
                            this->order.erase(std::remove(this->order.begin(),this->order.end(),*p_it),this->order.end());
                        players[player_num].fold();
                        break;
                    }
                case check:
                    this->last_action = player_num;
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
        // Equalize bets
        betsEqulization();
        // Call players
        for(Player& p : players)
            p.changeGameState();
        // Change pot size
        if(pot_size == 0)
            this->pot_sizes.push_back(this->pot_size);
        else
        {
            this->pot_sizes.push_back(pot_size);
            this->pot_size = pot_size;
        }
        // Push new cards to the board
        if(new_cards.size() == 0)
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
            if(state == flop and new_cards.size() != 3)
                throw std::logic_error("Invalid cards number for this game state (" + std::to_string(new_cards.size()) + ")");
            for(pkr::Card card : new_cards)
                this->board.push_back(card);
        }
    }

    void Game::end()
    {
        betsEqulization();
        std::for_each(this->players.begin(),this->players.end(),[&](Player& p){ p.process(this->board); });
        // Find out the winner
        auto winner(*std::max(players.begin(),players.end()));
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