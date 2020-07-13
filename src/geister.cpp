#include "geister.hpp"
#include <iostream>

Geister::Geister():
mResult(Result::OnPlay),
takenBlue1st(0),
takenBlue2nd(0),
takenRed1st(0),
takenRed2nd(0),
mTurn(0),
units{
    Unit(1, 4, 'U', 0x0),
    Unit(2, 4, 'U', 0x1),
    Unit(3, 4, 'U', 0x2),
    Unit(4, 4, 'U', 0x3),
    Unit(1, 5, 'U', 0x4),
    Unit(2, 5, 'U', 0x5),
    Unit(3, 5, 'U', 0x6),
    Unit(4, 5, 'U', 0x7),
    Unit(4, 1, 'u', 0x8),
    Unit(3, 1, 'u', 0x9),
    Unit(2, 1, 'u', 0xA),
    Unit(1, 1, 'u', 0xB),
    Unit(4, 0, 'u', 0xC),
    Unit(3, 0, 'u', 0xD),
    Unit(2, 0, 'u', 0XE),
    Unit(1, 0, 'u', 0xF)
}
{
}

Geister::Geister(const std::string& info):
mResult(Result::OnPlay),
mTurn(0),
units{
    Unit(info[0] - '0', info[1] - '0', info[2], 0x0),
    Unit(info[3] - '0', info[4] - '0', info[5], 0x1),
    Unit(info[6] - '0', info[7] - '0', info[8], 0x2),
    Unit(info[9] - '0', info[10] - '0', info[11], 0x3),
    Unit(info[12] - '0', info[13] - '0', info[14], 0x4),
    Unit(info[15] - '0', info[16] - '0', info[17], 0x5),
    Unit(info[18] - '0', info[19] - '0', info[20], 0x6),
    Unit(info[21] - '0', info[22] - '0', info[23], 0x7),
    Unit(info[24] - '0', info[25] - '0', info[26], 0x8),
    Unit(info[27] - '0', info[28] - '0', info[29], 0x9),
    Unit(info[30] - '0', info[31] - '0', info[32], 0xA),
    Unit(info[33] - '0', info[34] - '0', info[35], 0xB),
    Unit(info[36] - '0', info[37] - '0', info[38], 0xC),
    Unit(info[39] - '0', info[40] - '0', info[41], 0xD),
    Unit(info[42] - '0', info[43] - '0', info[44], 0xE),
    Unit(info[45] - '0', info[46] - '0', info[47], 0xF)
}
{
    for(const Unit& u: units){
        if(u.isEscape()){
            mResult = u.is1st() ? Result::Escape1st : Result::Escape2nd;
        }
    }
    countTaken();
}

Geister::Geister(const std::string& red1, const std::string& red2):
mResult(Result::OnPlay),
takenBlue1st(0),
takenBlue2nd(0),
takenRed1st(0),
takenRed2nd(0),
mTurn(0),
units{
    Unit(1, 4, 'B', 0x0),
    Unit(2, 4, 'B', 0x1),
    Unit(3, 4, 'B', 0x2),
    Unit(4, 4, 'B', 0x3),
    Unit(1, 5, 'B', 0x4),
    Unit(2, 5, 'B', 0x5),
    Unit(3, 5, 'B', 0x6),
    Unit(4, 5, 'B', 0x7),
    Unit(4, 1, 'b', 0x8),
    Unit(3, 1, 'b', 0x9),
    Unit(2, 1, 'b', 0xA),
    Unit(1, 1, 'b', 0xB),
    Unit(4, 0, 'b', 0xC),
    Unit(3, 0, 'b', 0xD),
    Unit(2, 0, 'b', 0xE),
    Unit(1, 0, 'b', 0xF)
}
{
    for(size_t i = 0; i < red1.size(); ++i){
        units[red1[i] - 'A'].setColor(UnitColor::Red);
    }
    for(size_t i = 0; i < red2.size(); ++i){
        units[red2[i] - 'A' + 8].setColor(UnitColor::red);
    }
}

Geister::Geister(const Geister& game, const std::string& red1, const std::string& red2):
mResult(game.mResult),
mTurn(game.mTurn),
units(game.units)
{
    setColor(red1, red2);
}

void Geister::setState(const std::string& state){
    for(int i = 0; i < 16; ++i){
        units[i].setPos(state[i * 3] - '0', state[i * 3 + 1] - '0');
        units[i].setColor(UnitColor(state[i * 3 + 2]));
    }
    for(const Unit& u: units){
        if(u.isEscape()){
            if(u.is1st())
                mResult = Result::Escape1st;
            else if(u.is2nd())
                mResult = Result::Escape2nd;
        }
    }
    countTaken();
}

