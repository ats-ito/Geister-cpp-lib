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
#include "player.hpp"
#if __has_include(<filesystem>)
#define FS_ENABLE
#include <filesystem>
#elif __has_include(<experimental/filesystem>)
#define FS_EXPERIMENTAL_ENABLE
#include <experimental/filesystem>
#endif
#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#if defined(FS_ENABLE)
namespace fs = std::filesystem;
#elif defined(FS_EXPERIMENTAL_ENABLE)
namespace fs = std::experimental::filesystem;
#endif

using namespace nonsugar;

static std::string getFileName(std::string path){
#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
            return fs::path(path).filename().generic_string();
#else
    int last = 0;
    for(size_t i = 0; i < path.size(); ++i){
        if(path[i] == '/' || path[i] == '\\'){
            last = i+1;
        }
    }
    return std::string(&path[last]);
#endif
}

#ifdef _WIN32
using HANDLE_TYPE = HMODULE;
#else
using HANDLE_TYPE = void*;
#endif

bool logEnable = false;
std::string logRoot = "log";
std::string dllPath1, dllPath2;
std::string dllName1, dllName2;
#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
std::string logDir;
std::ofstream digestFile;
#endif
int outputLevel = 2;
int mask = 0;

template<class T>
T dynamicLink(HANDLE_TYPE& handle, const char* funcName){
#ifdef _WIN32
    T func=(T)GetProcAddress(handle, funcName);
#else
    T func=(T)dlsym(handle, funcName);
#endif
    if(!func){
        std::cerr << "cant call " << funcName << std::endl;
        exit(1);
    }
    return func;
}

