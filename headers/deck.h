#ifndef DECK
#define DECK

#include <set>
#include <cstdlib>
#include <ctime>
#include "card.h"
#include "vars.h"

namespace pkr
{
    class Deck
    {
    private:
        std::multiset<Card> cards;
    public:
        Deck();
        Card take();
        void take(Card card);
    };
}

#endif