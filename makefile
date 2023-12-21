# Compiler and flags
CPP := g++
CPPFLAGS := -std=c++17 -Isrc -Isrc/lib
# CPPFLAGS := -std=c++17 -Wall -Wextra -Isrc -Isrc/lib

# Directories
SRC_DIR := src
BUILD_DIR := build

# Files
EXECUTABLE := program

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/lib/**/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Targets
all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE): $(OBJ_FILES)
	$(CPP) $(CPPFLAGS) -o $@ $^

# This pattern rule generates object files in the correct directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

.PHONY: all clean
