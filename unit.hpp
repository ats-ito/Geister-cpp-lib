#include <string>

#ifndef UNIT
#define UNIT

struct Position{
    int x;
    int y;

    Position();
    Position(int x, int y);

    Position move(int dx, int dy);
    Position moveByDir(char dir);
    
    bool onBoard()const;

    Position operator+(Position displacement);
    Position operator-(Position displacement);

    static Position outBoard;
    static Position escapeBoard;
};

struct UnitColor{
    enum Color: unsigned char{
        NotDefine = 0b000,
        Blue = 0b010,
        blue = 0b011,
        Red = 0b100,
        red = 0b101,
        Unknown = 0b000,
        unknown = 0b001,
        Purple = 0b110,
        purple = 0b111,
    } color;

    UnitColor() : color(NotDefine) {}
    UnitColor(Color color) : color(color) {}
    UnitColor(int number);
    UnitColor(char color);

    bool operator==(const UnitColor& c)const { return color == c.color; }
    bool operator!=(const UnitColor& c)const { return color != c.color; }
    
    bool isBlue()const{
        return static_cast<unsigned char>(color) & 0b010;
    }
    bool isRed()const{
        return static_cast<unsigned char>(color) & 0b100;
    }
    bool is1st()const{
        return !(static_cast<unsigned char>(color) & 0b1);
    }
    bool is2nd()const{
        return static_cast<unsigned char>(color) & 0b1;
    }

    char toChar()const{
        switch (color)
        {
            case Blue:
                return 'B';
            case blue:
                return 'b';
            case Red:
                return 'R';
            case red:
                return 'r';
            case Unknown:
                return 'U';
            case unknown:
                return 'u';
            default:
                return ' ';
        }
    }

    std::string toString()const{
        switch (color)
        {
            case Blue:
                return std::string("Blue");
            case blue:
                return std::string("blue");
            case Red:
                return std::string("Red");
            case red:
                return std::string("red");
            case Unknown:
                return std::string("Unknown");
            case unknown:
                return std::string("unknown");
            default:
                return std::string("Not defined");
        }
    }

    int toInt()const{
        switch (color)
        {
            case Blue:
                return 0;
            case blue:
                return 1;
            case Red:
                return 2;
            case red:
                return 3;
            case Unknown:
                return 4;
            case unknown:
                return 5;
            default:
                return 7;
        }
    }

    UnitColor reverseSide()const{
        switch (color)
        {
            case Blue:
                return blue;
            case blue:
                return Blue;
            case Red:
                return red;
            case red:
                return Red;
            case Unknown:
                return unknown;
            case unknown:
                return Unknown;
            default:
                return NotDefine;
        }
    }
};

class Unit{
public:
    int x, y;
    Position pos;
    UnitColor color;
    char name;
    Unit();
    Unit(int x, int y, int color, char name);
    Unit(int x, int y, char color, char name);
    Unit(int x, int y, UnitColor color, char name);

    std::string toString() const;

    bool onBoard() const
    {
        return 0 <= x && x <= 5 && 0 <= y && y <= 5;
    }
    bool isTaken() const
    {
        return x == 9 && y == 9;
    }
    bool isEscape() const
    {
        return x == 8 && y == 8;
    }
};

class OpUnit : public Unit{
public:
    double blue;
    OpUnit();
    OpUnit(int x, int y, int color, char name);
    OpUnit(int x, int y, char color, char name);
};

#endif