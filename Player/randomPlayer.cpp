#include <string>
#include <iostream>
#include "randomPlayer.hpp"

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
    player.initialize();
    return player.decideRed();
}
#ifdef _WIN32
__declspec(dllexport)
#endif
void finalize(std::string res){
    player.finalize(res);
}
}
