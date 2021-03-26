#ifndef CARD
#define CARD

class Card
{
private:
    char value;
    int suit;
public:

    Card(const int value, const int suit);
    Card();

    char getValue();
    int getSuit();

    friend bool operator>(const Card& a, const Card& b);
    friend bool operator<(const Card& a, const Card& b);
    friend bool operator==(const Card& a, const Card& b);
    friend bool operator==(const Card& a, const int b);
    friend int operator-(const Card& a, const Card& b);
};

#endif