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
    Unit(1, 4, 'U', 'A'),
    Unit(2, 4, 'U', 'B'),
    Unit(3, 4, 'U', 'C'),
    Unit(4, 4, 'U', 'D'),
    Unit(1, 5, 'U', 'E'),
    Unit(2, 5, 'U', 'F'),
    Unit(3, 5, 'U', 'G'),
    Unit(4, 5, 'U', 'H'),
    Unit(4, 1, 'u', 'a'),
    Unit(3, 1, 'u', 'b'),
    Unit(2, 1, 'u', 'c'),
    Unit(1, 1, 'u', 'd'),
    Unit(4, 0, 'u', 'e'),
    Unit(3, 0, 'u', 'f'),
    Unit(2, 0, 'u', 'g'),
    Unit(1, 0, 'u', 'h')
}
{
}

Geister::Geister(const std::string& info):
mResult(Result::OnPlay),
mTurn(0),
units{
    Unit(info[0] - '0', info[1] - '0', info[2], 'A'),
    Unit(info[3] - '0', info[4] - '0', info[5], 'B'),
    Unit(info[6] - '0', info[7] - '0', info[8], 'C'),
    Unit(info[9] - '0', info[10] - '0', info[11], 'D'),
    Unit(info[12] - '0', info[13] - '0', info[14], 'E'),
    Unit(info[15] - '0', info[16] - '0', info[17], 'F'),
    Unit(info[18] - '0', info[19] - '0', info[20], 'G'),
    Unit(info[21] - '0', info[22] - '0', info[23], 'H'),
    Unit(info[24] - '0', info[25] - '0', info[26], 'a'),
    Unit(info[27] - '0', info[28] - '0', info[29], 'b'),
    Unit(info[30] - '0', info[31] - '0', info[32], 'c'),
    Unit(info[33] - '0', info[34] - '0', info[35], 'd'),
    Unit(info[36] - '0', info[37] - '0', info[38], 'e'),
    Unit(info[39] - '0', info[40] - '0', info[41], 'f'),
    Unit(info[42] - '0', info[43] - '0', info[44], 'g'),
    Unit(info[45] - '0', info[46] - '0', info[47], 'h')
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
mTurn(0),
units{
    Unit(1, 4, 'B', 'A'),
    Unit(2, 4, 'B', 'B'),
    Unit(3, 4, 'B', 'C'),
    Unit(4, 4, 'B', 'D'),
    Unit(1, 5, 'B', 'E'),
    Unit(2, 5, 'B', 'F'),
    Unit(3, 5, 'B', 'G'),
    Unit(4, 5, 'B', 'H'),
    Unit(4, 1, 'b', 'a'),
    Unit(3, 1, 'b', 'b'),
    Unit(2, 1, 'b', 'c'),
    Unit(1, 1, 'b', 'd'),
    Unit(4, 0, 'b', 'e'),
    Unit(3, 0, 'b', 'f'),
    Unit(2, 0, 'b', 'g'),
    Unit(1, 0, 'b', 'h')
}
{
    for(size_t i = 0; i < red1.size(); ++i){
        units[red1[i] - 'A'].color = UnitColor::Red;
    }
    for(size_t i = 0; i < red2.size(); ++i){
        units[red2[i] - 'A' + 8].color = UnitColor::red;
    }
    countTaken();
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
        units[i].x = state[i * 3] - '0';
        units[i].y = state[i * 3 + 1] - '0';
        units[i].color = UnitColor(state[i * 3 + 2]);
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
                units[c - 'A'].color = UnitColor::Red;
            else
                units[c - 'A'].color = UnitColor::Blue;
        }
    }
    if(!second.empty()){
        for(char c = 'A'; c <= 'H'; ++c){
            if(second.find(c) != std::string::npos)
                units[c - 'A'+8].color = UnitColor::red;
            else
                units[c - 'A'+8].color = UnitColor::blue;
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
        Unit(1, 4, 'U', 'A'),
        Unit(2, 4, 'U', 'B'),
        Unit(3, 4, 'U', 'C'),
        Unit(4, 4, 'U', 'D'),
        Unit(1, 5, 'U', 'E'),
        Unit(2, 5, 'U', 'F'),
        Unit(3, 5, 'U', 'G'),
        Unit(4, 5, 'U', 'H'),
        Unit(4, 1, 'u', 'a'),
        Unit(3, 1, 'u', 'b'),
        Unit(2, 1, 'u', 'c'),
        Unit(1, 1, 'u', 'd'),
        Unit(4, 0, 'u', 'e'),
        Unit(3, 0, 'u', 'f'),
        Unit(2, 0, 'u', 'g'),
        Unit(1, 0, 'u', 'h')
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
            for(const Unit& u: units){
                if(u.x == j && u.y == i){
                    if(u.isBlue() && u.isRed())
                        std::cout << "\e[35m";
                    else if(u.isBlue())
                        std::cout << "\e[34m";
                    else if(u.isRed())
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
        const Unit& u = units[i];
        if(u.isTaken()){
            if(u.isBlue() && u.isRed())
                std::cout << "\e[35m";
            else if(u.isBlue())
                std::cout << "\e[34m";
            else if(u.isRed())
                std::cout << "\e[31m";
            std::cout << u.name;
            std::cout << "\e[0m" << ",";
        }
    }
    std::cout << std::endl;
    for(int i = 0; i < 16; ++i){
        const Unit& u = units[i];
        if(u.isEscape()){
            std::cout << "Escape: " << "\e[34m" << u.name << "\e[0m" << std::endl;
        }
    }
    std::cout << std::endl;
}

bool Geister::canMove1st(const Unit& unit, const Direction direct) const
{
    int x = unit.x;
    int y = unit.y;
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
        if(u.x == x && u.y == y && u.is1st())
            return false;
    }
    return true;
}

bool Geister::canMove1st(const Unit& unit, const char direct) const
{
    int x = unit.x;
    int y = unit.y;
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
        if(u.x == x && u.y == y && u.is1st())
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
            const int x = unit.x;
            const int y = unit.y;
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
    int x = unit.x;
    int y = unit.y;
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
        if(u.x == x && u.y == y && u.color.is2nd())
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
            const int x = unit.x;
            const int y = unit.y;
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
    int x = hand.unit.x;
    int y = hand.unit.y;
    if(hand.unit.is1st()){
        Unit& unit = units[hand.unit.name - 'A'];
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
        unit.x = x;
        unit.y = y;
    }
    else 
    {
        Unit& unit = units[hand.unit.name - 'a' + 8];
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
        unit.x = x;
        unit.y = y;
    }
    if(++mTurn >= maxTurn && mResult == Result::OnPlay)
        mResult = Result::Draw;
}

Geister Geister::mask(){
    Geister res(*this);
    for(size_t i = 8; i < units.size(); ++i){
        Unit& u = res.units[i];
        if(u.onBoard())
            u.color = UnitColor::unknown;
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

Hand Geister::diff(const Geister& target){
    std::vector<Hand> res;
    for(size_t u = 0; u < 16; ++u){
        const Unit& unit = units[u];
        const Unit& targetUnit = target.units[u];
        if(unit.x != targetUnit.x || unit.y != targetUnit.y){
            if((targetUnit.y - unit.y) == -1){
                res.emplace_back(unit, Direction::North);
            }
            else if((targetUnit.x - unit.x) == 1){
                res.emplace_back(unit, Direction::East);
            }
            else if((targetUnit.x - unit.x) == -1){
                res.emplace_back(unit, Direction::West);
            }
            else if((targetUnit.y - unit.y) == 1){
                res.emplace_back(unit, Direction::South);
            }
            else{
                continue;
            }
        }
    }
    if (res.size() == 1)
    {
        return res[0];
    }
    else
        return Hand();
}

Hand diff(const Geister& left, const Geister& right){
    std::vector<Hand> res;
    for(size_t u = 0; u < 16; ++u){
        const Unit& leftUnit = left.allUnit()[u];
        const Unit& rightUnit = right.allUnit()[u];
        if(leftUnit.x != rightUnit.x || leftUnit.y != rightUnit.y){
            if((rightUnit.y - leftUnit.y) == -1){
                res.emplace_back(leftUnit, Direction::North);
            }
            else if((rightUnit.x - leftUnit.x) == 1){
                res.emplace_back(leftUnit, Direction::East);
            }
            else if((rightUnit.x - leftUnit.x) == -1){
                res.emplace_back(leftUnit, Direction::West);
            }
            else if((rightUnit.y - leftUnit.y) == 1){
                res.emplace_back(leftUnit, Direction::South);
            }
            else{
                continue;
            }
        }
    }
    if (res.size() == 1)
    {
        return res[0];
    }
    else
        return Hand();
}