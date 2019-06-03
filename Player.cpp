#include "Player.hpp"
#include "random/randomPlayer.hpp"
#include "Geister.hpp"
#include "hand.hpp"

using PlayerClass = RandomPlayer;
PlayerClass player;

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
std::string decideHand(std::string res){
    return player.decideHand(res);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
std::string decideRed(){
    return player.decideRed();
}
}