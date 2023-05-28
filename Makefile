CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS =
TARGET = main.exe

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
    RM = del /Q
else
    RM = rm -f
endif

.PHONY: all clean build run

all: clean build

build: $(TARGET)

run: build
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(TARGET)
