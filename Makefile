CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g -O2
SDL_DIR = x86_64-w64-mingw32
CFLAGS += -I$(SDL_DIR)/include
LDFLAGS += -L$(SDL_DIR)/lib
LDLIBS  = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lm

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=build/%.o)
TARGET = app.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LDLIBS)

build/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if exist build rmdir /s /q build
	@if exist $(TARGET) del /Q $(TARGET)
.PHONY: all clean