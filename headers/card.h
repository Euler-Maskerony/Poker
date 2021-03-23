#ifndef CARD
#define CARD

class Card
{
public:
    char value;
    int suit;

    Card(int value, int suit) : value(value), suit(suit) {};
    Card();

    friend bool operator>(const Card& a, const Card& b);
    friend bool operator<(const Card& a, const Card& b);
    friend bool operator==(const Card& a, const Card& b);
    friend bool operator==(const Card& a, const int b);
    friend int operator-(const Card& a, const Card& b);
};

#endif