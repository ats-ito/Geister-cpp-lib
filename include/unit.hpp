#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>
#include <array>

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

    constexpr UnitColor();
    constexpr UnitColor(Color color);
    UnitColor(char c);
    

    bool operator==(const UnitColor& c) const;
    bool operator!=(const UnitColor& c) const;
    
    bool isBlue() const;
    bool isRed() const;
    bool is1st() const;
    bool is2nd() const;

    char toChar() const;

    UnitColor reverseSide() const;
};

constexpr uint8_t name2id(char name);

class Unit{
    
    int8_t mX;
    int8_t mY;
    UnitColor mColor;
    uint8_t mID;
public:
    constexpr static std::array<char, 16> nameList = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    constexpr Unit(int8_t x, int8_t y, UnitColor color, uint8_t id);

    int x() const;
    int y() const;
    const UnitColor& color() const;
    char name() const;
    int id() const;

    void setColor(UnitColor c);
    void setPos(int x, int y);

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