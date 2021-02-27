#include <set>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>

enum CardValue
{
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace
};

enum CardSuit
{
    club,
    diamond,
    heart,
    spade
};

enum Combinations
{
    high_card,
    pair,
    two_pairs,
    triple,
    straight,
    flash,
    fullhouse,
    square,
    straight_flash,
    flash_royale
};

class Card
{
public:
    int value;
    int suit;

    Card(int value, int suit) : value(value), suit(suit) {};

    Card()
    {
        this->value = std::rand() % (ace+1);
        this->suit = std::rand() % (spade+1);
    }

    friend bool operator>(const Card& a, const Card& b);
    friend bool operator<(const Card& a, const Card& b);
    friend bool operator==(const Card& a, const Card& b);
    friend bool operator==(const Card& a, const int b);
    friend int operator-(const Card& a, const Card& b);
};

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

typedef std::pair<Card,Card> Hand;

class Combination
{
private:
    bool combs[10];  // just for convinience of checking

    void cut(int begin, int len, int pos, std::string& str)
    {
        str.replace(pos,len,str,begin,len);
        str.erase(begin+len,len);
    }

public:
    int c_type;
    std::string info{""};

    Combination() = delete;

    Combination(std::vector<Card> cards)
    {
        int f,l;
        for(int i(0); i < 10; ++i)
            combs[i] = false;
        sort(cards.begin(),cards.end(),std::greater<Card>());
        combs[flash] = true;
        combs[high_card] = true;
        for(int i(0); i < 5; ++i)
            info += std::to_string(cards[i].value);
        sort(info.begin(), info.end(), std::greater<char>());
        for(int i(0); i < 5; ++i)
        {
            if(i and combs[flash])
                if(cards[i-1].suit != cards[i].suit)
                    combs[flash] = false;
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
        if(not combs[pair] and not combs[triple] and not combs[square] and cards[0] - cards[4] == 4)
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
    }

    friend bool operator<(const Combination& a, const Combination& b);
    friend bool operator>(const Combination& a, const Combination& b);
};

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

class Player
{
private:
    std::vector<Card> all_cards{7};

    bool next_combination (int mask[5]) 
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

public:
    std::set<Combination,std::greater<Combination>> combinations;
    long long stack{-1};
    Hand hand;
    bool host{false};

    Player(Hand hand) : hand(hand) {}
    Player(Hand hand, long long stack) : hand(hand), stack(stack) {}

    void process(std::vector<Card> board)
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
        while(next_combination(mask))
        {
            for(int i(0); i < 5; ++i)
                comb[i] = all_cards[mask[i]];
            combinations.insert(Combination(comb));
        }
    }

    friend bool operator>(const Player& a, const Player& b);
    friend bool operator<(const Player& a, const Player& b);
};

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

class Game
{
private:
    std::vector<Player> players;
    std::vector<Card> board{5};
    int last;
    bool win;
public:
    Game() = delete;

    Game(Hand host_hand, std::vector<Card> board, int players_num)
    {
        for(Card el : board)
        {
            this->board[last] = el;
            ++last;
        }
        while(last < 5)
        {
            this->board[last] = Card();
            ++last;
        }
        players.push_back(Player(host_hand));
        players[0].process(this->board);
        players[0].host = true;
        for(int i(1); i < players_num; ++i)
        {
            players.push_back(Player(std::make_pair(Card(),Card())));
            (*players.rbegin()).process(this->board);
        }
        std::sort(players.begin(),players.end(),std::greater<Player>());
        win = players[0].host;
    }
};

int main()
{
    Game* g = new Game(std::make_pair(Card(ace,heart),Card(jack,spade)),std::vector<Card>(),4);
    return 0;
}