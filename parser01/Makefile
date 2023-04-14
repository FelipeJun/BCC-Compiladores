CC=gcc
CFLAGS=  -Wall -Wextra -pedantic -lm -std=c11

ifeq ($(OS),Windows_NT)
    RM = del /Q
else
    RM = rm -f
endif

build: clean main

main: main.c
	$(CC) $(CFLAGS) main.c -o main

clean:
	$(RM) *.o *.exe