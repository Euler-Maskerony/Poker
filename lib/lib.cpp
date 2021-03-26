#include "lib.h"

namespace pkr
{
    Card::Card()
    {
        this->value = std::rand() % ((int)CardValue::ace-(int)CardValue::two+1) + (int)CardValue::two;
        this->suit = std::rand() % ((int)CardSuit::spade+1);
    }

    Card::Card(const int value, const int suit) : value(value), suit(suit) {}

    bool operator>(const Card& a, const Card& b)
    {
            return a.value > b.value;
    }

    bool operator<(const Card& a, const Card& b)
    {
            return a.value < b.value;
    }

    bool operator==(const Card& a, const Card& b)
    {
            return a.value == b.value;
    }

    bool operator==(const Card& a, const int b)
    {
            return a.value == b;
    }

    int operator-(const Card& a, const Card& b)
    {
            return a.value - b.value;
    }

    int Card::getSuit() { return suit; }

    char Card::getValue() { return value; }

    Combination::Combination(std::vector<Card> cards)
    {
        int f,l;
        for(int i(0); i < 10; ++i)
            combs[i] = false;
        sort(cards.begin(),cards.end(),std::greater<Card>());
        combs[flash] = true;
        combs[high_card] = true;
        for(int i(0); i < 5; ++i)
            info += cards[i].getValue();
        sort(info.begin(), info.end(), std::greater<char>());
        for(int i(0); i < 5; ++i)
        {
            if(i and combs[flash])
                if(cards[i-1].getSuit() != cards[i].getSuit())
                    combs[flash] = false;
        }
        for(int i(0); i < 5; ++i)
        {
            f = std::distance(cards.begin(), std::find(cards.begin(), cards.end(), cards[i]));
            std::reverse(cards.begin(), cards.end());
            l = std::distance(std::find(cards.begin(), cards.end(), cards[4-i]), cards.end())-1;
            std::reverse(cards.begin(), cards.end());
            switch(l - f + 1)
            {
            case 2:
                if(combs[pair])
                {
                    combs[two_pairs] = true;
                    cut(i,2,2,info);
                }
                else if(combs[triple])
                    combs[fullhouse] = true;
                else if(not combs[two_pairs])
                {
                    combs[pair] = true;
                    cut(i,2,0,info);
                }
                break;
            case 3:
                if(combs[pair])
                {
                    combs[fullhouse] = true;
                    cut(i,3,0,info);
                }
                else
                {
                    combs[triple] = true;
                    cut(i,3,0,info);
                }
                break;
            case 4:
                combs[square] = true;
                cut(i,4,0,info);
                break;
            default:
                break;
            }
            i = l;
        }
        if((not combs[pair] and not combs[triple] and not combs[square] and cards[0] - cards[4] == 4) or 
        (cards[4] == two and cards[3] == three and cards[2] == four and cards[1] == five and cards[0] == ace))
            combs[straight] = true;
        if(combs[straight] and combs[flash])
            combs[straight_flash] = true;
        if(combs[straight_flash] and cards[0] == ace)
            combs[flash_royale] = true;
        
        for(int i(flash_royale); i >= high_card; --i)
            if(combs[i])
            {
                c_type = i;
                break;
            }
        if(c_type == flash)
            sort(info.begin(), info.end(), std::greater<char>());
        else if((c_type == straight or c_type == straight_flash) and cards[0] == ace)
            info = "ddabc";

    }

    void Combination::cut(int begin, int len, int pos, std::string& str)
        {
            str.insert(pos,str.substr(begin,len));
            str.erase(begin+len,len);
        }

    bool operator<(const Combination& a, const Combination& b)
    {
        if(a.c_type != b.c_type) { return a.c_type < b.c_type; }
        else { return a.info < b.info; }
    }

    bool operator>(const Combination& a, const Combination& b)
    {
        if(a.c_type != b.c_type) { return a.c_type > b.c_type; }
        else { return a.info > b.info; }
    }

    bool operator==(const Combination& a, const Combination& b)
    {
        if(a.c_type != b.c_type) { return a.c_type == b.c_type; }
        else { return a.info == b.info; }
    }

