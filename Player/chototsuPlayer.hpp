#ifndef CHOTOTSUPLAYER
#define CHOTOTSUPLAYER
#include <string>
#include "../Player.hpp"
#include "../random.hpp"


class ChototsuPlayer: public Player{
    cpprefjp::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<int> serector1;
public:
    ChototsuPlayer():
    mt(rd()),
    serector1(0, 1)
    {}

    virtual std::string decideRed(){
        return std::string("BCFG");
    }

    virtual std::string decideHand(std::string res){
        game.setState(res);

        const auto& units = game.allUnit();
        for(const auto& u: units){
            if(u.color.toInt() == 0){
                if(u.x == 0 && u.y == 0){
                    return Hand({u, Direction::West});
                }
                if(u.x == 5 && u.y == 0){
                    return Hand({u, Direction::East});
                }
            }
        }
        for(const auto& u: units){
            if(u.color.toInt() == 0 && u.y == 0){
                if(u.x < 3 && u.x > 0){
                    return Hand({u, Direction::West});
                }
                else if(u.x < 5){
                    return Hand({u, Direction::East});
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
        return Hand({units[mostFrontIndex], Direction::North});
    }
};

#endif