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

.PHONY: all
all: montecarloPlayer MCTSPlayer randomPlayer chototsuPlayer client competition PlayerTest

.PHONY: allclean
allclean:
	rm -rf *.o *.obj

.PHONY: clean
clean:
	rm -rf *.$(OBJ_EXT)

montecarloPlayer: montecarloPlayer.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT) simulateBoard.$(OBJ_EXT)
	$(CC) $(CFLAGS) -shared board.$(OBJ_EXT) unit.$(OBJ_EXT) simulateBoard.$(OBJ_EXT) montecarloPlayer.$(OBJ_EXT) -o montecarloPlayer.$(LIB_EXT)
	
MCTSPlayer: MCTSPlayer.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT) simulateBoard.$(OBJ_EXT) MCTSNode.$(OBJ_EXT)
	$(CC) $(CFLAGS) -shared board.$(OBJ_EXT) unit.$(OBJ_EXT) simulateBoard.$(OBJ_EXT) MCTSNode.$(OBJ_EXT) MCTSPlayer.$(OBJ_EXT) -o MCTSPlayer.$(LIB_EXT)
	
PlayerTest: PlayerTest.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT) simulateBoard.$(OBJ_EXT)
	$(CC) $(CFLAGS) board.$(OBJ_EXT) unit.$(OBJ_EXT) simulateBoard.$(OBJ_EXT) PlayerTest.$(OBJ_EXT) $(LIBS) -o PlayerTest.$(EXE_EXT)

randomPlayer: randomPlayer.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT)
	$(CC) $(CFLAGS) -shared board.$(OBJ_EXT) unit.$(OBJ_EXT) randomPlayer.$(OBJ_EXT) -o randomPlayer.$(LIB_EXT)

chototsuPlayer: chototsuPlayer.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT)
	$(CC) $(CFLAGS) -shared board.$(OBJ_EXT) unit.$(OBJ_EXT) chototsuPlayer.$(OBJ_EXT) -o chototsuPlayer.$(LIB_EXT)

client: client.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT) tcpClient.$(OBJ_EXT)
	$(CC) $(CFLAGS) client.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT) tcpClient.$(OBJ_EXT) $(LIBS) -o Client.$(EXE_EXT)

competition: competition.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT) gameBoard.$(OBJ_EXT)
	$(CC) $(CFLAGS) competition.$(OBJ_EXT) board.$(OBJ_EXT) unit.$(OBJ_EXT) gameBoard.$(OBJ_EXT) $(LIBS) -o Competition.$(EXE_EXT)

client.$(OBJ_EXT): client.cpp
	$(CC) $(CFLAGS) -c client.cpp -o client.$(OBJ_EXT)
competition.$(OBJ_EXT): competition.cpp
	$(CC) $(CFLAGS) -c competition.cpp -o competition.$(OBJ_EXT)
montecarloPlayer.$(OBJ_EXT): montecarloPlayer.cpp montecarloPlayer.hpp
	$(CC) $(CFLAGS) -c montecarloPlayer.cpp -o montecarloPlayer.$(OBJ_EXT)
MCTSPlayer.$(OBJ_EXT): MCTSPlayer.cpp MCTSPlayer.hpp
	$(CC) $(CFLAGS) -c MCTSPlayer.cpp -o MCTSPlayer.$(OBJ_EXT)
MCTSNode.$(OBJ_EXT): MCTSNode.cpp MCTSNode.hpp
	$(CC) $(CFLAGS) -c MCTSNode.cpp -o MCTSNode.$(OBJ_EXT)
PlayerTest.$(OBJ_EXT): PlayerTest.cpp
	$(CC) $(CFLAGS) -c PlayerTest.cpp -o PlayerTest.$(OBJ_EXT)
randomPlayer.$(OBJ_EXT): randomPlayer.cpp randomPlayer.hpp
	$(CC) $(CFLAGS) -c randomPlayer.cpp -o randomPlayer.$(OBJ_EXT)
chototsuPlayer.$(OBJ_EXT): chototsuPlayer.cpp
	$(CC) $(CFLAGS) -c chototsuPlayer.cpp -o chototsuPlayer.$(OBJ_EXT)
board.$(OBJ_EXT): board.cpp board.hpp
	$(CC) $(CFLAGS) -c board.cpp -o board.$(OBJ_EXT)
unit.$(OBJ_EXT): unit.cpp unit.hpp
	$(CC) $(CFLAGS) -c unit.cpp -o unit.$(OBJ_EXT)
tcpClient.$(OBJ_EXT): tcpClient.cpp tcpClient.hpp
	$(CC) $(CFLAGS) -c tcpClient.cpp -o tcpClient.$(OBJ_EXT)
simulateBoard.$(OBJ_EXT): simulateBoard.cpp simulateBoard.hpp
	$(CC) $(CFLAGS) -c simulateBoard.cpp -o simulateBoard.$(OBJ_EXT)
gameBoard.$(OBJ_EXT): gameBoard.cpp gameBoard.hpp
	$(CC) $(CFLAGS) -c gameBoard.cpp -o gameBoard.$(OBJ_EXT)