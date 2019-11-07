#include <array>
#include <vector>
#include <string>
#include "unit.hpp"
#include "hand.hpp"

#ifndef GEISTER
#define GEISTER

class Geister
{
protected:
    std::array<Unit, 16> units;
    int takeBlue1st;
    int takeBlue2nd;
    int takeRed1st;
    int takeRed2nd;
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

    virtual bool canMove1st(Unit unit, Direction direct) const;

    virtual bool canMove1st(Unit unit, char direct) const;

    virtual std::vector<Hand> getLegalMove1st() const;

    virtual bool canMove2nd(Unit unit, char direct) const;

    virtual std::vector<Hand> getLegalMove2nd() const;

    virtual std::string& toString() const;

    virtual void take(Unit unit);

    virtual void move(char u, char direct);

    virtual void move(Hand h);

    virtual double checkResult() const;

    virtual Unit* getUnitByPos(int x, int y);

    virtual Geister mask();

    virtual void changeSide();

    virtual void countTaken();

    virtual operator std::string() const { return toString(); }
};

inline std::ostream& operator<<(std::ostream& stream, const Geister& value){
    return stream << value.toString();
}

#endif