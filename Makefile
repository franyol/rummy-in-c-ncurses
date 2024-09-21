CC = gcc

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Flags
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# libs
CLIBS = -lncurses

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.test.c)

# Output executables
MAIN_EXEC = $(BUILD_DIR)/main
TEST_EXECUTABLES = $(patsubst $(TEST_DIR)/%.test.c, $(BUILD_DIR)/%.test, $(TEST_FILES))

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Default target: Build main program
all: $(BUILD_DIR) $(MAIN_EXEC)

# Build main executable
$(MAIN_EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(CLIBS)
	@echo "Compiled main executable: $@"

# Build all tests
tests: $(BUILD_DIR) $(TEST_EXECUTABLES)

# Build individual test executables
$(BUILD_DIR)/double_linked_lists.test: $(TEST_DIR)/double_linked_lists.test.c
	$(CC) $(CFLAGS) $(TEST_DIR)/double_linked_lists.test.c -o $(BUILD_DIR)/double_linked_lists.test $(LDFLAGS)
	@echo "Compiled test executable: double_linked_lists"

$(BUILD_DIR)/fsm.test: $(TEST_DIR)/fsm.test.c $(BUILD_DIR)/fsm.o
	$(CC) $(CFLAGS) $(TEST_DIR)/fsm.test.c -o $(BUILD_DIR)/fsm.test $(BUILD_DIR)/fsm.o $(LDFLAGS)
	@echo "Compiled test executable: fsm"

# Compile object files for source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory."

# Phony targets
.PHONY: all clean tests main
