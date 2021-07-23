#include "headers/player.h"

namespace pkr
{
    void Player::process(std::vector<Card> board)
    {
        // Can't estimate combination of folded player
        if(this->folded)
            return;
        // Fill container with hand and board cards
        std::vector<Card> all_cards(board.size()+2);
        all_cards[0] = this->hand.first;
        all_cards[1] = this->hand.second;
        std::copy(board.begin(),board.end(),all_cards.begin()+2);
        // Create mask for permutations
        int mask[all_cards.size()];
        std::iota(mask,mask+all_cards.size(),0);
        // Find max combinations over all posible
        std::vector<Card> comb{5};
        std::set<Combination,std::greater<Combination>> combinations;
        do
        {
            for(int i(0); i < 5; ++i)
                comb[i] = all_cards[mask[i]];
            combinations.insert(Combination(comb));
        }
        while(std::next_permutation(mask,mask+all_cards.size()));

        this->combination = new Combination(*combinations.begin());
    }

    bool operator>(const Player& a, const Player& b)
    {
        if(a.combination == nullptr or b.combination == nullptr)
            throw std::invalid_argument("Start process for all players first");
        // Folded players are always smaller
        if(a.folded)
            return false;
        else if(b.folded)
            return true;
        else
            return *a.combination > *b.combination;
    }

    bool operator<(const Player& a, const Player& b)
    {
        if(a.combination == nullptr or b.combination == nullptr)
            throw std::invalid_argument("Start process for all players first");
        // Folded players are always smaller
        if(a.folded)
            return true;
        else if(b.folded)
            return false;
        else
            return *a.combination < *b.combination;
    }

    bool Player::ends_with(const std::string& value, const std::string& ending)
    {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    Player::Player() : host(false) { this->fold(); }

    Player::Player(Hand hand, bool host) : hand(hand), host(host) {}

    Player::Player(Hand hand, long long stack, bool host) : hand(hand), stack(stack), host(host) {}

    void Player::check() noexcept
    {
        if(*this->game_log.rbegin() != 'c')
        {
            if(*this->game_log.rbegin() != ' ')
                this->game_log += "/";
            this->game_log += "c";
        }
    }

    void Player::fold() noexcept
    {
        if(*this->game_log.rbegin() != 'f' and not this->folded)
        {
            this->last_bet = 0;
            if(*this->game_log.rbegin() != ' ')
                this->game_log += "/";
            this->game_log += "f";
            this->folded = true;
        }
    }

    void Player::bet(long long stake) noexcept
    {
        last_bet = stake;

        if(not ends_with(this->game_log,"b("+std::to_string(stake)+")"))
        {
            if(*this->game_log.rbegin() != ' ')
                this->game_log += "/";
            this->game_log += "b(" + std::to_string(stake) + ")";
            this->stack -= stake;
        }
    }

    void Player::skip() noexcept { this->game_log += "?"; }

    void Player::changeGameState() noexcept
    {
        if(not this->folded)
        {
            last_bet = 0;
            this->game_log += ' ';
        }
    }

    std::string Player::getLog() const noexcept { return nickname + ' ' + game_log; }
    long long Player::getLastBet() const noexcept{ return last_bet; }

    bool Player::isFolded() const noexcept { return folded; }
    bool Player::isHost() const noexcept { return host; }
}