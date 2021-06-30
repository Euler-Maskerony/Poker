#include <cstdlib>
#include "headers/vars.h"
#include "headers/deck.h"

namespace pkr
{
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
}