#include "hand.hpp"
#include "Geister.hpp"
#include <string>

#ifndef PLAYER
#define PLAYER

#ifndef PLAYER_NAME
#define PLAYER_NAME No_Name
#endif
#define STRING(str) STRING_(str)
#define STRING_(str) #str

class Player{
protected:
    Geister game;
public:

    virtual std::string name(){
        return STRING(PLAYER_NAME);
    }

    virtual std::string decideHand(std::string res) = 0;
    virtual std::string decideRed() = 0;
};

#endif