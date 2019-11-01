#ifndef __SIMULATOR__
#define __SIMULATOR__

#include "Geister.hpp"
#include <random>
#include "random.hpp"


class Simulator{
    cpprefjp::random_device rd;
    std::mt19937 mt;
public:
    Geister geister;
    
    int depth;

    Simulator();
    Simulator(const Geister& geister);
    Simulator(const Geister& geister, std::string ptn);

    virtual void init(const Geister& geister);
    virtual void init(const Geister& geister, std::string ptn);
    
    // 未判明の相手駒色を適当に仮定
    virtual void assume();
    
    virtual double playout();

    virtual double run();

    virtual double run(int count);

    virtual void initBoard();

    virtual double evaluate() const
    {
        return geister.checkResult() > 0 ? 1.0 : -1.0;
    }
};

#endif