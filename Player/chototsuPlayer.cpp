#include <string>
#include <vector>
#include <map>
#include <memory>
#include "chototsuPlayer.hpp"

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
ChototsuPlayer* createPlayer(){
    return new ChototsuPlayer;
}
}
