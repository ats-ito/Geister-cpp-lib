#include "Geister.hpp"
#include "unit.hpp"
#include <iostream>

std::vector<char> Geister::unitList = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

Geister::Geister(){
    this->takeBlue1st = 0;
    this->takeBlue2nd = 0;
    this->takeRed1st = 0;
    this->takeRed2nd = 0;
    this->turn = 0;
    units = {
        Unit(1, 4, 'U', 'A'),
        Unit(2, 4, 'U', 'B'),
        Unit(3, 4, 'U', 'C'),
        Unit(4, 4, 'U', 'D'),
        Unit(1, 5, 'U', 'E'),
        Unit(2, 5, 'U', 'F'),
        Unit(3, 5, 'U', 'G'),
        Unit(4, 5, 'U', 'H'),
        OpUnit(4, 1, 'u', 'a'),
        OpUnit(3, 1, 'u', 'b'),
        OpUnit(2, 1, 'u', 'c'),
        OpUnit(1, 1, 'u', 'd'),
        OpUnit(4, 0, 'u', 'e'),
        OpUnit(3, 0, 'u', 'f'),
        OpUnit(2, 0, 'u', 'g'),
        OpUnit(1, 0, 'u', 'h')
    };
}

Geister::Geister(const Geister& geister){
    this->takeBlue1st = 0;
    this->takeBlue2nd = 0;
    this->takeRed1st = 0;
    this->takeRed2nd = 0;
    this->turn = 0;
    units = geister.units;
}

Geister::Geister(std::string info){
    this->takeBlue1st = 0;
    this->takeBlue2nd = 0;
    this->takeRed1st = 0;
    this->takeRed2nd = 0;
    this->turn = 0;
    units = {
        Unit(info[0] - '0', info[1] - '0', info[2], 'A'),
        Unit(info[3] - '0', info[4] - '0', info[5], 'B'),
        Unit(info[6] - '0', info[7] - '0', info[8], 'C'),
        Unit(info[9] - '0', info[10] - '0', info[11], 'D'),
        Unit(info[12] - '0', info[13] - '0', info[14], 'E'),
        Unit(info[15] - '0', info[16] - '0', info[17], 'F'),
        Unit(info[18] - '0', info[19] - '0', info[20], 'G'),
        Unit(info[21] - '0', info[22] - '0', info[23], 'H'),
        OpUnit(info[24] - '0', info[25] - '0', info[26], 'a'),
        OpUnit(info[27] - '0', info[28] - '0', info[29], 'b'),
        OpUnit(info[30] - '0', info[31] - '0', info[32], 'c'),
        OpUnit(info[33] - '0', info[34] - '0', info[35], 'd'),
        OpUnit(info[36] - '0', info[37] - '0', info[38], 'e'),
        OpUnit(info[39] - '0', info[40] - '0', info[41], 'f'),
        OpUnit(info[42] - '0', info[43] - '0', info[44], 'g'),
        OpUnit(info[45] - '0', info[46] - '0', info[47], 'h')
    };
}

Geister::Geister(std::string red1, std::string red2){
    this->takeBlue1st = 0;
    this->takeBlue2nd = 0;
    this->takeRed1st = 0;
    this->takeRed2nd = 0;
    this->turn = 0;
    units = {
        Unit(1, 4, 'B', 'A'),
        Unit(2, 4, 'B', 'B'),
        Unit(3, 4, 'B', 'C'),
        Unit(4, 4, 'B', 'D'),
        Unit(1, 5, 'B', 'E'),
        Unit(2, 5, 'B', 'F'),
        Unit(3, 5, 'B', 'G'),
        Unit(4, 5, 'B', 'H'),
        OpUnit(4, 1, 'b', 'a'),
        OpUnit(3, 1, 'b', 'b'),
        OpUnit(2, 1, 'b', 'c'),
        OpUnit(1, 1, 'b', 'd'),
        OpUnit(4, 0, 'b', 'e'),
        OpUnit(3, 0, 'b', 'f'),
        OpUnit(2, 0, 'b', 'g'),
        OpUnit(1, 0, 'b', 'h')
    };
    for(int i = 0; i < red1.size(); ++i){
        units[red1[i] - 'A'].color = UnitColor::Red;
    }
    for(int i = 0; i < red2.size(); ++i){
        units[red2[i] - 'A' + 8].color = UnitColor::red;
    }
}

