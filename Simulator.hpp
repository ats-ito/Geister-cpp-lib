#ifndef __SIMULATOR__
#define __SIMULATOR__

#include "Geister.hpp"
#include <random>
#include "random.hpp"


class Simulator{
    static cpprefjp::random_device rd;
    static std::mt19937 mt;
public:
    Geister geister;
    
    int depth;

    Simulator();
    Simulator(const Geister& geister);
    Simulator(const Geister& geister, std::string ptn);

    virtual void init(const Geister& geister);
    virtual void init(const Geister& geister, std::string ptn);
    
    // 未判明の相手駒色を適当に仮定
    virtual void setColorRandom();
    
    virtual double playout();

    virtual double run();

    virtual double run(int count);

    virtual double evaluate() const
    {
        return geister.checkResult() > 0 ? 1.0 : -1.0;
    }
};

#endif