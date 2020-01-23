#include "clx/tcp.h"
#include "nonsugar.hpp"
#include "geister.hpp"
#include "tcpClient.hpp"
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

using namespace nonsugar;

bool visibleInfo = true;
int output = 2;

std::string host = "localhost";
int port = 10001;

int playoutCount = 1000;

using T = std::string (*)(std::string);
using T2 = std::string (*)(void);

int run(int port, std::string dll){
#ifdef _WIN32
    HMODULE handle;
    handle=LoadLibrary(dll.c_str());
#else
    void* handle;
    handle=dlopen(dll.c_str(), RTLD_LAZY);
#endif

    if(!handle){
        std::cerr << "cant open DLL file" << std::endl;
        exit(1);
    }
#ifdef _WIN32
    T decideHand=(T)GetProcAddress(handle, "decideHand");
    T2 decideRed=(T2)GetProcAddress(handle, "decideRed");
#else
    T decideHand=(T)dlsym(handle, "decideHand");
    T2 decideRed=(T2)dlsym(handle, "decideRed");
#endif
    if(!decideHand){
        std::cerr << "cant call decideHand" << std::endl;
        exit(1);
    }

    std::string red_ptn;
    if(!decideRed){
        std::cerr << "cant call decideRed" << std::endl;
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
        std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
        red_ptn = pattern[serector(mt)];
    }
    else {
        red_ptn = decideRed();
    }
    
    int turn = 0;
    
    if(visibleInfo && output)
        std::cout << red_ptn << std::endl;

    auto cl = TCPClient(host, port);

    cl.send("SET:" + red_ptn + "\r\n");

    std::string res = "";
    while(res.substr(0, 3) != "MOV"){
        res = cl.recv();
        if(visibleInfo && output)
            std::cout << res << std::endl;
    }
    auto game = Geister(res.substr(4));

    while(res.substr(0, 3) != "WON" && res.substr(0, 3) != "LST" && res.substr(0, 3) != "DRW"){
        if(output > 2)
            game.printBoard();
        auto hand = Hand(decideHand(game));
        auto name = string{hand.unit.name};
        auto direct = string{hand.direct.toChar()};
        if(output > 1)
            std::cout << name << " " << direct << std::endl;
        cl.move(name, direct);
        res = cl.recv();
        while(res.substr(0, 3) != "MOV"){
            res = cl.recv();
            if(res.substr(0, 3) == "WON" || res.substr(0, 3) == "LST" || res.substr(0, 3) == "DRW")
                break;
        }
        if(res.substr(0, 3) == "MOV"){
            game.setState(res.substr(4));
            turn += 1;
        }
    }
    auto result = res.substr(0, 3);
    game.setState(res.substr(4));
    std::map<std::string, double> score = {{"WON", 1}, {"LST", 0}, {"DRW", 0.1}};
    if(output > 1)
        game.printBoard();
    if(output)
        std::cout << result << ": " << turn << std::endl;
    cl.close();
#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
    return score[result];
}

int main(int argc, char** argv){
    std::string dll;
    try {
        auto const cmd = command<char>("client", "geister client")
            .flag<'h'>({'h'}, {"help"}, "produce help message")
            .flag<'v'>({'v'}, {"version"}, "print version string")
            .flag<'p', int>({'p'}, {"port"}, "N", "connect port")
            .flag<'H', std::string>({'H'}, {"host"}, "host string", "connect host")
            .flag<'c', int>({}, {"playout"}, "N", "playout count")
            .flag<'i'>({'i'}, {"hide"}, "hide information")
            .flag<'o', int>({'o'}, {"output"}, "N", "output level")
            .argument<'d', std::vector<std::string>>("DLL-Path")
            ;
        auto const opts = parse(argc, argv, cmd, argument_order::flexible);
        if (opts.has<'h'>()) {
            std::cout << usage(cmd);
            return 0;
        }
        if (opts.has<'v'>()) {
            std::cout << "client, version 1.0\n";
            return 0;
        }
        if (opts.has<'p'>()) {
            port = opts.get<'p'>();
        }
        if (opts.has<'H'>()) {
            host = opts.get<'H'>();
        }
        if (opts.has<'c'>()) {
            playoutCount = opts.get<'c'>();
        }
        if (opts.has<'o'>()) {
            output = opts.get<'o'>();
        }
        if(opts.get<'d'>().size() > 0){
            dll = opts.get<'d'>()[0];
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

    run(port, dll);
}
