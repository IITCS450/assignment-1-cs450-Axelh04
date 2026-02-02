CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -O2 -g
INCLUDES=-Iinclude
BIN_DIR=bin
SRC_DIR=src
BINS=$(BIN_DIR)/procinfo $(BIN_DIR)/runwait
all: $(BINS)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
$(BIN_DIR)/procinfo: $(SRC_DIR)/procinfo.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
$(BIN_DIR)/runwait: $(SRC_DIR)/runwait.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -rf $(BIN_DIR)
test: all
	bash tests/test_procinfo.sh
	bash tests/test_runwait.sh
.PHONY: all clean test
