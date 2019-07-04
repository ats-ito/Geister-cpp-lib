CC = g++
ifeq ($(CC),clang++)
  LIBS = -ldl -stdlib=libc++ -lc++experimental -lc++abi -lc++fs
  CFLAGS = -MMD -MP -w -std=c++17 -Ofast -march=native -mtune=native -fPIC -stdlib=libc++
endif
ifeq ($(CC),g++)
  LIBS = -ldl -lstdc++fs
  CFLAGS = -MMD -MP -w -std=c++17 -Ofast -march=native -mtune=native -fPIC
endif
OBJ_EXT = o
LIB_EXT = so
EXE_EXT = out

ifeq ($(OS),Windows_NT)
  LIBS =  -lws2_32 -lwsock32 -lwinmm
  CFLAGS = -MMD -MP -w -std=c++17 -Ofast -march=native -mtune=native
  CC = clang++
  OBJ_EXT = obj
  LIB_EXT = dll
  EXE_EXT = exe
endif

BIN_DIR = bin
OBJ_DIR = obj
$(shell mkdir $(BIN_DIR))
$(shell mkdir $(OBJ_DIR))

VPATH = random:chototsu:Simulator

TARGETS = $(addprefix $(BIN_DIR)/,PlayerTest.$(EXE_EXT) randomPlayer.$(LIB_EXT) chototsuPlayer.$(LIB_EXT) Player.$(EXE_EXT) client.$(EXE_EXT) competition.$(EXE_EXT))

.PHONY: all
all: $(TARGETS)

.PHONY: allclean
allclean:
	rm -rf *.o *.obj

.PHONY: clean
clean:
	rm -rf *.$(OBJ_EXT) *.exp *.lib bin/*.* obj/*.*
	
PlayerTest_OBJ = $(addprefix $(OBJ_DIR)/,PlayerTest.$(OBJ_EXT) Geister.$(OBJ_EXT) unit.$(OBJ_EXT))
$(addprefix $(BIN_DIR)/,PlayerTest.$(EXE_EXT)): $(PlayerTest_OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

randomPlayer_OBJ = $(addprefix $(OBJ_DIR)/,randomPlayer.$(OBJ_EXT) Geister.$(OBJ_EXT) unit.$(OBJ_EXT))
$(addprefix $(BIN_DIR)/,randomPlayer.$(LIB_EXT)): $(randomPlayer_OBJ)
	$(CC) $(CFLAGS) -shared $^ -o $@

chototsuPlayer_OBJ = $(addprefix $(OBJ_DIR)/,chototsuPlayer.$(OBJ_EXT) Geister.$(OBJ_EXT) unit.$(OBJ_EXT))
$(addprefix $(BIN_DIR)/,chototsuPlayer.$(LIB_EXT)): $(chototsuPlayer_OBJ)
	$(CC) $(CFLAGS) -shared $^ -o $@

Player_OBJ = $(addprefix $(OBJ_DIR)/,Player.$(OBJ_EXT) Geister.$(OBJ_EXT) unit.$(OBJ_EXT))
$(addprefix $(BIN_DIR)/,Player.$(EXE_EXT)): $(Player_OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

client_OBJ = $(addprefix $(OBJ_DIR)/,client.$(OBJ_EXT) Geister.$(OBJ_EXT) unit.$(OBJ_EXT) tcpClient.$(OBJ_EXT))
$(addprefix $(BIN_DIR)/,client.$(EXE_EXT)): $(client_OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

competition_OBJ = $(addprefix $(OBJ_DIR)/,competition.$(OBJ_EXT) unit.$(OBJ_EXT) Geister.$(OBJ_EXT))
$(addprefix $(BIN_DIR)/,competition.$(EXE_EXT)): $(competition_OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

OBJS = $(addprefix $(OBJ_DIR)/,$(addsuffix .$(OBJ_EXT),client competition PlayerTest unit tcpClient Geister Simulator randomPlayer chototsuPlayer Player))
DEPS   = $(OBJS:.$(OBJ_EXT)=.d)
$(warning OBJS = $(OBJS))
$(warning DEPS = $(DEPS))

$(OBJ_DIR)/%.$(OBJ_EXT): %.cpp
	$(CC) $(CFLAGS) -I./ -I./random/ -I./chototsu -I./lib/ -c $< -o $@

-include $(DEPS)
