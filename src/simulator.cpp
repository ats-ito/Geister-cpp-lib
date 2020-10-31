#include "geister.hpp"
#include "unit.hpp"
#include "simulator.hpp"
#include "result.hpp"
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

Simulator::Simulator(const Geister& g, std::string_view ptn): root(g, "", ptn), depth(0)
{
}

// 可能性のあるすべての相手駒パターンを列挙
std::vector<std::string> Simulator::getLegalPattern() const
{
    constexpr static std::array<const char*, 70> pattern = {
        "ABCD", "ABCE", "ABCF", "ABCG", "ABCH", "ABDE", "ABDF",
        "ABDG", "ABDH", "ABEF", "ABEG", "ABEH", "ABFG", "ABFH",
        "ABGH", "ACDE", "ACDF", "ACDG", "ACDH", "ACEF", "ACEG",
        "ACEH", "ACFG", "ACFH", "ACGH", "ADEF", "ADEG", "ADEH",
        "ADFG", "ADFH", "ADGH", "AEFG", "AEFH", "AEGH", "AFGH",
        "BCDE", "BCDF", "BCDG", "BCDH", "BCEF", "BCEG", "BCEH",
        "BCFG", "BCFH", "BCGH", "BDEF", "BDEG", "BDEH", "BDFG",
        "BDFH", "BDGH", "BEFG", "BEFH", "BEGH", "BFGH", "CDEF",
        "CDEG", "CDEH", "CDFG", "CDFH", "CDGH", "CEFG", "CEFH",
        "CEGH", "CFGH", "DEFG", "DEFH", "DEGH", "DFGH", "EFGH"
    };
    
    std::vector<std::string> res(70);
    std::vector<char> blue(4);
    std::vector<char> red(4);

    size_t bsize = std::distance(blue.begin(), std::copy_if(Unit::nameList.begin(), Unit::nameList.begin()+8, blue.begin(), [&](const auto u){ return current.allUnit()[u-'A'+8].color().isBlue();}));
    blue.resize(bsize);
    size_t rsize = std::distance(red.begin(), std::copy_if(Unit::nameList.begin(), Unit::nameList.begin()+8, red.begin(), [&](const auto u){ return current.allUnit()[u-'A'+8].color().isRed();}));
    red.resize(rsize);

    size_t resSize = std::distance(res.begin(), std::copy_if(pattern.begin(), pattern.end(), res.begin(), [&](const char* p){ return 
        std::none_of(blue.begin(), blue.end(), [&](const char b){ return p[0] == b || p[1] == b || p[2] == b || p[3] == b; }) // 青と分かっている駒を含まないパターンである
        && std::all_of(red.begin(), red.end(), [&](const char r){ return p[0] == r || p[1] == r || p[2] == r || p[3] == r; }) // 赤と分かっている駒を含むパターンである
        ; }
    ));
    res.resize(resSize);
    return res;
}
    
// 未判明の相手駒色を適当に仮定
std::string Simulator::getRandomPattern() const
{
    std::vector<std::string> legalPattern = getLegalPattern();
    std::uniform_int_distribution<int> selector(0, legalPattern.size() - 1);
    return legalPattern[selector(mt)];
}

// 未判明の相手駒色を適当に仮定
void Simulator::setColor(std::string_view ptn){
    current.setColor("", ptn);
}
    
// 未判明の相手駒色を適当に仮定
void Simulator::setColorRandom(){
    static std::uniform_int_distribution<int> BorR(0,1);
    std::string red;
    int assumeTakeBlue = 4;
    int assumeTakeRed = 4;
    for(int i = 8; i < 16; ++i){
        UnitColor color = current.allUnit()[i].color();
        if(color == UnitColor::blue)
            assumeTakeBlue -= 1;
        if(color == UnitColor::red){
            assumeTakeRed -= 1;
            red += std::toupper(current.allUnit()[i].name());
        }
    }
    for(int i = 8; i < 16; ++i){
        if(current.allUnit()[i].color() == UnitColor::unknown){
            if(assumeTakeBlue > 0 && BorR(mt)){
                assumeTakeBlue -= 1;
            }
            else if(assumeTakeRed > 0){
                red += std::toupper(current.allUnit()[i].name());
                assumeTakeRed -= 1;
            }
            else{
                break;
            }
        }
    }
    current.setColor("", red);
}
    
double Simulator::playout(){
    static std::uniform_int_distribution<> selector;
    std::array<Hand, 32> lm;
    while(true){
        if(current.isEnd())
            break;
        // 相手の手番
        // std::vector<Hand> lm2 = current.getLegalMove2nd();
        // selector.param(std::uniform_int_distribution<>::param_type(0, lm2.size() - 1));
        selector.param(std::uniform_int_distribution<>::param_type(0, current.setLegalMove2nd(lm) - 1));
        Hand& m2 = lm[selector(mt)];
        current.move(m2);
        if(current.isEnd())
            break;
        // 自分の手番
        // std::vector<Hand> lm1 = current.getLegalMove1st();
        // selector.param(std::uniform_int_distribution<>::param_type(0, lm1.size() - 1));
        selector.param(std::uniform_int_distribution<>::param_type(0, current.setLegalMove1st(lm) - 1));
        Hand& m1 = lm[selector(mt)];
        current.move(m1);
    }
    return evaluate();
}

double Simulator::run(const size_t count){
    double result = 0.0;
    for(size_t i = 0; i < count; ++i){
        current = root;
        setColor(getRandomPattern());
        // current.printBoard();
        // std::cout << current.result() << "\n";
        // setColorRandom();
        result += playout();
    }
    return result;
}

double Simulator::run(std::string_view ptn, const size_t count){
    double result = 0.0;
    for(size_t i = 0; i < count; ++i){
        current = root;
        setColor(ptn);
        result += playout();
    }
    return result;
}
