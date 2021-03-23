#ifndef DECK
#define DECK

#include <set>
#include "card.h"

class Deck
{
private:
    std::multiset<Card> cards;
public:
    Deck();
    Card take();
    void take(Card card);
};

#endif