TARGET      := my_game
SRC_DIR     := source
MOD_DIR     := $(SRC_DIR)/copium
BUILD_DIR   := build
CXX         := g++
CXXSTD      := -std=c++20 -fmodules-ts

# find all files
MOD_SRCS    := $(shell find $(MOD_DIR) -name "*.cpp")
MOD_OBJS    := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(MOD_SRCS))

SRCS        := $(filter-out $(MOD_SRCS), $(shell find $(SRC_DIR) -name "*.cpp"))
# define objects
OBJS        := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# auto-dependency generation
DEPFLAGS    := -MMD -MP
DEPS        := $(MOD_OBJS:.o=.d) $(OBJS:.o=.d)

# sdl flags
SDL_CFLAGS  := $(shell pkg-config --cflags sdl3)
SDL_LIBS    := $(shell pkg-config --libs sdl3)

# build release by default
BUILD_TYPE  ?= Release

ifeq ($(BUILD_TYPE), Debug)
	CXXFLAGS    := -Wall -Wextra -g -O0 $(SDL_CFLAGS)
	LDFLAGS     := $(SDL_LIBS)
	BIN_DIR     := $(BUILD_DIR)/debug
else
	CXXFLAGS    := -Wall -Wextra -O2 $(SDL_CFLAGS)
	LDFLAGS     := -s $(SDL_LIBS)
	BIN_DIR     := $(BUILD_DIR)/release
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
$(OBJS): | $(MOD_OBJS)

# compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXSTD) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# create dirs if nonexistant
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# include the generated dependencies
-include $(DEPS)

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) gcm.cache
