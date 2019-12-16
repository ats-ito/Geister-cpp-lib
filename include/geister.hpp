#include <array>
#include <vector>
#include <string>
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

    static std::vector<char> unitList;

    Geister();
    Geister(std::string info);
    Geister(std::string red1st, std::string red2nd);
    Geister(const Geister &geister);

    void setState(std::string state);

    void initialize();

    void printAll() const;
    void printBoard() const;
    void printInfo() const;

    virtual std::array<Unit, 16>& allUnit();
    virtual const std::array<Unit, 16>& allUnit() const;

    virtual bool canMove1st(Unit unit, Direction direct) const;

    virtual bool canMove1st(Unit unit, char direct) const;

    virtual std::vector<Hand> getLegalMove1st() const;

    virtual bool canMove2nd(Unit unit, char direct) const;

    virtual std::vector<Hand> getLegalMove2nd() const;

    virtual std::string& toString() const;

    virtual void take(Unit& unit);
    
    virtual void escape(Unit& unit);

    virtual void move(char u, char direct);

    virtual void move(Hand h);

    virtual Result getResult() const;

    virtual bool isEnd() const;

    virtual Unit* getUnitByPos(int x, int y);

    virtual Geister mask();

    virtual void changeSide();

    virtual void countTaken();

    virtual operator std::string() const { return toString(); }

    int takenCount(UnitColor c) const{
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

    virtual Hand diff(const Geister& target);
};

Hand diff(const Geister& left, const Geister& right);

inline std::ostream& operator<<(std::ostream& stream, const Geister& value){
    return stream << value.toString();
}

#endif