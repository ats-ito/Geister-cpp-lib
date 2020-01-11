#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include "unit.hpp"
#include "hand.hpp"
#include "result.hpp"

#ifndef GEISTER
#define GEISTER

class Geister
{
protected:
    std::array<Unit, 16> units;
    int takenBlue1st;
    int takenBlue2nd;
    int takenRed1st;
    int takenRed2nd;
    Result result;
public:
    int turn;
    std::vector<std::pair<Hand, std::string>> history;

    static std::array<char, 16> unitList;

    Geister();
    Geister(std::string info);
    Geister(std::string red1st, std::string red2nd);
    Geister(const Geister &geister);

    void setState(const std::string& state);

    void initialize();

    void printBoard() const;
    void printInfo() const
    {
        for(int i = 0; i < 16; ++i){
            std::cout << units[i].name << "(" << units[i].color.toChar() << "): " << units[i].x << ", " << units[i].y << std::endl;
        }
    }
    void printAll() const
    {
        printBoard();
        printInfo();
    }

    std::array<Unit, 16>& allUnit(){
        return units;
    }
    const std::array<Unit, 16>& allUnit() const
    {
        return units;
    }

    bool canMove1st(const Unit& unit, const Direction direct) const;

    bool canMove1st(const Unit& unit, const char direct) const;

    std::vector<Hand>& getLegalMove1st() const;

    bool canMove2nd(const Unit& unit, const char direct) const;

    std::vector<Hand>& getLegalMove2nd() const;

    std::string& toString() const
    {
        static std::string res = "14U24U34U44U15U25U35U45U41u31u21u11u40u30u20u10u";
        for(int i = 0; i < 16; ++i){
            res[i*3] = '0' + units[i].x;
            res[i*3 + 1] = '0' + units[i].y;
            res[i*3 + 2] = units[i].color.toChar();
        }
        return res;
    }

    void take(Unit& unit){
        unit.x = 9;
        unit.y = 9;
        if(unit.is1st()){
            if(unit.color.isRed()){
                if(++takenRed1st == 4)
                    result = Result::TakenRed1st;
                return;
            }
            else if(unit.color.isBlue()){
                if(++takenBlue1st == 4)
                    result = Result::TakeBlue2nd;
                return;
            }
        }
        else if(unit.is2nd()){
            if(unit.color.isRed()){
                if(++takenRed2nd == 4)
                    result = Result::TakenRed2nd;
                return;
            }
            else if(unit.color.isBlue()){
                if(++takenBlue2nd == 4)
                    result = Result::TakeBlue1st;
                return;
            }
        }
    }
    
    void escape(Unit& unit){
        unit.x = 8;
        unit.y = 8;
        if(unit.is1st()){
            result = Result::Escape1st;
        }
        else{
            result = Result::Escape2nd;
        }
    }

    void move(const char u, const char direct);

    void move(const Hand& h){
        move(h.unit.name, h.direct.toChar());
    }

    Result getResult() const
    {
        return result;
    }

    bool isEnd() const
    {
        return result != Result::OnPlay;
    }

    Unit* getUnitByPos(const int x, const int y){
        for(int i = 0; i < 16; ++i){
            if(units[i].x == x && units[i].y == y){
                return &units[i];
            }
        }
        return nullptr;
    }

    Geister mask();

    void changeSide();

    void countTaken(){
        if((takenBlue1st = std::count_if(units.begin(), units.begin()+8,
            [&](Unit& u){ return u.isTaken() && u.color.isBlue(); })) == 4)
            result = Result::TakeBlue2nd;
        if((takenRed1st = std::count_if(units.begin(), units.begin()+8,
            [&](Unit& u){ return u.isTaken() && u.color.isRed(); })) == 4)
            result = Result::TakenRed1st;
        if((takenBlue2nd = std::count_if(units.begin()+8, units.end(),
            [&](Unit& u){ return u.isTaken() && u.color.isBlue(); })) == 4)
            result = Result::TakeBlue1st;
        if((takenRed2nd = takenRed2nd = std::count_if(units.begin()+8, units.end(),
            [&](Unit& u){ return u.isTaken() && u.color.isRed(); })) == 4)
            result = Result::TakenRed2nd;
    }

    operator std::string() const { return toString(); }

    int takenCount(const UnitColor& c) const{
        if(c == UnitColor::Blue)
            return takenBlue1st;
        if(c == UnitColor::Red)
            return takenRed1st;
        if(c == UnitColor::blue)
            return takenBlue2nd;
        if(c == UnitColor::red)
            return takenRed2nd;
        return -1;
    }

    Hand diff(const Geister& target);

    bool exist1st(const int x, const int y)const{
        for(int i = 0; i < 8; ++i){
            const auto& u = units[i];
            if(u.x == x && u.y == y){
                return true;
            }
        }
        return false;
    }
    bool exist2nd(const int x, const int y)const{
        for(int i = 8; i < 16; ++i){
            const auto& u = units[i];
            if(u.x == x && u.y == y){
                return true;
            }
        }
        return false;
    }
};

Hand diff(const Geister& left, const Geister& right);

inline std::ostream& operator<<(std::ostream& stream, const Geister& value){
    return stream << value.toString();
}

#endif