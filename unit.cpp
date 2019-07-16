#include "unit.hpp"


Position::Position(){
    x = 0;
    y = 0;
};
Position::Position(int x, int y){
    this->x = x;
    this->y = y;
}

Position Position::move(int dx, int dy){
    return Position(x + dx, y + dy);
}
Position Position::moveByDir(char dir){
    return *this;
}

bool Position::onBoard()const{
    return x < 6 && y < 6;
}

Position Position::operator+(Position displacement){
    return move(displacement.x, displacement.y);
}
Position Position::operator-(Position displacement){
    return move(-displacement.x, -displacement.y);
}

Position Position::outBoard = Position(9, 9);
Position Position::escapeBoard = Position(8, 8);


UnitColor::UnitColor(int number) {
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
    }
}
UnitColor::UnitColor(char c) {
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
    }
}
    

Unit::Unit(){
    x = -1;
    y = -1;
    color = UnitColor::Unknown;
    name = ' ';
}
    
Unit::Unit(int x, int y, int color, char name){
    this->x = x;
    this->y = y;
    this->color = UnitColor(color);
    this->name = name;
}
    
Unit::Unit(int x, int y, char color, char name){
    this->x = x;
    this->y = y;
    this->color = UnitColor(color);
    this->name = name;
}

    
Unit::Unit(int x, int y, UnitColor color, char name){
    this->x = x;
    this->y = y;
    this->color = color;
    this->name = name;
}

std::string Unit::toString() const{
    return std::string{name} + std::string{color.toChar()} + std::to_string(x) + std::to_string(y);
}

OpUnit::OpUnit(){
    x = -1;
    y = -1;
    color = UnitColor::unknown;
    name = ' ';
    blue = 0;
}
    
OpUnit::OpUnit(int x, int y, int color, char name){
    this->x = x;
    this->y = y;
    this->color = UnitColor(color);
    this->name = name;
    blue = 0;
}
    
OpUnit::OpUnit(int x, int y, char color, char name){
    this->x = x;
    this->y = y;
    this->color = UnitColor(color);
    this->name = name;
    blue = 0;
}