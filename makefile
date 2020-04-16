USE_FS ?= true

ifeq  ($(shell uname),Darwin)
	CXX ?= clang++
else
	CXX ?= g++
endif

CXXFLAGS ?= -MMD -MP -w -std=c++17 -Ofast -march=native -mtune=native

ifeq ($(OS),Windows_NT)
	LDFLAGS ?= -lws2_32 -lwsock32 -lwinmm
	OBJ_EXT ?= obj
	LIB_EXT ?= dll
	EXE_EXT ?= exe
else
	CXXFLAGS += -fPIC
	LDFLAGS ?= -ldl -lpthread
	OBJ_EXT ?= o
	LIB_EXT ?= so
	EXE_EXT ?= out
	ifneq ($(filter clang++%, $(CXX)),)
		CXXFLAGS += -stdlib=libc++
		LDFLAGS += -stdlib=libc++ -lc++experimental -lc++abi
		ifdef USE_FS
			LDFLAGS += -lc++fs
		endif
	endif
	ifneq ($(filter g++%, $(CXX)),)
		LDFLAGS += -lstdc++
		ifdef USE_FS
			LDFLAGS += -lstdc++fs
		endif
	endif
endif

ifdef PN
	PLAYER_NAME ?= $(PN)
endif
PLAYER_NAME ?= Player
ifneq ($(PC),pc)
	PLAYER_CLASS ?= $(PC)
endif
ifdef PLAYER_CLASS
# $(shell find ./Player -type f -name \*.hpp | awk -F"/" '{ print $$NF }' | grep -v all.hpp | awk '{print "#include \"" $$1 "\""}' > Player/all.hpp)
endif
PLAYER_CLASS ?= RandomPlayer
PLAYER_CLASS_FILE := $(shell grep -rl -e "class $(PLAYER_CLASS)" Player)
$(shell sed -i "2c #include \"$(PLAYER_CLASS_FILE)\"" src/player.cpp)

DEFS := -DPLAYER_NAME=$(PLAYER_NAME) -DPLAYER_CLASS=$(PLAYER_CLASS)

ifdef USE_FS
DEFS += -DUSE_FS
endif

SRC_DIR ?= src
BIN_DIR ?= bin
OBJ_DIR ?= obj
EXIST_BIN_DIR := $(shell find ./ -type d -name $(BIN_DIR) | awk -F"/" '{ print $$NF }')
ifneq ($(EXIST_BIN_DIR),$(BIN_DIR))
$(shell mkdir $(BIN_DIR))
endif
EXIST_OBJ_DIR := $(shell find ./ -type d -name $(OBJ_DIR) | awk -F"/" '{ print $$NF }')
ifneq ($(EXIST_OBJ_DIR),$(OBJ_DIR))
$(shell mkdir $(OBJ_DIR))
endif

SRCS := $(notdir $(wildcard src/*) $(wildcard Player/*.cpp))
OBJS := $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.$(OBJ_EXT)))
DEPS := $(OBJS:.$(OBJ_EXT)=.d)

VPATH := Player:$(SRC_DIR)

LIBS := $(addsuffix .$(LIB_EXT), randomPlayer chototsuPlayer $(PLAYER_NAME))
EXES := $(addsuffix .$(EXE_EXT), client competition $(PLAYER_NAME))
TARGETS := $(addprefix $(BIN_DIR)/, $(LIBS) $(EXES))


.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.*


player_OBJ := $(addprefix $(OBJ_DIR)/,player.$(OBJ_EXT) geister.$(OBJ_EXT) simulator.$(OBJ_EXT))
$(BIN_DIR)/$(PLAYER_NAME).$(EXE_EXT): $(player_OBJ)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(BIN_DIR)/$(PLAYER_NAME).$(LIB_EXT): $(player_OBJ)
	$(CXX) $(CXXFLAGS) -shared $^ $(LDFLAGS) -o $@
	
$(BIN_DIR)/%.$(LIB_EXT): $(OBJ_DIR)/%.$(OBJ_EXT) $(OBJ_DIR)/geister.$(OBJ_EXT)
	$(CXX) $(CXXFLAGS) -shared $^ $(LDFLAGS) -o $@

$(BIN_DIR)/%.$(EXE_EXT): $(OBJ_DIR)/%.$(OBJ_EXT) $(OBJ_DIR)/geister.$(OBJ_EXT)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.$(OBJ_EXT): %.cpp
	$(CXX) $(CXXFLAGS) $(DEFS) -I. -I./include -I./include/lib -c $< -o $@

-include $(DEPS)