int run(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2){
#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
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
    player1->initialize();
    player2->initialize();
    std::string red_ptn1 = player1->decideRed();
    std::string red_ptn2 = player2->decideRed();
    
    Geister game(red_ptn1, red_ptn2);
    if(outputLevel > 1){
        if(!(mask & 0b01))
            std::cout << "1stPlayerSet: " << red_ptn1 << std::endl;
        if(!(mask & 0b10))
            std::cout << "2ndPlayerSet: " << red_ptn2 << std::endl;
    }
#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
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
        Hand hand = Hand(player1->decideHand(game.mask()));
        if(outputLevel > 1){
            std::cout << "1stPlayer: " << hand.unit.name() << " " << hand.direct.toChar() << '\t' << game << std::endl;
        }
#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
        if(logEnable){
            logFile << "Move," << "1," << hand.unit.name() << "," << hand.direct.toChar() << "," << game << std::endl;
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
        result = game.result();
        if(game.isEnd())
            break;
        game.changeSide();
        hand = Hand(player2->decideHand(game.mask()));
        game.changeSide();
        if(outputLevel > 1){
            std::cout << "2ndPlayer: " << hand.unit.name() << " " << hand.direct.toChar() << '\t' << game << std::endl;
        }
#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
        if(logEnable){
            logFile << "Move," << "2," << hand.unit.name() << "," << hand.direct.toChar() << "," << game << std::endl;
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
        result = game.result();
    }
    player1->finalize(game);
    game.changeSide();
    player2->finalize(game);
    game.changeSide();
    // game.turn++;
    if(outputLevel > 0){
        std::cout << result << ": " << game.turn() << '\t' << game << std::endl;
    }
    int resultInt = result == Result::Draw ? 0 : static_cast<int>(result);
#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
    if(logEnable){
        logFile << "Result," << resultInt << "," << game << std::endl;
        logFile << "Turn," << game.turn() << std::endl;
        digestFile << resultInt << "," << game.turn() << std::endl;
    }
#endif
    return resultInt;
}

int main(int argc, char** argv){
    std::cin.tie(0);
    std::ios::sync_with_stdio(false);
    
    uint64_t match = 1;
    try {
        auto const cmd = command<char>("competition", "geister competition")
            .flag<'h'>({'h'}, {"help"}, "", "produce help message")
            .flag<'v'>({'v'}, {"version"}, "", "print version string")
            .flag<'l'>({'l'}, {"log"}, "", "enable log record")
            .flag<'d', std::string>({'d'}, {"dest"}, "PATH", "log destination")
            .flag<'o', int>({'o'}, {"output"}, "N", "output level")
            .flag<'c', uint64_t>({'c'}, {"match"}, "N", "match count")
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
            dllName1 = getFileName(dllPath1);
            dllName2 = getFileName(dllPath2);
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

    HANDLE_TYPE handle1;
    HANDLE_TYPE handle2;
#ifdef _WIN32
    handle1=LoadLibrary(dllPath1.c_str());
    handle2=LoadLibrary(dllPath2.c_str());
#else
    handle1=dlopen(dllPath1.c_str(), RTLD_LAZY);
    handle2=dlopen(dllPath2.c_str(), RTLD_LAZY);
#endif
    if(!handle1 || !handle2){
        std::cerr << "cant open DLL file" << std::endl;
        exit(1);
    }

#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
    if(logEnable){
        fs::create_directory(logRoot);

        //時刻取得用
        char dn[256];

        //現在時刻取得
        time_t now = time(NULL);
        struct tm *pnow = localtime(&now);
        sprintf(dn, "%04d-%02d-%02d_%02d-%02d-%02d(%d)", pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday,
            pnow->tm_hour, pnow->tm_min, pnow->tm_sec, (int)match);
        std::string dirName(dn);

        logDir = logRoot + "/" + dllName1 + "-" + dllName2 + "/" + dirName;
        fs::create_directories(logDir);

        digestFile.open(logDir + "/" + dllName1 + "-" + dllName2 + "_digest.txt", std::ios::out);
    }
#endif

    std::array<int, 6> winreason({0,0,0,0,0,0});

    int win1st = 0;
    int win2nd = 0;
    int draw = 0;
    {
        using T= Player* (*)();
        T createPlayer1 = dynamicLink<T>(handle1, "createPlayer");
        T createPlayer2 = dynamicLink<T>(handle2, "createPlayer");
        std::shared_ptr<Player> player1(createPlayer1());
        std::shared_ptr<Player> player2(createPlayer2());
        for(size_t i = 0; i < match; ++i){
            if(outputLevel > 0){
                std::cout << dllName1 << " vs " << dllName2 << std::endl;
                std::cout << "Match: " << i << std::endl;
            }
            int res = run(player1, player2);
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
    }
    if(outputLevel > 0){
        std::cout << dllName1 << " vs " << dllName2 << std::endl;
        std::cout << "Match: " << match << std::endl;
        std::cout << win1st << ":" << win2nd << ":" << draw << " - ";
        for(const int x: winreason)
            std::cout << x << ",";
        std::cout << std::endl;
    }
#if defined(FS_ENABLE) || defined(FS_EXPERIMENTAL_ENABLE)
    if(logEnable){
        std::ofstream summary(logDir + "/" + dllName1 + "-" + dllName2 + "_summary.txt", std::ios::out);
        summary << "1stPlayer," << dllName1 << std::endl;
        summary << "2ndPlayer," << dllName2 << std::endl;
        summary << "MatchCount," << match << std::endl;
        summary << "TotalWin1st," << win1st << std::endl;
        summary << "Escape1st," << winreason[0] << std::endl;
        summary << "TakeBlue1st," << winreason[1] << std::endl;
        summary << "TakenRed1st," << winreason[2] << std::endl;
        summary << "TotalWin2nd," << win2nd << std::endl;
        summary << "Escape2nd," << winreason[3] << std::endl;
        summary << "TakeBlue2nd," << winreason[4] << std::endl;
        summary << "TakenRed2nd," << winreason[5] << std::endl;
        summary << "Draw," << draw << std::endl;
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
