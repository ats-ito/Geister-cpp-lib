# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)

# コンパイラの指定
if($ENV{OS} MATCHES "Windows_NT")
    message("This is Windows!")
    SET(CMAKE_CXX_COMPILER "clang++")
else()
    message("This is Linux?")
    SET(CMAKE_CXX_COMPILER "clang++")
endif()

# コンパイラに応じたコンパイルフラグの設定
if(CMAKE_CXX_COMPILER MATCHES ".*clang.*")
    SET(CMAKE_CXX_FLAGS "-w -std=c++17 -Ofast -march=native -mtune=native -fPIC -stdlib=libc++")
elseif(CMAKE_CXX_COMPILER MATCHES ".*g\\+\\+.*")
    SET(CMAKE_CXX_FLAGS "-w -std=c++17 -Ofast -march=native -mtune=native -fPIC")
endif()


# コンパイラに応じたコンパイルフラグの設定
# if(CMAKE_CXX_COMPILER MATCHES ".*clang.*")
#     SET(CMAKE_CXX_FLAGS "-w -std=c++17 -Ofast -march=native -mtune=native -fPIC -stdlib=libc++")
#     set(LIBS " -ldl -stdlib=libc++ -lc++experimental -lc++abi -lc++fs")
# elseif(CMAKE_CXX_COMPILER MATCHES ".*g\\+\\+.*")
#     SET(CMAKE_CXX_FLAGS "-w -std=c++17 -Ofast -march=native -mtune=native -fPIC")
#     SET(LIBS "-ldl -lstdc++fs")
# endif()

# プロジェクト名と使用する言語を設定
project(geister CXX)

# インクルードパスの設定
include_directories(./)
include_directories(./lib/)

# 実行ファイルを追加
add_executable(competition competition.cpp unit.cpp Geister.cpp)
add_executable(client client.cpp Geister.cpp unit.cpp tcpClient.cpp)

# OSの判別
if($ENV{OS} MATCHES "Windows_NT")
    # SET(LIBS "-lws2_32 -lwsock32 -lwinmm")
    if(CMAKE_CXX_COMPILER MATCHES ".*clang.*")
        SET(CMAKE_CXX_FLAGS "-w -std=c++17 -Ofast -march=native -mtune=native -stdlib=libc++")
    elseif(CMAKE_CXX_COMPILER MATCHES ".*g\\+\\+.*")
        SET(CMAKE_CXX_FLAGS "-w -std=c++17 -Ofast -march=native -mtune=native")
    endif()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/windows)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/windows/player)
    target_link_libraries(competition ws2_32 wsock32 winmm)
    target_link_libraries(client ws2_32 wsock32 winmm)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux/player)
    if(CMAKE_CXX_COMPILER MATCHES ".*clang.*")
        target_link_libraries(competition dl c++experimental c++abi c++fs)
        target_link_libraries(client dl c++experimental c++abi c++fs)
        SET(CMAKE_CXX_FLAGS "-w -std=c++17 -Ofast -march=native -mtune=native -fPIC -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    elseif(CMAKE_CXX_COMPILER MATCHES ".*g\\+\\+.*")
        target_link_libraries(competition dl stdc++fs)
        target_link_libraries(client dl stdc++fs)
        SET(CMAKE_CXX_FLAGS "-w -std=c++17 -Ofast -march=native -mtune=native -fPIC")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    endif()
endif()

set(CMAKE_SHARED_LIBRARY_PREFIX "")
add_library(randomPlayer SHARED ./random/randomPlayer.cpp Geister.cpp unit.cpp)
add_library(chototsuPlayer SHARED ./chototsu/chototsuPlayer.cpp Geister.cpp unit.cpp)
add_library(Player SHARED Player.cpp Geister.cpp unit.cpp Geister.cpp)

if($ENV{OS} MATCHES "Windows_NT")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/windows)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    set_target_properties( randomPlayer chototsuPlayer
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/windows/player
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/windows/player
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/windows/player
    )
endif()