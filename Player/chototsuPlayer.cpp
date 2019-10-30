#include <string>
#include <vector>
#include <map>
#include <memory>
#include "chototsuPlayer.hpp"


ChototsuPlayer player;

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
