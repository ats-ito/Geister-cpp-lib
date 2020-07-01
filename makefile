ifeq  ($(shell uname),Darwin)
	CXX ?= clang++
else
	CXX ?= g++
endif

CXXFLAGS ?= -MMD -MP -std=c++17 -w -O

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
		LDFLAGS += -stdlib=libc++ -lc++experimental -lc++abi -lc++fs
	endif
	ifneq ($(filter g++%, $(CXX)),)
		LDFLAGS += -lstdc++ -lstdc++fs
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
PLAYER_CLASS_FILE := $(shell grep -wrl -e "class $(PLAYER_CLASS)" Player)
$(shell sed -i "2c #include \"$(PLAYER_CLASS_FILE)\"" src/player.cpp)

DEFS := -DPLAYER_NAME=$(PLAYER_NAME) -DPLAYER_CLASS=$(PLAYER_CLASS)

SRC_DIR ?= src
BIN_DIR ?= bin
OBJ_DIR ?= obj
$(shell mkdir -p $(BIN_DIR))
$(shell mkdir -p $(OBJ_DIR))

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

.PHONY: set-release
set-release:
	$(eval DEFS += -DNDEBUG)
	$(eval CXXFLAGS := $(CXXFLAGS:-W%=))
	$(eval CXXFLAGS := $(CXXFLAGS:-O%=))
	$(eval CXXFLAGS += -w -Ofast -march=native -mtune=native)

.PHONY: release
release: set-release all
	@mkdir -p $(BIN_DIR)/$@
	@cp $(TARGETS) $(BIN_DIR)/$@

.PHONY: set-debug
set-debug:
	$(eval CXXFLAGS := $(CXXFLAGS:-w=))
	$(eval CXXFLAGS := $(CXXFLAGS:-O%=))
	$(eval CXXFLAGS += -Wall -Wextra -O0 -g3)

.PHONY: debug
debug: set-debug all
	@mkdir -p $(BIN_DIR)/$@
	@cp $(TARGETS) $(BIN_DIR)/$@


player_OBJ := $(addprefix $(OBJ_DIR)/,player.$(OBJ_EXT) geister.$(OBJ_EXT) simulator.$(OBJ_EXT))
$(BIN_DIR)/$(PLAYER_NAME).$(EXE_EXT): $(player_OBJ)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(BIN_DIR)/$(PLAYER_NAME).$(LIB_EXT): $(player_OBJ)
	$(CXX) $(CXXFLAGS) -shared $^ $(LDFLAGS) -o $@
	
$(BIN_DIR)/%.$(LIB_EXT): $(OBJ_DIR)/%.$(OBJ_EXT) $(OBJ_DIR)/geister.$(OBJ_EXT)
	$(CXX) $(CXXFLAGS) -shared $^ $(LDFLAGS) -o $@

$(BIN_DIR)/%.$(EXE_EXT): $(OBJ_DIR)/%.$(OBJ_EXT) $(OBJ_DIR)/geister.$(OBJ_EXT)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

.PRECIOUS: $(OBJ_DIR)/%.$(OBJ_EXT)
$(OBJ_DIR)/%.$(OBJ_EXT): %.cpp
	$(CXX) $(CXXFLAGS) $(DEFS) -I. -I./include -I./include/lib -c $< -o $@

-include $(DEPS)
