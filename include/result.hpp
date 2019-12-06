#ifndef RESULT_HPP
#define RESULT_HPP

#include <string>
#include <ostream>

enum struct Result{
    OnPlay = 0,
    Escape1st,
    TakeBlue1st,
    TakenRed1st,
    TakenRed2nd = -3,
    TakeBlue2nd,
    Escape2nd,
    Draw = 7
};

inline std::ostream& operator<<(std::ostream& stream, const Result& result){
    std::string str;
    if(result == Result::OnPlay)
        str = "OnPlay";
    else if(result == Result::Escape1st)
        str = "Escape_1st";
    else if(result == Result::Escape2nd)
        str = "Escape_2nd";
    else if(result == Result::TakeBlue1st)
        str = "TakeBlue_1st";
    else if(result == Result::TakeBlue2nd)
        str = "TakeBlue_2nd";
    else if(result == Result::TakenRed1st)
        str = "TakenRed_1st";
    else if(result == Result::TakenRed2nd)
        str = "TakenRed_2nd";
    else if(result == Result::Draw)
        str = "Draw";
    return stream << str;
}

#endif