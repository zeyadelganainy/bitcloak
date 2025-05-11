CC = gcc
CFLAGS = -std=c99 -Wall -fopenmp -Iinclude
SRC = src/main.c src/embed.c src/extract.c src/image_io.c src/utils.c src/crypto.c src/aes.c src/lodepng.c
OBJ = $(SRC:.c=.o)
TARGET = bitcloak

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	del /Q src\*.o bitcloak.exe 2>nul || exit 0

