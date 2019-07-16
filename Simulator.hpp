#ifndef __SIMULATOR__
#define __SIMULATOR__

#include "Geister.hpp"
#include <random>
#include "random.hpp"

extern cpprefjp::random_device rd;
extern std::mt19937 mt;

class Simulator{
public:
    Geister geister;
    
    int depth;

    Simulator();
    Simulator(const Simulator& simulator);
    Simulator(const Geister& geister);
    Simulator(Geister geister, std::string ptn);
    
    // 未判明の相手駒色を適当に仮定
    virtual void assume();
    
    virtual double playout();

    virtual void initBoard();

    virtual double evaluate() const
    {
        return geister.checkResult() > 0 ? 1.0 : -1.0;
    }
};

#endif