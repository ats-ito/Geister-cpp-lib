#ifndef CHOTOTSUPLAYER
#define CHOTOTSUPLAYER
#include "hand.hpp"
#include "Geister.hpp"
#include <string>
#include "random.hpp"
#include "Player.hpp"


class ChototsuPlayer: public Player{
public:
    virtual std::string decideRed(){
        return std::string("BCFG");
    }

    virtual std::string decideHand(std::string res){
        game.setState(res);
        cpprefjp::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> serector1(0, 1);

        auto legalMoves = game.getLegalMove1st();
        auto action = legalMoves[0];
        const auto& units = game.allUnit();
        for(const auto& u: units){
            if(u.color.toInt() == 0){
                if(u.x == 0 && u.y == 0){
                    return Hand({u, Direction::West}).toString();
                }
                if(u.x == 5 && u.y == 0){
                    return Hand({u, Direction::East}).toString();
                }
            }
        }
        for(const auto& u: units){
            if(u.color.toInt() == 0 && u.y == 0){
                if(u.x < 3 && u.x > 0){
                    return Hand({u, Direction::West}).toString();
                }
                else if(u.x < 5){
                    return Hand({u, Direction::East}).toString();
                }
            }
        }
        int mostFrontPos = 6;
        int mostFrontIndex = -1;
        for(int u = 0; u < 8; ++u){
            const auto& unit = units[u];
            if(unit.color == UnitColor::Blue && unit.y <= mostFrontPos && unit.y > 0){
                if(unit.y < mostFrontPos || serector1(mt)){
                    mostFrontIndex = u;
                    mostFrontPos = unit.y;
                }
            }
        }
        return Hand({units[mostFrontIndex], Direction::North}).toString();
    }
};

#endif