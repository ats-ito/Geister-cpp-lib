#ifndef UNIT_HPP
#include "unit.hpp"
#endif

inline constexpr Unit::Unit():x{-1}, y{-1}, color{UnitColor::Unknown}, name{' '}{}
inline constexpr Unit::Unit(int x, int y, UnitColor color, char name): x{x}, y{y}, color{color}, name{name}{}

inline std::string Unit::toString() const{
	return std::string{name} + std::string{color.toChar()} + std::to_string(x) + std::to_string(y);
}

inline bool Unit::onBoard() const
{
	return 0 <= x && x <= 5 && 0 <= y && y <= 5;
}

inline bool Unit::isTaken() const
{
	return x == 9 && y == 9;
}

inline bool Unit::isEscape() const
{
	return x == 8 && y == 8;
}

inline bool Unit::is1st() const
{
	return color.is1st();
}
inline bool Unit::is2nd() const
{
	return color.is2nd();
}

inline bool Unit::isBlue() const
{
	return color.isBlue();
}
inline bool Unit::isRed() const
{
	return color.isRed();
}

inline void Unit::reverseSide(){
	if(x <= 5){
		x = 5 - x;
		y = 5 - y;
	}
	if(is1st()){
		name = std::tolower(name);
	}
	else{
		name = std::toupper(name);
	}
	color = color.reverseSide();
}



inline constexpr UnitColor::UnitColor() : color(NotDefine) {}
inline constexpr UnitColor::UnitColor(Color color) : color(color) {}
inline UnitColor::UnitColor(char c) {
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


inline bool UnitColor::operator==(const UnitColor& c) const { return color == c.color; }
inline bool UnitColor::operator!=(const UnitColor& c) const { return color != c.color; }

inline bool UnitColor::isBlue() const
{
	return static_cast<unsigned char>(color) & 0b010;
}
inline bool UnitColor::isRed() const
{
	return static_cast<unsigned char>(color) & 0b100;
}
inline bool UnitColor::is1st() const
{
	return !(static_cast<unsigned char>(color) & 0b1);
}
inline bool UnitColor::is2nd() const
{
	return static_cast<unsigned char>(color) & 0b1;
}

inline char UnitColor::toChar() const
{
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

inline UnitColor UnitColor::reverseSide() const
{
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