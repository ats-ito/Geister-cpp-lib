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
    Simulator(const Geister& geister, const std::string& ptn);
    
    virtual std::vector<std::string>& getLegalPattern() const;
    // 未判明の相手駒色を適当に仮定
    virtual std::string getRandomPattern() const;

    virtual void setColor(const std::string& ptn);
    
    virtual void setColorRandom();

    virtual double playout();
    
    virtual double run(const size_t count = 1);

    virtual double run(const std::string& ptn, const size_t count = 1);

    virtual double evaluate() const
    {
        return current.getResult() == Result::Draw ? 0 : static_cast<int>(current.getResult()) > 0 ? 1.0 : -1.0;
    }
};

#endif