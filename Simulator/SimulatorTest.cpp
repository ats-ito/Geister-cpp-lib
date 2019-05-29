#include "Simulator.cpp"
#include <iostream>

int main(){
    Simulator s;
    s.geister.printBoard();
    // s.geister.move('a', 'S');
    // s.geister.move('a', 'S');
    // s.geister.move('a', 'S');
    // s.geister.move('a', 'S');
    // std::cout << s.geister.allUnit()[7].name << "(" << s.geister.allUnit()[7].x << ", " << s.geister.allUnit()[7].y << "): " << s.geister.allUnit()[7].color.is1st() << std::endl;
    // std::cout << s.geister.allUnit()[8].name << "(" << s.geister.allUnit()[8].x << ", " << s.geister.allUnit()[8].y << "): " << s.geister.allUnit()[8].color.is1st() << std::endl;
    // s.geister.move('a', 'W');
    // s.geister.move('a', 'W');
    // s.geister.move('a', 'W');
    // s.geister.move('a', 'W');
    // s.geister.printBoard();
    // s.geister.move('a', 'E');
    // s.geister.printBoard();
    // s.geister.move('a', 'W');
    // s.geister.printBoard();
    // s.geister.move('a', 'E');
    // s.geister.printBoard();
    for(auto&& lm: s.geister.getLegalMove2nd())
        std::cout << lm.unit.name << ", " << lm.direct.toChar() << std::endl;
    s.playout();
    s.geister.printBoard();
    std::cout << s.geister.checkResult() << std::endl;
    return 0;
}