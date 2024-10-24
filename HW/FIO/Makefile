# Makefile для компиляции program1 и program2

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGETS = program1 program2

all: $(TARGETS)

program1: program1.c
	$(CC) $(CFLAGS) -o program1 program1.c

program2: program2.c
	$(CC) $(CFLAGS) -o program2 program2.c

clean:
	rm -f $(TARGETS)

.PHONY: all clean
