CC = gcc
CFLAGS = -Wall -g -std=c99 -fsanitize=address,undefined -pthread 
OBJS = ttt.o ttts.o procedure.o
TARGET = ttt ttts procedure


all: $(TARGET)

ttt: ttt.o
	$(CC) $(CFLAGS) -o $@ $^

ttts: ttts.o procedure.o
	$(CC) $(CFLAGS) -o $@ $^

procedure: procedure.o
	$(CC) $(CFLAGS) -o procedure procedure.o

clean:
	rm -rf *.o $(TARGET)