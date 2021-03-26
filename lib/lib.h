#ifndef LIB
#define LIB

#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <unordered_map>

namespace pkr
{
    enum CardValue
    {
        two = 'a',
        three = 'b',
        four = 'c',
        five = 'd',
        six = 'e',
        seven = 'f',
        eight = 'g',
        nine = 'h',
        ten = 'i',
        jack = 'j',
        queen = 'k',
        king = 'l',
        ace = 'm'
    };

    enum CardSuit
    {
        club,
        diamond,
        heart,
        spade
    };

    enum Actions
    {
        bet,
        check,
        fold
    };

    enum States
    {
        pflop,
        flop,
        turn,
        river
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

    extern std::unordered_map<int,std::string> CombinationsOut;

    extern std::unordered_map<int,std::string> CardSuitsOut;

    extern std::unordered_map<char,std::string> CardValuesOut;


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

    typedef std::pair<Card,Card> Hand;

    class Combination
    {
    private:
        bool combs[10];  // just for convinience of checking
        void cut(int begin, int len, int pos, std::string& str);
        int c_type;
        std::string info{""};
    public:

        Combination() = delete;

        Combination(std::vector<Card> cards);

        friend bool operator<(const Combination& a, const Combination& b);
        friend bool operator>(const Combination& a, const Combination& b);
        friend bool operator==(const Combination& a, const Combination& b);
    };

    class Deck
    {
    private:
        std::multiset<Card> cards;
    public:
        Deck();
        Card take();
        void take(Card card);
    };

        class Player
    {
    private:
        std::set<Combination,std::greater<Combination>> combinations;
        std::vector<Card> all_cards{7};
        bool host{false},folded{false};
        long long stack{-1};
        Hand hand;
        std::string nickname;
        std::string game_log{" "};

        bool next_combination (int mask[5]);
        bool ends_with(const std::string& value, const std::string& ending);
    public:
        Player() = delete;
        Player(Hand hand, bool host=false);
        Player(Hand hand, long long stack, bool host=false);

        void process(std::vector<Card> board);
        void changeGameState();
        std::string getLog();

        void check();
        void bet(int stake);
        void fold();

        bool isHost();
        bool isFolded();

        friend bool operator>(const Player& a, const Player& b);
        friend bool operator<(const Player& a, const Player& b);
    };

    typedef bool Win;

    class Game
    {
    private:
        std::vector<Card> board{5};
        long long pot_size;
        int last,BB{0},state{pflop};
        Deck deck;
        std::vector<Player> players;
        std::vector<long long> pot_sizes;
    public:
        Game() = delete;
        Game(Hand host_hand, std::vector<Card> board, int players_num, int BB);
        Game(std::vector<Player> players, std::vector<Card> board, int BB);// first player must be SB, second BB and so on

        std::vector<Win> simulate();
        void playerAction(int player_num, int action, int stake=0);
        void changeGameState();
        std::string getLog() const;

        friend std::ostream& operator<<(std::ostream& out, const Game& g);
    };
}
#endif