#include "headers/deck.h"

namespace pkr
{
    Deck::Deck()
    {
        std::srand(std::time(NULL));
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
        if(auto c = cards.find(card) != cards.end())
            cards.erase(cards.find(card));
        else
            throw std::invalid_argument("Invalid card");
    }
}