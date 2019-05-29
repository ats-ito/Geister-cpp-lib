#include "clx/tcp.h"
#include "nonsugar.hpp"
#include "Geister.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "random.hpp"
#include "hand.hpp"
#ifdef _WIN32
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef _WIN32
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

using namespace nonsugar;

bool logEnable = false;
std::string dllPath1, dllPath2;
std::string dllName1, dllName2;
std::string logDir;
std::ofstream digestFile;
int outputLevel = 2;

using T = std::string (*)(std::string);
using T2 = std::string (*)();

#ifdef _WIN32
int run(HMODULE dll1, HMODULE dll2){
#else
int run(void* dll1, void* dll2){
#endif
#ifdef _WIN32
    T getHand1=(T)GetProcAddress(dll1, "getHand");
	T getHand2=(T)GetProcAddress(dll2, "getHand");
    T2 decideRed1=(T2)GetProcAddress(dll1, "decideRed");
	T2 decideRed2=(T2)GetProcAddress(dll2, "decideRed");
#else
    T getHand1=(T)dlsym(dll1, "getHand");
    T getHand2=(T)dlsym(dll2, "getHand");
    T2 decideRed1=(T2)dlsym(dll1, "decideRed");
    T2 decideRed2=(T2)dlsym(dll2, "decideRed");
#endif
    if(!getHand1 || !getHand2){
        std::cerr << "cant call getHand" << std::endl;
        exit(1);
    }

    std::ofstream logFile;
    if(logEnable){
        //時刻取得用
        char fn[] = "";

        //現在時刻取得
        time_t now = time(NULL);
        struct tm *pnow = localtime(&now);
        sprintf(fn, "%04d-%02d-%02d_%02d-%02d-%02d.txt", pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday,
            pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
        std::string filename(fn);

        auto fp = logDir + "/" + dllName1 + "-" + dllName2 + "_" + filename;
        fs::path filepath(fp);
        
        logFile.open(filepath, std::ios::out);
    }
    
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
        std::cout << "1stPlayerSet: " << red_ptn1 << std::endl;
        std::cout << "2ndPlayerSet: " << red_ptn2 << std::endl;
    }
    if(logEnable){
        logFile << "Set," << "1," << red_ptn1 << "," << "14U24U34U44U15U25U35U45U41u31u21u11u40u30u20u10u" << std::endl;
        logFile << "Set," << "2," << red_ptn2 << "," << game.mask().toString() << std::endl;
    }
    if(outputLevel > 2){
        game.printBoard();
    }

    int result = 0;

    while(result == 0){
        if(game.turn >= 200) break;
        auto hand = Hand(getHand1(game.mask().toString()));
        if(outputLevel > 1){
            std::cout << "1stPlayer: " << hand.unit.name << " " << hand.direct.toChar() << std::endl;
        }
        if(logEnable)
            logFile << "Move," << "1," << hand.unit.name << "," << hand.direct.toChar() << "," << game.toString() << std::endl;
        game.move(hand.unit.name, hand.direct.toChar());
        if(outputLevel > 2){
            game.printBoard();
        }
        result = game.checkResult();
        if(result)
            break;
        game.changeSide();
        hand = Hand(getHand2(game.mask().toString()));
        if(outputLevel > 1){
            std::cout << "2ndPlayer: " << hand.unit.name << " " << hand.direct.toChar() << std::endl;
        }
        if(logEnable){
            game.changeSide();
            logFile << "Move," << "2," << hand.unit.name << "," << hand.direct.toChar() << "," << game.toString() << std::endl;
            game.changeSide();
        }
        game.move(hand.unit.name, hand.direct.toChar());
        game.changeSide();
        if(outputLevel > 2){
            game.printBoard();
        }
        result = game.checkResult();
    }
    // game.turn++;
    if(outputLevel > 0){
        std::cout << result << ": " << game.turn << std::endl;
    }
    if(logEnable){
        logFile << "Result," << result << "," << game.toString() << std::endl;
        logFile << "Turn," << game.turn << std::endl;
        digestFile << result << "," << game.turn << std::endl;
    }
    return result;
}

int main(int argc, char** argv){
    double match = 1;
    try {
        auto const cmd = command<char>("client", "geister client")
            .flag<'h'>({'h'}, {"help"}, "produce help message")
            .flag<'v'>({'v'}, {"version"}, "print version string")
            .flag<'l'>({'l'}, {"log"}, "enable log record")
            .flag<'o', int>({'o'}, {"output"}, "N", "output level")
            .flag<'c', int>({'c'}, {"match"}, "N", "match count")
            .argument<'d', std::vector<std::string>>("Player-Path")
            ;
        auto const opts = parse(argc, argv, cmd);
        if (opts.has<'h'>()) {
            std::cout << usage(cmd);
            return 0;
        }
        if (opts.has<'v'>()) {
            std::cout << "client, version 1.0\n";
            return 0;
        }
        if (opts.has<'l'>()) {
            logEnable = true;
        }
        if (opts.has<'o'>()) {
            outputLevel = opts.get<'o'>();
        }
        if (opts.has<'c'>()) {
            match = opts.get<'c'>();
        }
        if(opts.get<'d'>().size() > 0){
            dllPath1 = opts.get<'d'>()[0];
            dllPath2 = opts.get<'d'>()[1];
            dllName1 = fs::path(dllPath1).filename().generic_string();
            dllName2 = fs::path(dllPath2).filename().generic_string();
            // dllName1 = dllPath1.substr(2, dllPath1.size());
            // dllName2 = dllPath2.substr(2, dllPath2.size());
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

    if(logEnable){
        fs::create_directory("./log");

        //時刻取得用
        char dn[] = "";

        //現在時刻取得
        time_t now = time(NULL);
        struct tm *pnow = localtime(&now);
        sprintf(dn, "%04d-%02d-%02d_%02d-%02d-%02d(%d)", pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday,
            pnow->tm_hour, pnow->tm_min, pnow->tm_sec, (int)match);
        std::string dirName(dn);

        logDir = "./log/" + dllName1 + "-" + dllName2 + "/" + dirName;
        fs::create_directories(logDir);

        digestFile.open(logDir + "/" + dllName1 + "-" + dllName2 + "_digest.txt", std::ios::out);
    }

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
            for(auto x: winreason)
                std::cout << x << ",";
            std::cout << std::endl;
        }
    }
    if(outputLevel > 0){
        std::cout << dllName1 << " vs " << dllName2 << std::endl;
        std::cout << "Match: " << match << std::endl;
        std::cout << win1st << ":" << win2nd << ":" << draw << " - ";
        for(auto x: winreason)
            std::cout << x << ",";
        std::cout << std::endl;
    }
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
#ifdef _WIN32
    FreeLibrary(handle1);
    FreeLibrary(handle2);
#else
    dlclose(handle1);
    dlclose(handle2);
#endif
}
