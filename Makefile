CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -Werror

all: main

main: main.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
	clear

run:
		./main
