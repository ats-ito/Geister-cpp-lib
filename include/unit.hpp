#include <string>

#ifndef UNIT
#define UNIT

struct Position{
    int x;
    int y;

    constexpr Position():x{}, y{}{
    }
    constexpr Position(int x, int y): x{x}, y{y}{
    }

    void move(int dx, int dy){
        x += dx;
        y += dy;
    }
    Position moveByDir(char dir){
        return *this;
    }
    
    bool onBoard()const{
        return x < 6 && y < 6;
    }

    Position operator+(Position displacement){
        return {x+displacement.x, y+displacement.y};
    }
    Position operator-(Position displacement){
        return {x-displacement.x, y-displacement.y};
    }
};

constexpr inline Position outBoard{9, 9};
constexpr inline Position escapeBoard{8, 8};

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
    UnitColor(int number) {
        switch (number)
        {
            case 0:
                color = Blue;
                break;
            case 1:
                color = blue;
                break;
            case 2:
                color = Red;
                break;
            case 3:
                color = red;
                break;
            case 4:
                color = Unknown;
                break;
            case 5:
                color = unknown;
                break;
            case 6:
                color = Purple;
                break;
            case 7:
                color = purple;
                break;
            default:
                color = NotDefine;
        }
    }
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
            case Purple:
                return std::string("Purple");
            case purple:
                return std::string("purple");
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
    Position pos;
    UnitColor color;
    char name;

    constexpr Unit():x{-1}, y{-1}, color{UnitColor::Unknown}, name{' '}{}
    Unit(int x, int y, int color, char name): x{x}, y{y}, color(color), name{name}{}
    Unit(int x, int y, char color, char name): x{x}, y{y}, color(color), name{name}{}
    constexpr Unit(int x, int y, UnitColor color, char name): x{x}, y{y}, color{color}, name{name}{}

    std::string toString() const{
        return std::string{name} + std::string{color.toChar()} + std::to_string(x) + std::to_string(y);
    }

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

    bool is1st() const
    {
        return color.is1st();
    }
    bool is2nd() const
    {
        return color.is2nd();
    }
};


inline std::ostream& operator<<(std::ostream& stream, const Unit& value){
    return stream << value.toString();
}

#endif