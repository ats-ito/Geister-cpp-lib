#include <string>
#include <iostream>
#include "player.hpp"
#include "random.hpp"


class HumanPlayer: public Player{
public:
    virtual std::string decideRed(){
        std::cout << "Please Input Red Pattern. (Ex. ABCD)" << std::endl;
        std::string res;
        std::cin >> res;
        return res;
    }

    virtual std::string decideHand(std::string res){
        game.setState(res);
        game.printBoard();
        std::string ret;

        while(true){
            std::cin >> ret;
            Unit unit = game.allUnit()[ret[0] - 'A'];
            Direction direct{ret[1]};
            if(game.canMove1st(unit, direct)){
                return Hand(unit, direct);
            }
            std::cout << "It's Illegal Move! " << Hand(unit, direct).toString() << std::endl;
        }
    }
};