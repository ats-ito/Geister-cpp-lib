#include "nonsugar.hpp"
#include "geister.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "random.hpp"
#include "hand.hpp"
#include "result.hpp"
#ifdef USE_FS
#ifdef _WIN32
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#endif
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef USE_FS
#ifdef _WIN32
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif
#endif

using namespace nonsugar;

static std::string getFileName(std::string path){
    int last = 0;
    for(size_t i = 0; i < path.size(); ++i){
        if(path[i] == '/' || path[i] == '\\'){
            last = i+1;
        }
    }
    return std::string(&path[last]);
}


bool logEnable = false;
std::string logRoot;
std::string dllPath1, dllPath2;
std::string dllName1, dllName2;
#ifdef USE_FS
std::string logDir;
std::ofstream digestFile;
#endif
int outputLevel = 2;
int mask = 0;

using T = std::string (*)(std::string);
using T2 = std::string (*)();

#ifdef _WIN32
int run(HMODULE dll1, HMODULE dll2){
#else
int run(void* dll1, void* dll2){
#endif
#ifdef _WIN32
    T decideHand1=(T)GetProcAddress(dll1, "decideHand");
    T decideHand2=(T)GetProcAddress(dll2, "decideHand");
    T2 decideRed1=(T2)GetProcAddress(dll1, "decideRed");
    T2 decideRed2=(T2)GetProcAddress(dll2, "decideRed");
#else
    T decideHand1=(T)dlsym(dll1, "decideHand");
    T decideHand2=(T)dlsym(dll2, "decideHand");
    T2 decideRed1=(T2)dlsym(dll1, "decideRed");
    T2 decideRed2=(T2)dlsym(dll2, "decideRed");
#endif
    if(!decideHand1 || !decideHand2){
        std::cerr << "cant call decideHand" << std::endl;
        exit(1);
    }

#ifdef USE_FS
    std::ofstream logFile;
    if(logEnable){
        //時刻取得用
        char fn[256];

        //現在時刻取得
        time_t now = time(NULL);
        struct tm *pnow = localtime(&now);
        sprintf(fn, "%04d-%02d-%02d_%02d-%02d-%02d", pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday,
            pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
        std::string filename(fn);

        constexpr const char* ext = ".txt";
        std::string fp = logDir + "/" + dllName1 + "-" + dllName2 + "_" + filename + ".0" + ext;
        fs::path filepath(fp);
        for(int i=1; fs::exists(filepath); ++i){
            filepath = fs::path(logDir + "/" + dllName1 + "-" + dllName2 + "_" + filename + "." + std::to_string(i) + ext);
        }
        
        logFile.open(filepath, std::ios::out);
    }
#endif
    cpprefjp::random_device rd;
    std::mt19937 mt(rd());

    const std::vector<std::string> pattern = {
        "ABCD", "ABCE", "ABCF", "ABCG", "ABCH", "ABDE", "ABDF",
        "ABDG", "ABDH", "ABEF", "ABEG", "ABEH", "ABFG", "ABFH",
        "ABGH", "ACDE", "ACDF", "ACDG", "ACDH", "ACEF", "ACEG",
        "ACEH", "ACFG", "ACFH", "ACGH", "ADEF", "ADEG", "ADEH",
        "ADFG", "ADFH", "ADGH", "AEFG", "AEFH", "AEGH", "AFGH",
        "BCDE", "BCDF", "BCDG", "BCDH", "BCEF", "BCEG", "BCEH",
        "BCFG", "BCFH", "BCGH", "BDEF", "BDEG", "BDEH", "BDFG",
        "BDFH", "BDGH", "BEFG", "BEFH", "BEGH", "BFGH", "CDEF",
        "CDEG", "CDEH", "CDFG", "CDFH", "CDGH", "CEFG", "CEFH",
        "CEGH", "CFGH", "DEFG", "DEFH", "DEGH", "DFGH", "EFGH"
    };
    
    int turn = 0;

    std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
    std::string red_ptn1;
    std::string red_ptn2;
    
    if(!decideRed1 || !decideRed2){
        red_ptn1 = pattern[serector(mt)];
        red_ptn2 = pattern[serector(mt)];
    }
    else{
        red_ptn1 = decideRed1();
        red_ptn2 = decideRed2();
    }
    
    Geister game(red_ptn1, red_ptn2);
    if(outputLevel > 1){
        if(!(mask & 0b01))
            std::cout << "1stPlayerSet: " << red_ptn1 << std::endl;
        if(!(mask & 0b10))
            std::cout << "2ndPlayerSet: " << red_ptn2 << std::endl;
    }
#ifdef USE_FS
    if(logEnable){
        logFile << "Set," << "1," << red_ptn1 << "," << "14U24U34U44U15U25U35U45U41u31u21u11u40u30u20u10u" << std::endl;
        logFile << "Set," << "2," << red_ptn2 << "," << game.mask() << std::endl;
    }
#endif
    if(outputLevel > 2){
        if(mask == 0)
            game.printBoard();
        else if(mask == 1){
            game.changeSide();
            Geister g = game.mask();
            g.changeSide();
            g.printBoard();
            game.changeSide();
        }
        else if(mask == 2){
            game.mask().printBoard();
        }
        else if(mask == 3){
            game.changeSide();
            Geister g = game.mask();
            g.changeSide();
            g.mask().printBoard();
            game.changeSide();
        }
    }

    Result result = Result::OnPlay;

    while(!game.isEnd()){
        if(game.turn >= 200) break;
        Hand hand = Hand(decideHand1(game.mask()));
        if(outputLevel > 1){
            std::cout << "1stPlayer: " << hand.unit.name << " " << hand.direct.toChar() << '\t' << game << std::endl;
        }
#ifdef USE_FS
        if(logEnable){
            logFile << "Move," << "1," << hand.unit.name << "," << hand.direct.toChar() << "," << game << std::endl;
        }
#endif
        game.move(hand);
        if(outputLevel > 2){
            if(mask == 0)
                game.printBoard();
            else if(mask == 1){
                game.changeSide();
                Geister g = game.mask();
                g.changeSide();
                g.printBoard();
                game.changeSide();
            }
            else if(mask == 2){
                game.mask().printBoard();
            }
            else if(mask == 3){
                game.changeSide();
                Geister g = game.mask();
                g.changeSide();
                g.mask().printBoard();
                game.changeSide();
            }
        }
        result = game.getResult();
        if(game.isEnd())
            break;
        game.changeSide();
        hand = Hand(decideHand2(game.mask()));
        game.changeSide();
        if(outputLevel > 1){
            std::cout << "2ndPlayer: " << hand.unit.name << " " << hand.direct.toChar() << '\t' << game << std::endl;
        }
#ifdef USE_FS
        if(logEnable){
            logFile << "Move," << "2," << hand.unit.name << "," << hand.direct.toChar() << "," << game << std::endl;
        }
#endif
        game.changeSide();
        game.move(hand);
        game.changeSide();
        if(outputLevel > 2){
            if(mask == 0)
                game.printBoard();
            else if(mask == 1){
                game.changeSide();
                Geister g = game.mask();
                g.changeSide();
                g.printBoard();
                game.changeSide();
            }
            else if(mask == 2){
                game.mask().printBoard();
            }
            else if(mask == 3){
                game.changeSide();
                Geister g = game.mask();
                g.changeSide();
                g.mask().printBoard();
                game.changeSide();
            }
        }
        result = game.getResult();
    }
    // game.turn++;
    if(outputLevel > 0){
        std::cout << result << ": " << game.turn << '\t' << game << std::endl;
    }
#ifdef USE_FS
    if(logEnable){
        logFile << "Result," << result << "," << game << std::endl;
        logFile << "Turn," << game.turn << std::endl;
        digestFile << result << "," << game.turn << std::endl;
    }
#endif
    return result == Result::Draw ? 0 : static_cast<int>(result);
}

int main(int argc, char** argv){
    std::cin.tie(0);
    std::ios::sync_with_stdio(false);
    
    double match = 1;
    try {
        auto const cmd = command<char>("competition", "geister competition")
            .flag<'h'>({'h'}, {"help"}, "produce help message")
            .flag<'v'>({'v'}, {"version"}, "print version string")
            .flag<'l'>({'l'}, {"log"}, "enable log record")
            .flag<'d', std::string>({'d'}, {"dest"}, "N", "log destination")
            .flag<'o', int>({'o'}, {"output"}, "N", "output level")
            .flag<'c', int>({'c'}, {"match"}, "N", "match count")
            .flag<'m', int>({'m'}, {"mask"}, "N", "mask player number")
            .argument<'p', std::vector<std::string>>("Player-Path")
            ;
        auto const opts = parse(argc, argv, cmd, argument_order::flexible);
        if (opts.has<'h'>()) {
            std::cout << usage(cmd);
            return 0;
        }
        if (opts.has<'v'>()) {
            std::cout << "competition, version 1.0\n";
            return 0;
        }
        if (opts.has<'l'>()) {
            logEnable = true;
        }
        if (opts.has<'d'>()){
            logEnable = true;
            logRoot = opts.get<'d'>();
        }
        if (opts.has<'o'>()) {
            outputLevel = opts.get<'o'>();
        }
        if (opts.has<'c'>()) {
            match = opts.get<'c'>();
        }
        if (opts.has<'m'>()) {
            mask = opts.get<'m'>();
        }
        if(opts.get<'p'>().size() > 0){
            dllPath1 = opts.get<'p'>()[0];
            dllPath2 = opts.get<'p'>()[1];
#ifdef USE_FS
            dllName1 = fs::path(dllPath1).filename().generic_string();
            dllName2 = fs::path(dllPath2).filename().generic_string();
#else
            dllName1 = getFileName(dllPath1);
            dllName2 = getFileName(dllPath2);
#endif
        }
        else{
            std::cerr << usage(cmd);
            return 1;
        }
        std::cout << "\n";
    } catch (error const &e) {
        std::cerr << e.message() << "\n";
        return 1;
    }

#ifdef _WIN32
    HMODULE handle1;
    HMODULE handle2;
    handle1=LoadLibrary(dllPath1.c_str());
    handle2=LoadLibrary(dllPath2.c_str());
#else
    void* handle1;
    handle1=dlopen(dllPath1.c_str(), RTLD_LAZY);
    void* handle2;
    handle2=dlopen(dllPath2.c_str(), RTLD_LAZY);
#endif
    if(!handle1 || !handle2){
        std::cerr << "cant open DLL file" << std::endl;
        exit(1);
    }

#ifdef USE_FS
    if(logEnable){
        fs::create_directory(logRoot + "log");

        //時刻取得用
        char dn[256];

        //現在時刻取得
        time_t now = time(NULL);
        struct tm *pnow = localtime(&now);
        sprintf(dn, "%04d-%02d-%02d_%02d-%02d-%02d(%d)", pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday,
            pnow->tm_hour, pnow->tm_min, pnow->tm_sec, (int)match);
        std::string dirName(dn);

        logDir = logRoot + "log/" + dllName1 + "-" + dllName2 + "/" + dirName;
        fs::create_directories(logDir);

        digestFile.open(logDir + "/" + dllName1 + "-" + dllName2 + "_digest.txt", std::ios::out);
    }
#endif
    std::array<int, 6> winreason({0,0,0,0,0,0});

    int win1st = 0;
    int win2nd = 0;
    int draw = 0;
    for(int i = 0; i < match; ++i){
        if(outputLevel > 0){
            std::cout << dllName1 << " vs " << dllName2 << std::endl;
            std::cout << "Match: " << i << std::endl;
        }
        int res = run(handle1, handle2);
        if(res > 0) win1st++;
        else if(res < 0) win2nd++;
        else draw++;

        if(res == 1) winreason[0]++;
        if(res == 2) winreason[1]++;
        if(res == 3) winreason[2]++;
        if(res == -1) winreason[3]++;
        if(res == -2) winreason[4]++;
        if(res == -3) winreason[5]++;
        if(outputLevel > 0){
            std::cout << win1st << ":" << win2nd << ":" << draw << " - ";
            for(const int x: winreason)
                std::cout << x << ",";
            std::cout << std::endl;
        }
    }
    if(outputLevel > 0){
        std::cout << dllName1 << " vs " << dllName2 << std::endl;
        std::cout << "Match: " << match << std::endl;
        std::cout << win1st << ":" << win2nd << ":" << draw << " - ";
        for(const int x: winreason)
            std::cout << x << ",";
        std::cout << std::endl;
    }
#ifdef USE_FS
    if(logEnable){
        std::ofstream summary(logDir + "/" + dllName1 + "-" + dllName2 + "_summary.txt", std::ios::out);
        summary << "1stPlayer: " << dllName1 << std::endl;
        summary << "2ndPlayer: " << dllName2 << std::endl;
        summary << "MatchCount: " << match << std::endl;
        summary << "Result: 1stPlayer: TotalWin: " << win1st << std::endl;
        summary << "                     Escape: " << winreason[0] << std::endl;
        summary << "                   TakeBlue: " << winreason[1] << std::endl;
        summary << "                   TakenRed: " << winreason[2] << std::endl;
        summary << "        2ndPlayer: TotalWin: " << win2nd << std::endl;
        summary << "                     Escape: " << winreason[3] << std::endl;
        summary << "                   TakeBlue: " << winreason[4] << std::endl;
        summary << "                   TakenRed: " << winreason[5] << std::endl;
        summary << "        Draw: " << draw << std::endl;
    }
#endif
#ifdef _WIN32
    FreeLibrary(handle1);
    FreeLibrary(handle2);
#else
    dlclose(handle1);
    dlclose(handle2);
#endif
    return 0;
}
