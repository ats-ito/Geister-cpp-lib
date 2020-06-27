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