#include <algorithm>
#include <utility>
#include <ctime>
#include <time.h>
#include "simulation.h"
#include <cassert>

Card::Card()
{
    this->value = std::rand() % (ace-two+1) + two;
    this->suit = std::rand() % (spade+1);
}

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

Combination::Combination(std::vector<Card> cards)
{
    int f,l;
    for(int i(0); i < 10; ++i)
        combs[i] = false;
    sort(cards.begin(),cards.end(),std::greater<Card>());
    combs[flash] = true;
    combs[high_card] = true;
    for(int i(0); i < 5; ++i)
        info += cards[i].value;
    sort(info.begin(), info.end(), std::greater<char>());
    for(int i(0); i < 5; ++i)
    {
        if(i and combs[flash])
            if(cards[i-1].suit != cards[i].suit)
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

Game::Game(Hand host_hand, std::vector<Card> board, int players_num)
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
    players.push_back(Player(host_hand));
    players[0].process(this->board);
    players[0].host = true;
    for(int i(1); i < players_num; ++i)
    {
        players.push_back(Player(std::make_pair(deck.take(),deck.take())));
        (*players.rbegin()).process(this->board);
    }
    std::sort(players.begin(),players.end(),std::greater<Player>());
    win = players[0].host;
}

std::ostream& operator<<(std::ostream& out, const Game& g)
{
    out << "Board: ";
    for(Card c : g.board)
    {
        out << '|' << CardValuesOut[c.value] << CardSuitsOut[c.suit] << '|';
    }
    out << '\n';
    for(Player p : g.players)
    {
        out << "-----------------" << '\n';
        if(p.host)
            out << "(You)" << '\n';
        out << "Hand: " << '|' << CardValuesOut[p.hand.first.value] << CardSuitsOut[p.hand.first.suit] << '|' << CardValuesOut[p.hand.second.value] << CardSuitsOut[p.hand.second.suit] << '|';
        out << '\n';
        out << CombinationsOut[(*p.combinations.begin()).c_type];
        if(*p.combinations.begin() == *(*g.players.begin()).combinations.begin())
            out << '\n' << "Winner";
        out << '\n';
    }
    out << "-----------------" << '\n';

    return out;
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

int main()
{
    Game* g;
    int i(0);
    clock_t st = clock();
    while(i < 10000)
    {
        g = new Game(std::make_pair(Card(),Card()),std::vector<Card>(),8);
        ++i;
    }
    std::cout << 10000*CLOCKS_PER_SEC/(clock()-st) << '\n';
    return 0;
}