void Geister::setState(std::string state){
    if(auto beforeState = toString(); state != beforeState){
        int moveIndex = 0;
        Hand u;
        for(moveIndex = 0; moveIndex < 16; ++moveIndex){
            if(state[moveIndex*3] != beforeState[moveIndex*3] || state[moveIndex*3+1] != beforeState[moveIndex*3+1]){
                if(state[moveIndex*3] - beforeState[moveIndex*3] == 1){
                    u = Hand(units[moveIndex], Direction::East);
                }
                else if(state[moveIndex*3] - beforeState[moveIndex*3] == -1){
                    u = Hand(units[moveIndex], Direction::West);
                }
                else if(state[moveIndex*3+1] != beforeState[moveIndex*3+1] == 1){
                    u = Hand(units[moveIndex], Direction::South);
                }
                else if(state[moveIndex*3+1] != beforeState[moveIndex*3+1] == -1){
                    u = Hand(units[moveIndex], Direction::North);
                }
                else{
                    continue;
                }
                history.push_back({u, toString()});
                break;
            }
        }
    }
    for(int i = 0; i < 16; ++i){
        units[i].x = state[i * 3] - '0';
        units[i].y = state[i * 3 + 1] - '0';
        units[i].color = UnitColor(state[i * 3 + 2]);
        if(state[i * 3 + 1] - '0' > 5){
            if(units[i].color == UnitColor::Blue)
                takeBlue1st++;
            else if(units[i].color == UnitColor::Red)
                takeRed1st++;
            else if(units[i].color == UnitColor::blue)
                takeBlue2nd++;
            else if(units[i].color == UnitColor::red)
                takeRed2nd++;
            continue;
        }
    }
}

void Geister::initialize(){
    setState("14U24U34U44U15U25U35U45U41u31u21u11u40u30u20u10u");
}

void Geister::printAll() const
{
    printBoard();
    printInfo();
}