    Deck::Deck()
    {
        for(int i(club); i <= spade; ++i)
        {
            for(int j(two); j <= ace; ++j)
                cards.insert(Card(j,i));
        }
    }

    Card Deck::take()
    {
        std::multiset<Card>::iterator c(cards.begin());
        std::advance(c,std::rand() % cards.size());
        Card card = *c;
        cards.erase(c);

        return card;
    }

    void Deck::take(Card card)
    {
        cards.erase(cards.find(card));
    }

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

    void Player::process(std::vector<Card> board)
    {
        all_cards[0] = hand.first;
        all_cards[1] = hand.second;
        int n(4);
        int mask[n+1];
        for(int i(2); i < 7; ++i)
        {
            all_cards[i] = board[i-2];
            mask[i-2] = i-2;
        }
        int i;
        std::vector<Card> comb{5};
        do
        {
            for(int i(0); i < 5; ++i)
                comb[i] = all_cards[mask[i]];
            combinations.insert(Combination(comb));
        } while(next_combination(mask));
    }

    bool Player::next_combination(int mask[5]) 
        {
            for (int i=4; i>=0; --i)
                if (mask[i] < 2+i) {
                    ++mask[i];
                    for (int j=i+1; j<5; ++j)
                        mask[j] = mask[j-1]+1;
                    return true;
            }
            return false;
        }

    bool operator>(const Player& a, const Player& b)
    {
        try
        {
            return *(a.combinations.begin()) > *(b.combinations.begin());
        }
        catch(const std::exception& e)
        {
            std::cerr << "Start process for all players first" << '\n';
        }
    }

    bool operator<(const Player& a, const Player& b)
    {
        try
        {
            return *(a.combinations.begin()) < *(b.combinations.begin());
        }
        catch(const std::exception& e)
        {
            std::cerr << "Start process for all players first" << '\n';
        }
    }

    bool Player::ends_with(const std::string& value, const std::string& ending)
    {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    Player::Player(Hand hand, bool host) : hand(hand), host(host) {}

    Player::Player(Hand hand, long long stack, bool host) : hand(hand), stack(stack), host(host) {}

    void Player::check()
    {
        if(*game_log.rbegin() != 'c')
        {
            if(not *game_log.rbegin() == ' ')
                game_log += "/";
            game_log += "c";
        }
    }

    void Player::fold()
    {
        if(not *game_log.rbegin() == ' ')
            game_log += "/";
        game_log += "f";
        folded = true;
    }

    void Player::bet(int stake)
    {
        if(not ends_with(game_log,"b("+std::to_string(stake)+")"))
        {
            if(not *game_log.rbegin() == ' ')
                game_log += "/";
            game_log += "b(" + std::to_string(stake) + ")";
            this->stack -= stake;
        }
    }

    void Player::changeGameState()
    {
        game_log += ' ';
    }

    std::string Player::getLog() { return nickname + ' ' + game_log; }

    bool Player::isFolded() { return folded; }
    bool Player::isHost() { return host; }

    std::unordered_map<int,std::string> CombinationsOut({
        {high_card, "High Card"},
        {pair, "Pair"},
        {two_pairs, "Two Pairs"},
        {triple, "Trips"},
        {straight, "Straight"},
        {flash, "Flash"},
        {fullhouse, "Fullhouse"},
        {square, "Square"},
        {straight_flash, "Straight-Flash"},
        {flash_royale, "Flash Royale"}
    });

    std::unordered_map<int,std::string> CardSuitsOut({
        {spade, "s"},
        {heart, "h"},
        {diamond, "d"},
        {club, "c"}
    });

    std::unordered_map<char,std::string> CardValuesOut({
        {'a', "2"},
        {'b', "3"},
        {'c', "4"},
        {'d', "5"},
        {'e', "6"},
        {'f', "7"},
        {'g', "8"},
        {'h', "9"},
        {'i', "10"},
        {'j', "J"},
        {'k', "Q"},
        {'l', "K"},
        {'m', "A"}
    });
}