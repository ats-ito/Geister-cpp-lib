#include "randomPlayer.hpp"
#include "../Geister.hpp"
#include "../hand.hpp"
#include "../random.hpp"
#include <string>
#include <iostream>

RandomPlayer player;

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
std::string decideHand(const std::string& res){
    return player.decideHand(res);
}
#ifdef _WIN32
__declspec(dllexport)
#endif
std::string decideRed(){
    return player.decideRed();
}
}
