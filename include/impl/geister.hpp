#ifndef GEISTER_HPP
#include "geister.hpp"
#endif
inline void Geister::setColor(const char unit, const UnitColor color){
	if('A' <= unit && unit <= 'H'){
		units[unit-'A'].setColor(color);
	}
	else if('a' <= unit && unit <= 'h'){
		units[unit-'a'+8].setColor(color);
	}
	countTaken();
}

inline void Geister::printInfo() const
{
	for(int i = 0; i < 16; ++i){
		std::cout << units[i].name() << "(" << units[i].color().toChar() << "): " << units[i].x() << ", " << units[i].y() << std::endl;
	}
}

inline void Geister::printAll() const
{
	printBoard();
	printInfo();
}

inline const std::array<Unit, 16>& Geister::allUnit() const noexcept
{
	return units;
}

inline std::string& Geister::toString() const
{
	static std::string res = "14U24U34U44U15U25U35U45U41u31u21u11u40u30u20u10u";
	for(int i = 0; i < 16; ++i){
		res[i*3] = '0' + units[i].x();
		res[i*3 + 1] = '0' + units[i].y();
		res[i*3 + 2] = units[i].color().toChar();
	}
	return res;
}
inline Geister::operator std::string() const { return toString(); }


inline void Geister::escape(Unit& unit){
	unit.setPos(8, 8);
	mResult = unit.is1st() ? Result::Escape1st : Result::Escape2nd;
}

inline Result Geister::result() const noexcept
{
	return mResult;
}

inline bool Geister::isEnd() const noexcept
{
	return mResult != Result::OnPlay;
}

inline Unit* Geister::getUnitByPos(const int x, const int y){
	for(int i = 0; i < 16; ++i){
		if(units[i].x() == x && units[i].y() == y){
			return &units[i];
		}
	}
	return nullptr;
}

inline const Unit* Geister::getUnitByPos(const int x, const int y)const{
	for(int i = 0; i < 16; ++i){
		if(units[i].x() == x && units[i].y() == y){
			return &units[i];
		}
	}
	return nullptr;
}

inline void Geister::countTaken(){
	if((takenBlue1st = std::count_if(units.begin(), units.begin()+8,
		[&](Unit& u){ return u.isTaken() && u.isBlue(); })) == 4)
		mResult = Result::TakeBlue2nd;
	if((takenRed1st = std::count_if(units.begin(), units.begin()+8,
		[&](Unit& u){ return u.isTaken() && u.isRed(); })) == 4)
		mResult = Result::TakenRed1st;
	if((takenBlue2nd = std::count_if(units.begin()+8, units.end(),
		[&](Unit& u){ return u.isTaken() && u.isBlue(); })) == 4)
		mResult = Result::TakeBlue1st;
	if((takenRed2nd = std::count_if(units.begin()+8, units.end(),
		[&](Unit& u){ return u.isTaken() && u.isRed(); })) == 4)
		mResult = Result::TakenRed2nd;
}

inline int Geister::takenCount(const UnitColor& c) const noexcept{
	if(c == UnitColor::Blue)
		return takenBlue1st;
	if(c == UnitColor::Red)
		return takenRed1st;
	if(c == UnitColor::blue)
		return takenBlue2nd;
	if(c == UnitColor::red)
		return takenRed2nd;
	return -1;
}

inline void Geister::take(Unit& unit){
	unit.setPos(9, 9);
	if(unit.is1st()){
		if(unit.isRed()){
			if(++takenRed1st == 4)
				mResult = Result::TakenRed1st;
			return;
		}
		else if(unit.isBlue()){
			if(++takenBlue1st == 4)
				mResult = Result::TakeBlue2nd;
			return;
		}
	}
	else if(unit.is2nd()){
		if(unit.isRed()){
			if(++takenRed2nd == 4)
				mResult = Result::TakenRed2nd;
			return;
		}
		else if(unit.isBlue()){
			if(++takenBlue2nd == 4)
				mResult = Result::TakeBlue1st;
			return;
		}
	}
}

inline bool Geister::exist1st(const int x, const int y)const{
	for(int i = 0; i < 8; ++i){
		const Unit& u = units[i];
		if(u.x() == x && u.y() == y){
			return true;
		}
	}
	return false;
}
inline bool Geister::exist2nd(const int x, const int y)const{
	for(int i = 8; i < 16; ++i){
		const Unit& u = units[i];
		if(u.x() == x && u.y() == y){
			return true;
		}
	}
	return false;
}

inline int Geister::turn() const noexcept
{
	return mTurn;
}
