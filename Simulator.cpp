#include "Geister.hpp"
#include "unit.hpp"
#include "Simulator.hpp"
#include <array>
#include <algorithm>
#include <map>

cpprefjp::random_device Simulator::rd;
std::mt19937 Simulator::mt(rd());

Simulator::Simulator(const Simulator& s): root(s.root), depth(s.depth)
{
};

Simulator::Simulator(const Geister& g): root(g), depth(0)
{
}

Simulator::Simulator(const Geister& g, std::string ptn): root(g), depth(0)
{
    for(int u = 8; u < 16; ++u){
        if(ptn.find(std::toupper(root.allUnit()[u].name)) != std::string::npos)
            root.allUnit()[u].color = UnitColor::red;
        else
            root.allUnit()[u].color = UnitColor::blue;
    }
}
    
// 未判明の相手駒色を適当に仮定
void Simulator::setColorRandom(){
    std::uniform_int_distribution<int> BorR(0,1);
    int assumeTakeBlue = 4;
    int assumeTakeRed = 4;
    for(int i = 8; i < 16; ++i){
        auto color = current.allUnit()[i].color;
        if(color == UnitColor::blue)
            assumeTakeBlue -= 1;
        if(color == UnitColor::red)
            assumeTakeRed -= 1;
    }
    for(int i = 8; i < 16; ++i){
        if(current.allUnit()[i].color == UnitColor::unknown){
            if(assumeTakeBlue > 0 && BorR(mt)){
                current.allUnit()[i].color = UnitColor::blue;
                assumeTakeBlue -= 1;
            }
            else if(assumeTakeRed > 0){
                current.allUnit()[i].color = UnitColor::red;
                assumeTakeRed -= 1;
            }
            else{
                current.allUnit()[i].color = UnitColor::blue;
                assumeTakeBlue -= 1;
            }
        }
    }
    current.countTaken();
}
    
double Simulator::playout(){
    while(true){
        if(current.checkResult() != 0)
            break;
        // 相手の手番
        auto lm = current.getLegalMove2nd();
        std::uniform_int_distribution<int> selector1(0, lm.size() - 1);
        auto m = lm[selector1(mt)];
        current.move(m);
        if(current.checkResult() != 0)
            break;
        // 自分の手番
        lm = current.getLegalMove1st();
        std::uniform_int_distribution<int> selector2(0, lm.size() - 1);
        m = lm[selector2(mt)];
        current.move(m);
    }
    return evaluate();
}

double Simulator::run(){
    current = root;
    setColorRandom();
    double result = playout();
    return result;
}

double Simulator::run(int count){
    double result = 0.0;
    for(int i = 0; i < count; ++i){
        current = root;
        setColorRandom();
        result += playout();
    }
    return result;
}
