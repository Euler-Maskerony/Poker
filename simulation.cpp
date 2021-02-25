#include <set>
#include <iostream>
#include <algorithm>

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

    bool operator>(Card b)
    {
        return this->value > b.value;
    }

    bool operator>=(Card b)
    {
        return this->value >= b.value;
    }

    bool operator<(Card b)
    {
        return this->value < b.value;
    }

    bool operator<=(Card b)
    {
        return this->value <= b.value;
    }

    bool operator==(Card b)
    {
        return this->value == b.value;
    }
    
    int operator-(Card b)
    {
        return this->value - b.value;
    }

    bool operator==(int b)
    {
        return this->value == b;
    }
};

class Combination
{
private:
    bool combs[10];  // just for convinience of checking
public:
    int c_type;
    std::string info;

    Combination(Card cards[5])
    {
        int f,l;
        combs[flash] = true;
        combs[high_card] = true;
        for(int i(0); i < 5; ++i)
        {
            if(not i and combs[flash])
                if(cards[i-1].suit != cards[i].suit)
                    combs[flash] = false;
            f = std::distance(cards, std::find(cards, cards+4, cards[i]));
            std::reverse(cards, cards+4);
            l = std::distance(std::find(cards, cards+4, cards[i]), cards+4);
            std::reverse(cards, cards+4);
            switch (l - f + 1)
            {
            case 2:
                if(combs[pair]) combs[two_pairs] = true;
                else if(combs[triple]) combs[fullhouse] = true;
                else combs[pair] = true;
                break;
            case 3:
                if(combs[pair]) combs[fullhouse] = true;
                else combs[triple] = true;
                break;
            case 4:
                combs[square] = true;
                break;
            default:
                break;
            }
        }
        if(not combs[pair] and not combs[triple] and not combs[square] and cards[0] - cards[4] == 4)
            combs[straight] = true;
        if(combs[straight] and combs[flash])
            combs[straight_flash] = true;
        if(combs[straight_flash] and cards[0] == ace)
            combs[flash_royale] = true;
        
        for(int i(9); i >= 0; --i)
            if(combs[i])
            {
                c_type = i;
                break;
            }
    }

    bool operator<(Combination a)
    {
        if(this->c_type != a.c_type) { return this->c_type < a.c_type; }
        else { return this->info < a.info; }
    }
};

class Player
{
private:
    Card card_set[5];
    Combination comb;
};

int main()
{

    return 0;
}