void Geister::printBoard() const
{
    std::cout << "2ndPlayer Take: ";
    for(int i = 0; i < 8; ++i){
        auto& u = units[i];
        if(u.isTaken()){
            if(u.color.isBlue())
                std::cout << "\e[34m";
            else if(u.color.isRed())
                std::cout << "\e[31m";
            std::cout << u.name;
            std::cout << "\e[0m" << ",";
        }
    }
    std::cout << std::endl;
    std::cout << "  0 1 2 3 4 5" << std::endl;
    for(int i = 0; i < 6; ++i){
        std::cout << i;
        for(int j = 0; j < 6; ++j){
            std::cout << " ";
            bool exist = false;
            for(auto&& u: units){
                if(u.x == j && u.y == i){
                    if(u.color.isBlue())
                        std::cout << "\e[34m";
                    else if(u.color.isRed())
                        std::cout << "\e[31m";
                    std::cout << u.name;
                    std::cout << "\e[0m";
                    exist = true;
                    break;
                }
            }
            if(!exist) std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "1stPlayer Take: ";
    for(int i = 8; i < 16; ++i){
        auto& u = units[i];
        if(u.isTaken()){
            if(u.color.isBlue())
                std::cout << "\e[34m";
            else if(u.color.isRed())
                std::cout << "\e[31m";
            std::cout << u.name;
            std::cout << "\e[0m" << ",";
        }
    }
    std::cout << std::endl;
    for(int i = 0; i < 16; ++i){
        auto& u = units[i];
        if(u.isEscape()){
            std::cout << "Escape: " << "\e[34m" << u.name << "\e[0m" << std::endl;
        }
    }
    std::cout << std::endl;
}

void Geister::printInfo() const
{
    for(int i = 0; i < 16; ++i){
        std::cout << units[i].name << "(" << units[i].color.toChar() << "): " << units[i].x << ", " << units[i].y << std::endl;
    }
}

std::array<Unit, 16>& Geister::allUnit(){
    return units;
}

bool Geister::canMove1st(Unit unit, Direction direct) const
{
    if(direct == Direction::North)
        unit.y -= 1;
    else if(direct == Direction::East){
        if(unit.x == 5)
            return unit.y == 0 && unit.color == UnitColor::Blue;
        unit.x += 1;
    }
    else if(direct == Direction::West){
        if(unit.x == 0)
            return unit.y == 0 && unit.color == UnitColor::Blue;
        unit.x -= 1;
    }
    else if(direct == Direction::South)
        unit.y += 1;
    for(auto&& u: units){
        if(u.x == unit.x && u.y == unit.y && u.color.is1st())
            return false;
    }
    return true;
}

bool Geister::canMove1st(Unit unit, char direct) const
{
    if(direct == 0)
        unit.y -= 1;
    else if(direct == 1){
        if(unit.x == 5)
            return unit.y == 0 && unit.color == UnitColor::Blue;
        unit.x += 1;
    }
    else if(direct == 2){
        if(unit.x == 0)
            return unit.y == 0 && unit.color == UnitColor::Blue;
        unit.x -= 1;
    }
    else if(direct == 3)
        unit.y += 1;
    for(auto&& u: units){
        if(u.x == unit.x && u.y == unit.y && u.color.is1st())
            return false;
    }
    return true;
}

std::vector<Hand> Geister::getLegalMove1st() const
{
    static std::vector<Hand> legalMoves;
    legalMoves.clear();
    Direction direct;
    int x;
    int y;
    UnitColor c;
    bool movable1st = true;
    for(int i = 0; i < 8; ++i){
        auto& unit = units[i]; 
        if(unit.onBoard()){
            x = unit.x;
            y = unit.y;
            c = unit.color;
            if(y > 0){
                movable1st = true;
                for(auto&& u: units){
                    if(u.x == x && u.y == y-1 && u.color.is1st()){
                        movable1st = false;
                        break;
                    }
                }
                if(movable1st)
                    legalMoves.push_back(Hand({unit, Direction::North}));
            }
            if(y < 5){
                movable1st = true;
                for(auto&& u: units){
                    if(u.x == x && u.y == y+1 && u.color.is1st()){
                        movable1st = false;
                        break;
                    }
                }
                if(movable1st)
                    legalMoves.push_back(Hand({unit, Direction::South}));
            }
            if(x > 0){
                movable1st = true;
                for(auto&& u: units){
                    if(u.x == x-1 && u.y == y && u.color.is1st()){
                        movable1st = false;
                        break;
                    }
                }
                if(movable1st)
                    legalMoves.push_back(Hand({unit, Direction::West}));
            }
            else if(y == 0 && c == UnitColor::Blue){
                legalMoves.push_back(Hand({unit, Direction::West}));
            }
            if(x < 5){
                movable1st = true;
                for(auto&& u: units){
                    if(u.x == x+1 && u.y == y && u.color.is1st()){
                        movable1st = false;
                        break;
                    }
                }
                if(movable1st)
                    legalMoves.push_back(Hand({unit, Direction::East}));
            }
            else if(y == 0 && c == UnitColor::Blue){
                legalMoves.push_back(Hand({unit, Direction::East}));
            }
        }
    }
    return legalMoves;
}

bool Geister::canMove2nd(Unit unit, char direct) const{
    if(direct == 0)
        unit.y -= 1;
    else if(direct == 1){
        if(unit.x == 5)
            return unit.y == 5 && unit.color == UnitColor::blue;
        unit.x += 1;
    }
    else if(direct == 2){
        if(unit.x == 0)
            return unit.y == 5 && unit.color == UnitColor::blue;
        unit.x -= 1;
    }
    else if(direct == 3)
        unit.y += 1;
    for(auto&& u: units){
        if(u.x == unit.x && u.y == unit.y && u.color.is2nd())
            return false;
    }
    return true;
}

std::vector<Hand> Geister::getLegalMove2nd() const
{
    static std::vector<Hand> legalMoves;
    legalMoves.clear();
    Direction direct;
    int x;
    int y;
    UnitColor c;
    bool movable2nd = true;
    for(int i = 8; i < 16; ++i){
        auto& unit = units[i]; 
        if(unit.onBoard()){
            x = unit.x;
            y = unit.y;
            c = unit.color;
            if(y > 0){
                movable2nd = true;
                for(auto&& u: units){
                    if(u.x == x && u.y == y-1 && u.color.is2nd()){
                        movable2nd = false;
                        break;
                    }
                }
                if(movable2nd)
                    legalMoves.push_back(Hand({unit, Direction::North}));
            }
            if(y < 5){
                movable2nd = true;
                for(auto&& u: units){
                    if(u.x == x && u.y == y+1 && u.color.is2nd()){
                        movable2nd = false;
                        break;
                    }
                }
                if(movable2nd)
                    legalMoves.push_back(Hand({unit, Direction::South}));
            }
            if(x > 0){
                movable2nd = true;
                for(auto&& u: units){
                    if(u.x == x-1 && u.y == y && u.color.is2nd()){
                        movable2nd = false;
                        break;
                    }
                }
                if(movable2nd)
                    legalMoves.push_back(Hand({unit, Direction::West}));
            }
            else if(y == 5 && c == UnitColor::blue){
                legalMoves.push_back(Hand({unit, Direction::West}));
            }
            if(x < 5){
                movable2nd = true;
                for(auto&& u: units){
                    if(u.x == x+1 && u.y == y && u.color.is2nd()){
                        movable2nd = false;
                        break;
                    }
                }
                if(movable2nd)
                    legalMoves.push_back(Hand({unit, Direction::East}));
            }
            else if(y == 5 && c == UnitColor::blue){
                legalMoves.push_back(Hand({unit, Direction::East}));
            }
        }
    }
    return legalMoves;
}

std::string& Geister::toString() const
{
	static std::string res = "14U24U34U44U15U25U35U45U41u31u21u11u40u30u20u10u";
	for(int i = 0; i < 16; ++i){
		res[i*3] = '0' + units[i].x;
		res[i*3 + 1] = '0' + units[i].y;
		res[i*3 + 2] = units[i].color.toChar();
	}
	return res;
}

void Geister::take(Unit unit){
    if(unit.color == UnitColor::Blue){
        takeBlue1st += 1;
        return;
    }
    else if(unit.color == UnitColor::blue){
        takeBlue2nd += 1;
        return;
    }
    else if(unit.color == UnitColor::Red){
        takeRed1st += 1;
        return;
    }
    else if(unit.color == UnitColor::red){
        takeRed2nd += 1;
        return;
    }
}

void Geister::move(char u, char direct){
    Unit* unit;
    if(u >= 'A' && u <= 'H'){
        unit = &units[u - 'A'];
        int x = unit->x;
        int y = unit->y;
        history.push_back({Hand(Unit(x, y, unit->color, u), Direction(direct)), toString()});
        if(direct == 'N'){
            Unit* target = getUnitByPos(x, y - 1);
            if(target && target->color.is2nd()){
                take(*target);
                target->x = 9;
                target->y = 9;
            }
            unit->x = x;
            unit->y = y - 1;
        }
        else if(direct == 'E'){
            if((unit->color == UnitColor::Blue && x == 5 && y == 0)){
                unit->x = 8;
                unit->y = 8;
                return;
            }
            Unit* target = getUnitByPos(x + 1, y);
            if(target && target->color.is2nd()){
                take(*target);
                target->x = 9;
                target->y = 9;
            }
            unit->x = x + 1;
            unit->y = y;
        }
        else if(direct == 'W'){
            if((unit->color == UnitColor::Blue && x == 0 && y == 0)){
                unit->x = 8;
                unit->y = 8;
                return;
            }
            Unit* target = getUnitByPos(x - 1, y);
            if(target && target->color.is2nd()){
                take(*target);
                target->x = 9;
                target->y = 9;
            }
            unit->x = x - 1;
            unit->y = y;
        }
        else if(direct == 'S'){
            Unit* target = getUnitByPos(x, y + 1);
            if(target && target->color.is2nd()){
                take(*target);
                target->x = 9;
                target->y = 9;
            }
            unit->x = x;
            unit->y = y + 1;
        }
        else return;
    }
    else if(u >= 'a' && u <= 'h'){
        unit = &units[u - 'a' + 8];
        int x = unit->x;
        int y = unit->y;
        history.push_back({Hand(Unit(x, y, unit->color, u), Direction(direct)), toString()});
        if(direct == 'N'){
            Unit* target = getUnitByPos(x, y - 1);
            if(target && target->color.is1st()){
                take(*target);
                target->x = 9;
                target->y = 9;
            }
            unit->x = x;
            unit->y = y - 1;
        }
        else if(direct == 'E'){
            if((unit->color == UnitColor::blue && x == 5 && y == 5)){
                unit->x = 8;
                unit->y = 8;
                return;
            }
            Unit* target = getUnitByPos(x + 1, y);
            if(target && target->color.is1st()){
                take(*target);
                target->x = 9;
                target->y = 9;
            }
            unit->x = x + 1;
            unit->y = y;
        }
        else if(direct == 'W'){
            if((unit->color == UnitColor::blue && x == 0 && y == 5)){
                unit->x = 8;
                unit->y = 8;
                return;
            }
            Unit* target = getUnitByPos(x - 1, y);
            if(target && target->color.is1st()){
                take(*target);
                target->x = 9;
                target->y = 9;
            }
            unit->x = x - 1;
            unit->y = y;
        }
        else if(direct == 'S'){
            Unit* target = getUnitByPos(x, y + 1);
            if(target && target->color.is1st()){
                take(*target);
                target->x = 9;
                target->y = 9;
            }
            unit->x = x;
            unit->y = y + 1;
        }
        else{
            return;
        }
    }
    else return;
    turn++;
}

double Geister::checkResult() const{
    // 駒取りによる勝敗
    if(takeBlue1st == 4)
		return -2;
	if(takeRed2nd == 4)
        return -3;
    if(takeBlue2nd == 4)
		return 2;
	if(takeRed1st == 4)
        return 3;

    // 駒の脱出による勝敗
    for(auto&& u: units)
        if(u.x == 8)
            return (u.color == UnitColor::Blue) ? 1 : -1;

    return 0;
}

Unit* Geister::getUnitByPos(int x, int y){
    for(int i = 0; i < 16; ++i){
        if(units[i].x == x && units[i].y == y){
            return &units[i];
        }
    }
    return nullptr;
}

Geister Geister::mask(){
	Geister res(*this);
	for(int i = 8; i < units.size(); ++i){
		auto& u = res.units[i];
		if(!u.onBoard())
			u.color = UnitColor::unknown;
	}
	return res;
}

void Geister::changeSide(){
    int tmp = takeBlue1st;
    takeBlue1st = takeBlue2nd;
    takeBlue2nd = tmp;
    tmp = takeRed1st;
    takeRed1st = takeRed2nd;
    takeRed2nd = tmp;

	for(int i = 0; i < 8; ++i){
		auto tmp = units[i];

		if(units[i+8].x > 5){
			units[i].x = units[i+8].x;
			units[i].y = units[i+8].y;
		}
		else{
			units[i].x = 5 - units[i+8].x;
			units[i].y = 5 - units[i+8].y;
		}
		units[i].name = units[i+8].name - 'a' + 'A';
		units[i].color = units[i+8].color.reverseSide();

		if(tmp.x > 5){
			units[i+8].x = tmp.x;
			units[i+8].y = tmp.y;
		}
		else{
			units[i+8].x = 5 - tmp.x;
			units[i+8].y = 5 - tmp.y;
		}
		units[i+8].name = tmp.name - 'A' + 'a';
		units[i+8].color = tmp.color.reverseSide();
	}
}