void Geister::setColor(const std::string& first, const std::string& second){
    if(!first.empty()){
        for(char c = 'A'; c <= 'H'; ++c){
            if(first.find(c) != std::string::npos)
                units[c - 'A'].setColor(UnitColor::Red);
            else
                units[c - 'A'].setColor(UnitColor::Blue);
        }
    }
    if(!second.empty()){
        for(char c = 'A'; c <= 'H'; ++c){
            if(second.find(c) != std::string::npos)
                units[c - 'A'+8].setColor(UnitColor::red);
            else
                units[c - 'A'+8].setColor(UnitColor::blue);
        }
    }
    countTaken();
}

void Geister::initialize(){
    mResult = Result::OnPlay;
    takenBlue1st = 0;
    takenBlue2nd = 0;
    takenRed1st = 0;
    takenRed2nd = 0;
    mTurn = 0;
    // mHistory.clear();
    units = {
        Unit(1, 4, 'U', 0x0),
        Unit(2, 4, 'U', 0x1),
        Unit(3, 4, 'U', 0x2),
        Unit(4, 4, 'U', 0x3),
        Unit(1, 5, 'U', 0x4),
        Unit(2, 5, 'U', 0x5),
        Unit(3, 5, 'U', 0x6),
        Unit(4, 5, 'U', 0x7),
        Unit(4, 1, 'u', 0x8),
        Unit(3, 1, 'u', 0x9),
        Unit(2, 1, 'u', 0xA),
        Unit(1, 1, 'u', 0xB),
        Unit(4, 0, 'u', 0xC),
        Unit(3, 0, 'u', 0xD),
        Unit(2, 0, 'u', 0xE),
        Unit(1, 0, 'u', 0xF)
    };
}

