#include "Geister.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "random.hpp"
#include "hand.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include "nonsugar.hpp"

using namespace nonsugar;

std::string dllPath1 = "./randoPlayer.so";

using T = Hand (*)(std::string);

int main(int argc, char** argv){
    std::string ptn = "----22B99B99B99B15R25R35R45R41u32u21u12u40u30u20u10u";
    try {
        auto const cmd = command<char>("client", "geister client")
            .flag<'h'>({'h'}, {"help"}, "produce help message")
            .flag<'v'>({'v'}, {"version"}, "print version string")
            .flag<'p', std::string>({'p'}, {"pattern"}, "string", "board pattern")
            .argument<'d', std::vector<std::string>>("DLL-Path")
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
        if (opts.has<'p'>()) {
            ptn = opts.get<'p'>();
        }
        if(opts.get<'d'>().size() > 0){
            dllPath1 = opts.get<'d'>()[0];
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
    handle1=LoadLibrary(dllPath1.c_str());
#else
    void* handle1;
    handle1=dlopen(dllPath1.c_str(), RTLD_LAZY);
#endif
    if(!handle1){
        std::cerr << "cant open DLL file" << std::endl;
        exit(1);
    }

#ifdef _WIN32
    T getHand1=(T)GetProcAddress(handle1, "getHand");
#else
    T getHand1=(T)dlsym(handle1, "getHand");
#endif
    if(!getHand1){
        std::cerr << "cant call getHand" << std::endl;
        exit(1);
    }

    int result = 0;
    Geister brd("----12B99B99B99B15R99R99R99R41u31u21u11u40u30u20u10u");
    brd.setState(ptn);
    brd.printBoard();

    auto hand = getHand1(brd.toString());
    std::cout << "1stPlayer: " << hand.unit << " " << hand.direct << std::endl;

#ifdef _WIN32
    FreeLibrary(handle1);
#else
    dlclose(handle1);
#endif
}
