#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "geister.hpp"
#include <random>
#include "random.hpp"
#include "result.hpp"


class Simulator{
protected:
    static cpprefjp::random_device rd;
    static std::mt19937 mt;
public:
    Geister root;
    Geister current;
    
    int depth;

    Simulator(const Simulator&);
    Simulator(const Geister& geister);
    Simulator(const Geister& geister, std::string ptn);
    
    // 未判明の相手駒色を適当に仮定
    virtual void setColorRandom();
    
    virtual double playout();

    virtual double run();

    virtual double run(int count);

    virtual double evaluate() const
    {
        return static_cast<int>(current.getResult()) > 0 ? 1.0 : -1.0;
    }
};

#endif