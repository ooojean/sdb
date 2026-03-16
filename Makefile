CC=gcc
CFLAGS=-g -Wall
BIN=example
OBJS=b.o example.o

.PHONY: all
all: $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

example: example.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm


.PHONY: clean

clean:
	rm -f *.o example
