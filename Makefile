CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -Werror

all: main

main: main.o
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o
	clear

run:
		./main
