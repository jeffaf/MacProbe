CC = clang
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -framework Foundation -framework Security

TARGET = macprobe
INJECT_LIB = inject.dylib
SRC_DIR = src
OBJ_DIR = obj
SRCS = $(filter-out $(SRC_DIR)/inject.c, $(wildcard $(SRC_DIR)/*.c))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET) $(INJECT_LIB)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(INJECT_LIB): $(SRC_DIR)/inject.c
	$(CC) -dynamiclib $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(INJECT_LIB)

.PHONY: all clean
