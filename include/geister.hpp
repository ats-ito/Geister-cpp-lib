#ifndef GEISTER_HPP
#define GEISTER_HPP

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include "unit.hpp"
#include "hand.hpp"
#include "result.hpp"

class Geister
{
protected:
    std::array<Unit, 16> units;
    int takenBlue1st;
    int takenBlue2nd;
    int takenRed1st;
    int takenRed2nd;
    Result mResult;
    int mTurn;
    std::vector<std::pair<Hand, std::string>> mHistory;

    void countTaken();

    void take(Unit& unit);
    
    void escape(Unit& unit);
public:
    constexpr static uint16_t maxTurn = 200;
    constexpr static std::array<char, 16> unitIDs = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    Geister();
    Geister(const std::string& info);
    Geister(const std::string& red1st, const std::string& red2nd);
    Geister(const Geister& game, const std::string& red1, const std::string& red2);

    void setState(const std::string& state);
    void setColor(const std::string& first, const std::string& second);
    void setColor(const char unit, const UnitColor color);

    void initialize();

    void printBoard() const;
    void printInfo() const;
    void printAll() const;

    const std::array<Unit, 16>& allUnit() const noexcept;

    bool canMove1st(const Unit& unit, const Direction direct) const;

    bool canMove1st(const Unit& unit, const char direct) const;

    std::vector<Hand>& getLegalMove1st() const;

    bool canMove2nd(const Unit& unit, const char direct) const;

    std::vector<Hand>& getLegalMove2nd() const;

    std::string& toString() const;
    operator std::string() const;

    void move(const char u, const char direct);

    void move(const Hand& h);

    Result result() const noexcept;

    int turn() const noexcept;

    const std::vector<std::pair<Hand, std::string>>& history() const noexcept;

    bool isEnd() const noexcept;

    Unit* getUnitByPos(const int x, const int y);

    bool exist1st(const int x, const int y)const;
    bool exist2nd(const int x, const int y)const;

    Geister mask();

    void changeSide();

    int takenCount(const UnitColor& c) const noexcept;

    Hand diff(const Geister& target);
};

Hand diff(const Geister& left, const Geister& right);

inline std::ostream& operator<<(std::ostream& stream, const Geister& value){
    return stream << value.toString();
}

#include "impl/geister.hpp"

#endif