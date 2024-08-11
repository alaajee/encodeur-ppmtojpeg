CC = gcc
CFLAGS = -std=c99 -Iinclude -Wall -Wextra -O0  -g -D_GNU_SOURCE
LDFLAGS = -lm
EXEC = ppm2jpeg

SRC_DIR = src
OBJ_DIR = obj

SRC_FILES = $(SRC_DIR)/ppm2jpeg.c
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))


all: $(EXEC)


$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)


clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)
	find . -maxdepth 1 -type f -executable -exec rm -f {} \;

.PHONY: clean
