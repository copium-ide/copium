TARGET	  := my_game
SRC_DIR	 := source
MOD_DIR	 := $(SRC_DIR)/copium
BUILD_DIR   := build
CXX		 := g++
CXXSTD	  := -std=c++20 -fmodules

# find all files
MOD_SRCS	:= $(wildcard $(MOD_DIR)/*.cpp)
MOD_OBJS	:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(MOD_SRCS))

SRCS		:= $(filter-out $(MOD_SRCS), $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp))
# define objects
OBJS		:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# build release by default
BUILD_TYPE  ?= Release

ifeq ($(BUILD_TYPE), Debug)
	CXXFLAGS	:= -Wall -Wextra -g -O0
	LDFLAGS	 :=
	BIN_DIR	 := $(BUILD_DIR)/debug
else
	CXXFLAGS	:= -Wall -Wextra -O2
	LDFLAGS	 := -s
	BIN_DIR	 := $(BUILD_DIR)/release
endif

EXECUTABLE  := $(BIN_DIR)/$(TARGET)


.PHONY: all debug release clean mkdir build

all: $(BUILD_DIR) $(BIN_DIR) $(EXECUTABLE)

debug:
	$(MAKE) all BUILD_TYPE=Debug

release:
	$(MAKE) all BUILD_TYPE=Release

# build the executable
$(EXECUTABLE): $(MOD_OBJS) $(OBJS) | $(BIN_DIR)
	@echo "Linking $@"
	$(CXX) $(MOD_OBJS) $(OBJS) $(LDFLAGS) -o $@

# ensure modules are built before files that import them
$(OBJS): $(MOD_OBJS)

# compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXSTD) $(CXXFLAGS) -c $< -o $@

# create dirs if nonexistant
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) gcm.cache
