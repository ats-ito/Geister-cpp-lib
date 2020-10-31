#ifndef GEISTER_HPP
#define GEISTER_HPP

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <string_view>
#include "unit.hpp"
#include "hand.hpp"
#include "result.hpp"

class Geister
{
protected:
    Result mResult;
    int takenBlue1st;
    int takenBlue2nd;
    int takenRed1st;
    int takenRed2nd;
    int mTurn;
    std::array<Unit, 16> units;
    // std::vector<std::pair<Hand, std::string>> mHistory;

    void countTaken();

    void take(Unit& unit);
    
    void escape(Unit& unit);
public:
    constexpr static uint16_t maxTurn = 200;

    Geister();
    Geister(std::string_view info);
    Geister(std::string_view red1st, std::string_view red2nd);
    Geister(const Geister& game, std::string_view red1, std::string_view red2);

    void setState(std::string_view state);
    void setColor(std::string_view first, std::string_view second);
    void setColor(const Unit& unit, const UnitColor color);
    void setColor(const uint8_t id, const UnitColor color);

    void initialize();

    void printBoard() const;
    void printInfo() const;
    void printAll() const;

    const std::array<Unit, 16>& allUnit() const noexcept;

    bool canMove1st(const Unit& unit, const Direction direct) const;

    bool canMove1st(const Unit& unit, const char direct) const;

    std::vector<Hand> getLegalMove1st() const;
    int setLegalMove1st(std::array<Hand, 32>& legalMoves) const;

    bool canMove2nd(const Unit& unit, const char direct) const;

    std::vector<Hand> getLegalMove2nd() const;
    int setLegalMove2nd(std::array<Hand, 32>& legalMoves) const;

    std::string& toString() const;
    operator std::string() const;
    operator std::string_view() const;

    void move(const Hand& hand);

    Result result() const noexcept;

    int turn() const noexcept;

    bool isEnd() const noexcept;

    Unit* getUnitByPos(const int x, const int y);
    const Unit* getUnitByPos(const int x, const int y)const;

    bool exist1st(const int x, const int y)const;
    bool exist2nd(const int x, const int y)const;

    Geister mask();

    void changeSide();

    int takenCount(const UnitColor& c) const noexcept;
};

inline std::ostream& operator<<(std::ostream& stream, const Geister& value){
    return stream << value.toString();
}

#include "impl/geister.hpp"

#endif