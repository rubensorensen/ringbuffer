CC = xelatex

BUILD_PATH = build
BIN = rubens_ringbuffer
SRC_PATH = src
MAIN_FILE = main.tex

.PHONY : default_target
default_target : all

.PHONY : clean
clean :
	@echo "Deleting $(BIN).pdf symlink"
	@$(RM) $(BIN).pdf
	@echo "Deleting build directory"
	@$(RM) -r $(BUILD_PATH)

.PHONY : all
all : $(BUILD_PATH)/$(BIN)
	@echo "Creating symlink: $(BIN).pdf -> $<"
	@$(RM) $(BIN).pdf
	@ln -s $(BUILD_PATH)/$(BIN).pdf $(BIN).pdf

$(BUILD_PATH)/$(BIN) : dir
	$(CC) -output-directory=$(BUILD_PATH) -jobname=$(BIN) $(SRC_PATH)/$(MAIN_FILE)

.PHONY : dir
dir :
	@echo "Creating build directory"
	@mkdir -p $(BUILD_PATH)
