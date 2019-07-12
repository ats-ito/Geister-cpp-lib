#include "Geister.hpp"
#include "unit.hpp"
#include "Simulator.hpp"
#include <array>
#include <algorithm>
#include <map>

cpprefjp::random_device rd;
std::mt19937 mt(rd());

Simulator::Simulator(): geister(){
    depth = 0;
    assume();
    initBoard();
}

Simulator::Simulator(const Geister& g): geister(g){
    depth = 0;
    assume();
    initBoard();
}

Simulator::Simulator(Geister g, std::string ptn): geister(g){
    depth = 0;
    constexpr int l2s = 'a' - 'A';
    if(ptn.size() == 4){
        for(int u = 8; u < 16; ++u){
            for(int c = 0; c < ptn.size(); ++c){
                if((ptn[c] + l2s) == geister.allUnit()[u].name){
                    geister.allUnit()[u].color = UnitColor(3);
                    break;
                }
            }
            if(geister.allUnit()[u].color.toInt() != 3)
                geister.allUnit()[u].color = UnitColor(1);
        }
    }
    else
        assume();
    initBoard();
}
    
    // 未判明の相手駒色を適当に仮定
void Simulator::assume(){
    std::uniform_int_distribution<int> BorR(0,1);
    int assumeTakeBlue = 4;
    int assumeTakeRed = 4;
    for(int i = 8; i < 16; ++i){
        auto color = geister.allUnit()[i].color;
        if(color == UnitColor::blue)
            assumeTakeBlue -= 1;
        if(color == UnitColor::red)
            assumeTakeRed -= 1;
    }
    for(int i = 8; i < 16; ++i){
        if(geister.allUnit()[i].color == UnitColor::unknown){
            if(assumeTakeBlue > 0 && BorR(mt)){
                geister.allUnit()[i].color = UnitColor::blue;
                assumeTakeBlue -= 1;
            }
            else if(assumeTakeRed > 0){
                geister.allUnit()[i].color = UnitColor::red;
                assumeTakeRed -= 1;
            }
            else{
                geister.allUnit()[i].color = UnitColor::blue;
                assumeTakeBlue -= 1;
            }
        }
    }
}
    
double Simulator::playout(){
    while(true){
        if(geister.checkResult() != 0)
            break;
        // 相手の手番
        auto lm = geister.getLegalMove2nd();
        std::uniform_int_distribution<int> selector1(0, lm.size() - 1);
        auto m = lm[selector1(mt)];
        geister.move(m.unit.name, m.direct.toChar());
        if(geister.checkResult() != 0)
            break;
        // 自分の手番
        lm = geister.getLegalMove1st();
        std::uniform_int_distribution<int> selector2(0, lm.size() - 1);
        m = lm[selector2(mt)];
        geister.move(m.unit.name, m.direct.toChar());
    }
    return geister.checkResult();
}

void Simulator::initBoard(){
    geister.takeBlue1st = 4;
    geister.takeRed1st = 4;
    geister.takeBlue2nd = 4;
    geister.takeRed2nd = 4;

    for(auto u: geister.allUnit()){
        if(u.x < 6){
            if(u.color.toInt() == 0)
                geister.takeBlue1st -= 1;
            else if(u.color.toInt() == 1)
                geister.takeBlue2nd -= 1;
            else if(u.color.toInt() == 2)
                geister.takeRed1st -= 1;
            else if(u.color.toInt() == 3)
                geister.takeRed2nd -= 1;
        }
    }
}
