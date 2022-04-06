CXX ?= g++
CFLAGS = -g -Wall -Wextra -std=c++20

BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin
BIN = ringbuffer

SRC_EXT = cpp
SRC_PATH = src
INCL_PATH = -I include/

SRC = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
OBJ = $(SRC:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
DEPS = $(OBJ:.o=.d)

.PHONY : default_target
default_target : release

.PHONY : release
release : export CXXFLAGS := $(CXXFLAGS) $(CFLAGS)
release : dirs
	@$(MAKE) all

.PHONY : dirs
dirs :
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJ))
	@mkdir -p $(BIN_PATH)

.PHONY : clean
clean :
	@echo "Deleting $(BIN) symlink"
	@$(RM) $(BIN)
	@echo "Deleting directories"
	@$(RM) -r $(BIN_PATH)
	@$(RM) -r $(BUILD_PATH)

.PHONY : all
all : $(BIN_PATH)/$(BIN)
	@echo "Making symlink: $(BIN) -> $<"
	@$(RM) $(BIN)
	@ln -s $(BIN_PATH)/$(BIN) $(BIN)

$(BIN_PATH)/$(BIN) : $(OBJ)
	@echo "Linking: $@"
	$(CXX) $(OBJ) -o $@ ${libs} -lpthread

-include $(DEPS)

$(BUILD_PATH)/%.o : $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) -MP -MMD -c $< -o $@
