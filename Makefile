CC = gcc
CFLAGS = -Wall -g -std=c99 -fsanitize=address,undefined -pthread 


all: ttt ttts

ttt: ttt.o 
	$(CC) $(CFLAGS) -o $@ $^

ttt.o: ttt.c
	$(CC) $(CFLAGS) -c $^

ttts: ttts.o 
	$(CC) $(CFLAGS) -o $@ $^

ttts.o: ttts.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf *.o 