#ifndef CHOTOTSUPLAYER
#define CHOTOTSUPLAYER
#include <string>
#include "player.hpp"
#include "random.hpp"


class ChototsuPlayer: public Player{
    cpprefjp::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<int> choiceRandom;
public:
    ChototsuPlayer():
    mt(rd()),
    choiceRandom(0, 1)
    {}

    virtual std::string decideRed(){
        return std::string("BCFG");
    }

    virtual std::string decideHand(std::string res){
        game.setState(res);

        const std::array<Unit, 16>& units = game.allUnit();
        for(const Unit& u: units){
            if(u.color == UnitColor::Blue){
                if(u.x == 0 && u.y == 0){
                    return Hand({u, Direction::West});
                }
                if(u.x == 5 && u.y == 0){
                    return Hand({u, Direction::East});
                }
            }
        }
        for(const Unit& u: units){
            if(u.color == UnitColor::Blue && u.y == 0){
                if(u.x < 3 && u.x > 0){
                    return Hand({u, Direction::West});
                }
                else if(u.x < 5){
                    return Hand({u, Direction::East});
                }
            }
        }
        int mostFrontPos = units[0].y;
        int mostFrontIndex = 0;
        for(int u = 1; u < 8; ++u){
            const Unit& unit = units[u];
            if(unit.color == UnitColor::Blue && unit.y <= mostFrontPos && unit.y > 0){
                if(unit.y < mostFrontPos || choiceRandom(mt)){
                    mostFrontIndex = u;
                    mostFrontPos = unit.y;
                }
            }
        }
        return Hand({units[mostFrontIndex], Direction::North});
    }
};

#endif