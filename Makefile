CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
CLIBS=

all: main
.PHONY: all

main: main.c fstr.h
	$(CC) $(CFLAGS) -o main main.c $(CLIBS)
