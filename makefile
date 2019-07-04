CC = clang++
ifeq ($(CC),clang++)
  LIBS = -ldl -stdlib=libc++ -lc++experimental -lc++abi -lc++fs
  CFLAGS = -w -std=c++17 -Ofast -march=native -mtune=native -fPIC -stdlib=libc++
endif
ifeq ($(CC),g++)
  LIBS = -ldl -lstdc++fs
  CFLAGS = -w -std=c++17 -Ofast -march=native -mtune=native -fPIC
endif
OBJ_EXT = o
LIB_EXT = so
EXE_EXT = out

ifeq ($(OS),Windows_NT)
  LIBS =  -lws2_32 -lwsock32 -lwinmm
  CFLAGS = -w -std=c++17 -Ofast -march=native -mtune=native
  CC = clang++
  OBJ_EXT = obj
  LIB_EXT = dll
  EXE_EXT = exe
endif

VPATH = lib:lib/clx:random:chototsu:Simulator

.PHONY: all
all: randomPlayer chototsuPlayer client competition PlayerTest Player

.PHONY: allclean
allclean:
	rm -rf *.o *.obj

.PHONY: clean
clean:
	rm -rf *.$(OBJ_EXT)
	
PlayerTest: PlayerTest.$(OBJ_EXT) geister.$(OBJ_EXT) unit.$(OBJ_EXT)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@.$(EXE_EXT)

randomPlayer: randomPlayer.$(OBJ_EXT) geister.$(OBJ_EXT) unit.$(OBJ_EXT)
	$(CC) $(CFLAGS) -shared $^ -o $@.$(LIB_EXT)

chototsuPlayer: chototsuPlayer.$(OBJ_EXT) geister.$(OBJ_EXT) unit.$(OBJ_EXT)
	$(CC) $(CFLAGS) -shared $^ -o $@.$(LIB_EXT)

Player: Player.$(OBJ_EXT) geister.$(OBJ_EXT) unit.$(OBJ_EXT)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@.$(EXE_EXT)

client: client.$(OBJ_EXT) geister.$(OBJ_EXT) unit.$(OBJ_EXT) tcpClient.$(OBJ_EXT)
	$(CC) $(CFLAGS) $^ $(LIBS) -o Client.$(EXE_EXT)

competition: competition.$(OBJ_EXT) unit.$(OBJ_EXT) geister.$(OBJ_EXT)
	$(CC) $(CFLAGS) $^ $(LIBS) -o Competition.$(EXE_EXT)

client.$(OBJ_EXT): client.cpp
	$(CC) $(CFLAGS) -I./lib/ -c $< -o $@
competition.$(OBJ_EXT): competition.cpp
	$(CC) $(CFLAGS) -I./lib/ -c $< -o $@
PlayerTest.$(OBJ_EXT): PlayerTest.cpp
	$(CC) $(CFLAGS) -I./lib/ -c $< -o $@
unit.$(OBJ_EXT): unit.cpp unit.hpp
	$(CC) $(CFLAGS) -I./lib/ -c $< -o $@
tcpClient.$(OBJ_EXT): tcpClient.cpp tcpClient.hpp
	$(CC) $(CFLAGS) -I./lib/ -c $< -o $@
geister.$(OBJ_EXT): geister.cpp geister.hpp
	$(CC) $(CFLAGS) -I./lib/ -c $< -o $@
Simulator.$(OBJ_EXT): Simulator.cpp Simulator.hpp
	$(CC) $(CFLAGS) -I./ -I./lib/ -c $< -o $@
randomPlayer.$(OBJ_EXT): randomPlayer.cpp randomPlayer.hpp
	$(CC) $(CFLAGS) -I./ -I./random/ -I./lib/ -c $< -o $@
chototsuPlayer.$(OBJ_EXT): chototsuPlayer.cpp
	$(CC) $(CFLAGS) -I./ -I./chototsu -I./lib/ -c $< -o $@
Player.$(OBJ_EXT): Player.cpp Player.hpp
	$(CC) $(CFLAGS) -I./ -I./lib/ -c $< -o $@