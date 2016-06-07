CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -Werror
THREAD=-pthread -lpthread

all: main

main: main.o
	$(CC) -o $@ $^ $(THREAD)

%.o: %.c %.h
	$(CC) -o  $@ -c $< #$(CFLAGS)


clean:
	rm -rf *.o
	clear

run:
		./main
