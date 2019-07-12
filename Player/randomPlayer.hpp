#ifndef RANDOMPLAYER
#define RANDOMPLAYER
#include "../hand.hpp"
#include "../Geister.hpp"
#include <string>
#include "../random.hpp"
#include "../Player.hpp"


class RandomPlayer: public Player{
public:
    virtual std::string decideRed(){
        cpprefjp::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
        return pattern[serector(mt)];
    }

    virtual std::string decideHand(std::string res){
        game.setState(res);
        cpprefjp::random_device rd;
        std::mt19937 mt(rd());

        auto legalMoves = game.getLegalMove1st();
        std::uniform_int_distribution<int> serector1(0, legalMoves.size() - 1);
        auto action = legalMoves[serector1(mt) % legalMoves.size()];
        return Hand(action).toString();
    }
};

#endif