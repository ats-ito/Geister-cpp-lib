#include <string>
#include <iostream>
#include <memory>
#include "randomPlayer.hpp"

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
RandomPlayer* createPlayer(){
    return new RandomPlayer();
}
}
