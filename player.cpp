#include "headers/player.h"

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

bool Player::ends_with(const std::string& value, const std::string& ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

Player::Player(Hand hand, bool host) : hand(hand), host(host) {}

Player::Player(Hand hand, long long stack, bool host) : hand(hand), stack(stack), host(host) {}

void Player::check()
{
    if(*game_log.rbegin() != 'c')
    {
        if(not *game_log.rbegin() == ' ')
            game_log += "/";
        game_log += "c";
    }
}

void Player::fold()
{
    if(not *game_log.rbegin() == ' ')
        game_log += "/";
    game_log += "f";
    folded = true;
}

void Player::bet(int stake)
{
    if(not ends_with(game_log,"b("+std::to_string(stake)+")"))
    {
        if(not *game_log.rbegin() == ' ')
            game_log += "/";
        game_log += "b(" + std::to_string(stake) + ")";
        this->stack -= stake;
    }
}

void Player::changeGameState()
{
    game_log += ' ';
}

std::string Player::getLog() { return nickname + ' ' + game_log; }

bool Player::isFolded() { return folded; }
bool Player::isHost() { return host; }