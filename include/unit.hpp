#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>

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

    constexpr UnitColor() : color(NotDefine) {}
    constexpr UnitColor(Color color) : color(color) {}
    UnitColor(char c) {
        switch (c)
        {
            case 'B':
                color = Blue;
                break;
            case 'b':
                color = blue;
                break;
            case 'R':
                color = Red;
                break;
            case 'r':
                color = red;
                break;
            case 'U':
                color = Unknown;
                break;
            case 'u':
                color = unknown;
                break;
            case 'P':
                color = Purple;
                break;
            case 'p':
                color = purple;
                break;
            default:
                color = NotDefine;
        }
    }
    

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
            case Purple:
                return 'P';
            case purple:
                return 'p';
            default:
                return ' ';
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
            case Purple:
                return purple;
            case purple:
                return Purple;
            default:
                return NotDefine;
        }
    }
};

class Unit{
public:
    int x, y;
    UnitColor color;
    char name;

    constexpr Unit();
    constexpr Unit(int x, int y, UnitColor color, char name);

    std::string toString() const;

    bool onBoard() const;
    bool isTaken() const;
    bool isEscape() const;

    bool is1st() const;
    bool is2nd() const;

    bool isBlue() const;
    bool isRed() const;

    void reverseSide();
};


inline std::ostream& operator<<(std::ostream& stream, const Unit& value){
    return stream << value.toString();
}

#include "impl/unit.hpp"

#endif