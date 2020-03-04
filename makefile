CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

.PHONY: all clean

all: quantization

quantization: main.o trie.o list.o
	$(CC) $(LDFLAGS) -o $@ $^

trie.o: trie.c trie.h
	$(CC) $(CFLAGS) -c $<

list.o: list.c list.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c trie.h list.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o quantization
