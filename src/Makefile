CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -lncurses -lm

TETRIS_DIR = brick_game/tetris
COMMON_DIR = brick_game/common
FRONTEND_DIR = gui/cli
BUILD_DIR = build

INCLUDES = -I$(TETRIS_DIR) -I$(COMMON_DIR) -I$(FRONTEND_DIR)

TETRIS_SRC = $(wildcard $(TETRIS_DIR)/*.c)
COMMON_SRC = $(wildcard $(COMMON_DIR)/*.c)
FRONTEND_SRC = $(wildcard $(FRONTEND_DIR)/*.c)
ALL_SRC = $(TETRIS_SRC) $(COMMON_SRC) $(FRONTEND_SRC)


TETRIS_OBJ = $(patsubst $(TETRIS_DIR)/%.c, $(BUILD_DIR)/tetris_%.o, $(TETRIS_SRC))
COMMON_OBJ = $(patsubst $(COMMON_DIR)/%.c, $(BUILD_DIR)/common_%.o, $(COMMON_SRC))
FRONTEND_OBJ = $(patsubst $(FRONTEND_DIR)/%.c, $(BUILD_DIR)/frontend_%.o, $(FRONTEND_SRC))
ALL_OBJ = $(TETRIS_OBJ) $(COMMON_OBJ) $(FRONTEND_OBJ)

TARGET = tetris

PREFIX = /usr/local
INSTALL_BIN = $(PREFIX)/bin

TEST_DIR = tests
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/test_%.o, $(TEST_SRC))
TEST_TARGET = $(BUILD_DIR)/test_runner

TEST_LINK_OBJ = $(filter-out $(BUILD_DIR)/tetris_game.o, $(TETRIS_OBJ)) $(COMMON_OBJ)
CHECK_FLAGS = -lcheck -lpthread -lrt -lm -lsubunit

GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LCOV_INFO = coverage.info
REPORT_DIR = report

DOC_DIR = docs

DIST_NAME = tetris-1.0
DIST_ARCHIVE = $(DIST_NAME).tar.gz

.PHONY: all install uninstall clean dvi dist test gcov_report rebuild
all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(ALL_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/tetris_%.o: $(TETRIS_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/common_%.o: $(COMMON_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/frontend_%.o: $(FRONTEND_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

rebuild: clean all

install: all
	@mkdir -p $(INSTALL_BIN)
	@cp $(TARGET) $(INSTALL_BIN)/$(TARGET)
	@echo "Installed $(TARGET) to $(INSTALL_BIN)"

uninstall:
	@rm -f $(INSTALL_BIN)/$(TARGET)
	@echo "Uninstalled $(TARGET) from $(INSTALL_BIN)"


clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(TARGET)
	@rm -f $(LCOV_INFO)
	@rm -rf $(REPORT_DIR)
	@rm -rf $(DOC_DIR)
	@rm -rf $(DIST_NAME)
	@rm -f $(DIST_ARCHIVE)
	@echo "Cleaned build artifacts"

dist: clean
	@mkdir -p $(DIST_NAME)/brick_game/tetris
	@mkdir -p $(DIST_NAME)/brick_game/common
	@mkdir -p $(DIST_NAME)/gui/cli
	@cp -r $(TETRIS_DIR)/*.c $(TETRIS_DIR)/*.h $(DIST_NAME)/brick_game/tetris/ 2>/dev/null || true
	@cp -r $(COMMON_DIR)/*.c $(COMMON_DIR)/*.h $(DIST_NAME)/brick_game/common/ 2>/dev/null || true
	@cp -r $(FRONTEND_DIR)/*.c $(FRONTEND_DIR)/*.h $(DIST_NAME)/gui/cli/ 2>/dev/null || true
	@cp Makefile $(DIST_NAME)/
	@tar -czvf $(DIST_ARCHIVE) $(DIST_NAME)
	@rm -rf $(DIST_NAME)
	@echo "Created $(DIST_ARCHIVE)"