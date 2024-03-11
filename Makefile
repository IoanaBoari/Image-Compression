.phony: build run clean

CC=gcc
CFLAGS=-Wall -Werror -g
TARGET=quadtree

build: $(TARGET)

$(TARGET): tema2.c tema2.h
	$(CC) $(CFLAGS) -o $(TARGET) tema2.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
