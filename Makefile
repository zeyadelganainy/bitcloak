CC = gcc
CFLAGS = -std=c99 -Wall -fopenmp -Iinclude
SRC = src/image_io.c src/embed.c src/extract.c src/main.c
OBJ = $(SRC:.c=.o)
TARGET = bitcloak

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
