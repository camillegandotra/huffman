CC = clang
CFLAGS = -O3 -Wall -Wextra -Werror -Wpedantic

# Name of program this Makefile is going to build
EXECBIN = encode decode

# All the .c files
SOURCES  = $(wildcard *.c)

# C files corresponding .o files
OBJECTS  = $(SOURCES:%.c=%.o)

all: encode decode

decode: node.o pq.o code.o stack.o decode.o io.o huffman.o
	$(CC) -o $@ $^

encode: node.o pq.o code.o stack.o encode.o io.o huffman.o
	$(CC) -o $@ $^

huffman: huffman.o io.o node.o pq.o code.o stack.o
	$(CC) -o $@ $^
	
stack: stack.o node.o
	$(CC) -o $@ $^
	
code: code.o
	$(CC) -o $@ $^
	
pq: node.o pq.o
	$(CC) -o $@ $^
	
node: node.o
	$(CC) -o $@ $^

io: io.o code.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS)

spotless: 
	rm -f $(EXECBIN) $(OBJECTS)

format:
	clang-format -i -style=file decode.c encode.c stack.c code.c pq.c node.c io.c huffman.c
