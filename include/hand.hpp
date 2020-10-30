#ifndef HAND_HPP
#define HAND_HPP

#include <string>
#include "unit.hpp"

struct Direction{
    enum Directions: unsigned char{
        North = 'N',
        East = 'E',
        West = 'W',
        South = 'S',
        DontMove = 'D'
    } direction;

    Direction() : direction(DontMove) {}
    Direction(Directions direct) : direction(direct) {}
    Direction(int number){
        switch (number)
        {
        case 0:
            direction = North;
            break;
        case 1:
            direction = East;
            break;
        case 2:
            direction = West;
            break;
        case 3:
            direction = South;
            break;
        
        default:
            direction = DontMove;
            break;
        }
    }
    Direction(char direct){
        switch (direct)
        {
        case 'N':
            direction = North;
            break;
        case 'E':
            direction = East;
            break;
        case 'W':
            direction = West;
            break;
        case 'S':
            direction = South;
            break;
        
        default:
            direction = DontMove;
            break;
        }
    }
    
    bool operator==(const Direction& d)const { return direction == d.direction; }
    bool operator!=(const Direction& d)const { return direction != d.direction; }
    
    char toChar()const{
        return static_cast<char>(direction);
    }

    std::string toString()const{
        switch (direction)
        {
            case North:
                return std::string("North");
            case East:
                return std::string("East");
            case West:
                return std::string("West");
            case South:
                return std::string("South");
            default:
                return std::string("DontMove");
        }
    }
};

struct Hand{
    Unit unit;
    Direction direct;

    Hand(): unit{-1, -1, UnitColor::Unknown, 255}, direct{}
    {}

    Hand(Unit u, Direction d): unit(u), direct(d)
    {
    }

    Hand(const std::string& src):
    unit{static_cast<int8_t>(src[2]-'0'), static_cast<int8_t>(src[3]-'0'), src[1], name2id(src[0])},
    direct{src[5]}
    {
    }

    std::string toString() const{
        return unit.toString() + "," + direct.toChar();
    }
    
    operator std::string() const { return toString(); }
};

inline std::ostream& operator<<(std::ostream& stream, const Hand& value){
    return stream << value.toString();
}

#endif