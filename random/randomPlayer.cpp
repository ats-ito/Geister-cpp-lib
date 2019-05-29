#include "randomPlayer.hpp"
#include "Geister.hpp"
#include "hand.hpp"
#include "random.hpp"
#include <string>
#include <iostream>

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
Hand getHand(std::string res){
    Geister brd(res);
    cpprefjp::random_device rd;
	std::mt19937 mt(rd());

    auto legalMoves = brd.getLegalMove1st();
    std::uniform_int_distribution<int> serector1(0, legalMoves.size() - 1);
    auto action = legalMoves[serector1(mt) % legalMoves.size()];
    return action;
}
}
