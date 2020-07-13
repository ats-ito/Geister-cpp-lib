#ifndef UNIT_HPP
#include "unit.hpp"
#endif
#include <cassert>

constexpr uint8_t name2id(const char name){
	assert(('A' <= name && name <= 'H') || ('a' <= name && name <= 'h'));
	switch (name)
	{
	case 'A': return 0;
	case 'B': return 1;
	case 'C': return 2;
	case 'D': return 3;
	case 'E': return 4;
	case 'F': return 5;
	case 'G': return 6;
	case 'H': return 7;
	case 'a': return 8;
	case 'b': return 9;
	case 'c': return 10;
	case 'd': return 11;
	case 'e': return 12;
	case 'f': return 13;
	case 'g': return 14;
	case 'h': return 15;
	default: return 16;
	}
}

inline constexpr Unit::Unit(int8_t x, int8_t y, UnitColor color, uint8_t id): mX{x}, mY{y}, mColor{color}, mID{id}{}

inline std::string Unit::toString() const{
	return std::string{nameList[mID]} + std::string{mColor.toChar()} + std::to_string(mX) + std::to_string(mY);
}

inline const int Unit::x() const
{
	return mX;
}
inline const int Unit::y() const
{
	return mY;
}
inline const UnitColor& Unit::color() const
{
	return mColor;
}
inline const char Unit::name() const
{
	return nameList[mID];
}
inline const int Unit::id() const
{
	return mID;
}

inline void Unit::setColor(UnitColor c){
	mColor = c;
}

inline void Unit::setPos(int x, int y){
	mX = x;
	mY = y;
}

inline bool Unit::onBoard() const
{
	return 0 <= mX && mX <= 5 && 0 <= mY && mY <= 5;
}

inline bool Unit::isTaken() const
{
	return mX == 9 && mY == 9;
}

inline bool Unit::isEscape() const
{
	return mX == 8 && mY == 8;
}

inline bool Unit::is1st() const
{
	return mColor.is1st();
}
inline bool Unit::is2nd() const
{
	return mColor.is2nd();
}

inline bool Unit::isBlue() const
{
	return mColor.isBlue();
}
inline bool Unit::isRed() const
{
	return mColor.isRed();
}

inline void Unit::reverseSide(){
	if(mX <= 5){
		mX = 5 - mX;
		mY = 5 - mY;
	}
	mID ^= 0b1000;
	mColor = mColor.reverseSide();
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