void Geister::printBoard() const
{
    std::cout << "2ndPlayer Take: ";
    for(int i = 0; i < 8; ++i){
        const Unit& u = units[i];
        if(u.isTaken()){
            if(u.isBlue() && u.isRed())
                std::cout << "\e[35m";
            else if(u.isBlue())
                std::cout << "\e[34m";
            else if(u.isRed())
                std::cout << "\e[31m";
            std::cout << u.name();
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
            for(const Unit& u: units){
                if(u.x() == j && u.y() == i){
                    if(u.isBlue() && u.isRed())
                        std::cout << "\e[35m";
                    else if(u.isBlue())
                        std::cout << "\e[34m";
                    else if(u.isRed())
                        std::cout << "\e[31m";
                    std::cout << u.name();
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
        const Unit& u = units[i];
        if(u.isTaken()){
            if(u.isBlue() && u.isRed())
                std::cout << "\e[35m";
            else if(u.isBlue())
                std::cout << "\e[34m";
            else if(u.isRed())
                std::cout << "\e[31m";
            std::cout << u.name();
            std::cout << "\e[0m" << ",";
        }
    }
    std::cout << std::endl;
    for(int i = 0; i < 16; ++i){
        const Unit& u = units[i];
        if(u.isEscape()){
            std::cout << "Escape: " << "\e[34m" << u.name() << "\e[0m" << std::endl;
        }
    }
    std::cout << std::endl;
}

bool Geister::canMove1st(const Unit& unit, const Direction direct) const
{
    int x = unit.x();
    int y = unit.y();
    if(!unit.is1st())
        return false;
    if(direct == Direction::North)
        y -= 1;
    else if(direct == Direction::East){
        if(x == 5)
            return y == 0 && unit.isBlue();
        x += 1;
    }
    else if(direct == Direction::West){
        if(x == 0)
            return y == 0 && unit.isBlue();
        x -= 1;
    }
    else if(direct == Direction::South)
        y += 1;
    for(const Unit& u: units){
        if(u.x() == x && u.y() == y && u.is1st())
            return false;
    }
    return true;
}

bool Geister::canMove1st(const Unit& unit, const char direct) const
{
    int x = unit.x();
    int y = unit.y();
    if(!unit.is1st())
        return false;
    if(direct == 0)
        y -= 1;
    else if(direct == 1){
        if(x == 5)
            return y == 0 && unit.isBlue();
        x += 1;
    }
    else if(direct == 2){
        if(x == 0)
            return y == 0 && unit.isBlue();
        x -= 1;
    }
    else if(direct == 3)
        y += 1;
    for(const Unit& u: units){
        if(u.x() == x && u.y() == y && u.is1st())
            return false;
    }
    return true;
}

std::vector<Hand>& Geister::getLegalMove1st() const
{
    static std::vector<Hand> legalMoves;
    legalMoves.clear();
    for(int i = 0; i < 8; ++i){
        const Unit& unit = units[i];
        if(unit.onBoard()){
            const int x = unit.x();
            const int y = unit.y();
            if(y > 0 && !exist1st(x, y-1)){
                legalMoves.emplace_back(unit, Direction::North);
            }
            if(y < 5 && !exist1st(x, y+1)){
                legalMoves.emplace_back(unit, Direction::South);
            }
            if(!exist1st(x-1, y) && (x > 0 || (y == 0 && unit.isBlue()))){
                legalMoves.emplace_back(unit, Direction::West);
            }
            if(!exist1st(x+1, y) && (x < 5 || (y == 0 && unit.isBlue()))){
                legalMoves.emplace_back(unit, Direction::East);
            }
        }
    }
    return legalMoves;
}

bool Geister::canMove2nd(const Unit& unit, const char direct) const{
    int x = unit.x();
    int y = unit.y();
    if(!unit.is2nd())
        return false;
    if(direct == 0)
        y -= 1;
    else if(direct == 1){
        if(x == 5)
            return y == 5 && unit.isBlue();
        x += 1;
    }
    else if(direct == 2){
        if(x == 0)
            return y == 5 && unit.isBlue();
        x -= 1;
    }
    else if(direct == 3)
        y += 1;
    for(const Unit& u: units){
        if(u.x() == x && u.y() == y && u.color().is2nd())
            return false;
    }
    return true;
}

std::vector<Hand>& Geister::getLegalMove2nd() const
{
    static std::vector<Hand> legalMoves;
    legalMoves.clear();
    for(int i = 8; i < 16; ++i){
        const Unit& unit = units[i];
        if(unit.onBoard()){
            const int x = unit.x();
            const int y = unit.y();
            if(y > 0 && !exist2nd(x, y-1)){
                legalMoves.emplace_back(unit, Direction::North);
            }
            if(y < 5 && !exist2nd(x, y+1)){
                legalMoves.emplace_back(unit, Direction::South);
            }
            if(!exist2nd(x-1, y) && (x > 0 || (y == 5 && unit.isBlue()))){
                legalMoves.emplace_back(unit, Direction::West);
            }
            if(!exist2nd(x+1, y) && (x < 5 || (y == 5 && unit.isBlue()))){
                legalMoves.emplace_back(unit, Direction::East);
            }
        }
    }
    return legalMoves;
}

void Geister::move(const Hand& hand){
    Unit& unit = units[hand.unit.id()];
    int x = hand.unit.x();
    int y = hand.unit.y();
    if(hand.unit.is1st()){
        if(hand.direct == Direction::North){
            --y;
        }
        else if(hand.direct == Direction::East){
            if(x == 5){
                escape(unit);
                return;
            }
            ++x;
        }
        else if(hand.direct == Direction::West){
            if(x == 0){
                escape(unit);
                return;
            }
            --x;
        }
        else if(hand.direct == Direction::South){
            ++y;
        }
        else return;

        if(Unit* target = getUnitByPos(x, y); target){
            take(*target);
        }
        unit.setPos(x, y);
    }
    else 
    {
        if(hand.direct == Direction::North){
            --y;
        }
        else if(hand.direct == Direction::East){
            if(x == 5){
                escape(unit);
                return;
            }
            ++x;
        }
        else if(hand.direct == Direction::West){
            if(x == 0){
                escape(unit);
                return;
            }
            --x;
        }
        else if(hand.direct == Direction::South){
            ++y;
        }
        else return;

        if(Unit* target = getUnitByPos(x, y); target){
            take(*target);
        }
        unit.setPos(x, y);
    }
    if(++mTurn >= maxTurn && mResult == Result::OnPlay)
        mResult = Result::Draw;
}

Geister Geister::mask(){
    Geister res(*this);
    for(size_t i = 8; i < units.size(); ++i){
        Unit& u = res.units[i];
        if(u.onBoard())
            u.setColor(UnitColor::unknown);
    }
    return res;
}

void Geister::changeSide(){
    std::swap(takenBlue1st, takenBlue2nd);
    std::swap(takenRed1st, takenRed2nd);

    for(int i = 0; i < 8; ++i){
        units[i].reverseSide();
        units[i+8].reverseSide();
        std::iter_swap(units.begin()+i, units.begin()+8+i);
    }

    if(mResult != Result::Draw)
        mResult = static_cast<Result>(-static_cast<int>(mResult));
}