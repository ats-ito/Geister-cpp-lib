#include "Player.hpp"
#include "Player/all.hpp"
#include "Geister.hpp"
#include "hand.hpp"
#include <iostream>

#include <string>
#include <vector>

// https://marycore.jp/prog/cpp/std-string-split/
template<class T> std::vector<std::string> split(const std::string& s, const T& separator, bool ignore_empty = 0, bool split_empty = 0) {
  struct {
    auto len(const std::string&             s) { return s.length(); }
    auto len(const std::string::value_type* p) { return p ? std::char_traits<std::string::value_type>::length(p) : 0; }
    auto len(const std::string::value_type  c) { return c == std::string::value_type() ? 0 : 1; /*return 1;*/ }
  } util;
  
  if (s.empty()) { /// empty string ///
    if (!split_empty || util.len(separator)) return {""};
    return {};
  }
  
  auto v = std::vector<std::string>();
  auto n = static_cast<std::string::size_type>(util.len(separator));
  if (n == 0) {    /// empty separator ///
    if (!split_empty) return {s};
    for (auto&& c : s) v.emplace_back(1, c);
    return v;
  }
  
  auto p = std::string::size_type(0);
  while (1) {      /// split with separator ///
    auto pos = s.find(separator, p);
    if (pos == std::string::npos) {
      if (ignore_empty && p - n + 1 == s.size()) break;
      v.emplace_back(s.begin() + p, s.end());
      break;
    }
    if (!ignore_empty || p != pos)
      v.emplace_back(s.begin() + p, s.begin() + pos);
    p = pos + n;
  }
  return v;
}

#ifndef PLAYER_CLASS
#define PLAYER_CLASS RandomPlayer
#endif

using PlayerClass = PLAYER_CLASS;
PlayerClass player;

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
std::string decideHand(std::string res){
    return player.decideHand(res);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
std::string decideRed(){
    return player.decideRed();
}
}

int main(int argc, char *argv[]){
    std::string recv;
    while(std::cin.good()){
        std::getline(std::cin, recv);
        if(recv.size() == 0) continue;
        // std::cin >> recv;
        auto cmdList = split(recv, " ");
        if(cmdList[0] == "exit"){
            break;
        }
        else if(cmdList[0] == "name"){
          std::cout << "ENTRY " << player.name() << std::endl;
        }
        else if(cmdList[0] == "hand"){
            auto hand = player.decideHand(cmdList[1]);
            std::cout << hand << std::endl;
            // std::cerr << hand << std::endl;
        }
        else if(cmdList[0] == "red"){
            auto red = player.decideRed();
            std::cout << red << std::endl;
            // std::cerr << red << std::endl;
        }
        else if(cmdList[0] == "print"){
            if(cmdList.size() == 1){
                player.getState().printAll();
            }
            else{
                auto geister = Geister(cmdList[1]);
                geister.printAll();
            }
        }
        else{
            std::cout << "recieve: " << recv << std::endl;
            // std::cerr << "recieve: " << recv << std::endl;
        }
    }
    return 0;
}