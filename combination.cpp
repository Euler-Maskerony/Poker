#include <algorithm>
#include "headers/combination.h"
#include "headers/vars.h"

Combination::Combination(std::vector<Card> cards)
{
    int f,l;
    for(int i(0); i < 10; ++i)
        combs[i] = false;
    sort(cards.begin(),cards.end(),std::greater<Card>());
    combs[flash] = true;
    combs[high_card] = true;
    for(int i(0); i < 5; ++i)
        info += cards[i].getValue();
    sort(info.begin(), info.end(), std::greater<char>());
    for(int i(0); i < 5; ++i)
    {
        if(i and combs[flash])
            if(cards[i-1].getSuit() != cards[i].getSuit())
                combs[flash] = false;
    }
    for(int i(0); i < 5; ++i)
    {
        f = std::distance(cards.begin(), std::find(cards.begin(), cards.end(), cards[i]));
        std::reverse(cards.begin(), cards.end());
        l = std::distance(std::find(cards.begin(), cards.end(), cards[4-i]), cards.end())-1;
        std::reverse(cards.begin(), cards.end());
        switch(l - f + 1)
        {
        case 2:
            if(combs[pair])
            {
                combs[two_pairs] = true;
                cut(i,2,2,info);
            }
            else if(combs[triple])
                combs[fullhouse] = true;
            else if(not combs[two_pairs])
            {
                combs[pair] = true;
                cut(i,2,0,info);
            }
            break;
        case 3:
            if(combs[pair])
            {
                combs[fullhouse] = true;
                cut(i,3,0,info);
            }
            else
            {
                combs[triple] = true;
                cut(i,3,0,info);
            }
            break;
        case 4:
            combs[square] = true;
            cut(i,4,0,info);
            break;
        default:
            break;
        }
        i = l;
    }
    if((not combs[pair] and not combs[triple] and not combs[square] and cards[0] - cards[4] == 4) or 
    (cards[4] == two and cards[3] == three and cards[2] == four and cards[1] == five and cards[0] == ace))
        combs[straight] = true;
    if(combs[straight] and combs[flash])
        combs[straight_flash] = true;
    if(combs[straight_flash] and cards[0] == ace)
        combs[flash_royale] = true;
    
    for(int i(flash_royale); i >= high_card; --i)
        if(combs[i])
        {
            c_type = i;
            break;
        }
    if(c_type == flash)
        sort(info.begin(), info.end(), std::greater<char>());
    else if((c_type == straight or c_type == straight_flash) and cards[0] == ace)
        info = "ddabc";

}

void Combination::cut(int begin, int len, int pos, std::string& str)
    {
        str.insert(pos,str.substr(begin,len));
        str.erase(begin+len,len);
    }

bool operator<(const Combination& a, const Combination& b)
{
    if(a.c_type != b.c_type) { return a.c_type < b.c_type; }
    else { return a.info < b.info; }
}

bool operator>(const Combination& a, const Combination& b)
{
    if(a.c_type != b.c_type) { return a.c_type > b.c_type; }
    else { return a.info > b.info; }
}

bool operator==(const Combination& a, const Combination& b)
{
    if(a.c_type != b.c_type) { return a.c_type == b.c_type; }
    else { return a.info == b.info; }
}