CC = gcc
CFLAGS = -std=c99 -Wall -fopenmp -Iinclude
SRC = src/main.c src/embed.c src/extract.c src/image_io.c src/utils.c src/crypto.c src/aes.c src/lodepng.c
OBJ = $(SRC:.c=.o)
TARGET = bitcloak
TARGET_GUI = bitcloak_gui.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET_GUI): src/bitcloak_gui.c src/aes.c src/crypto.c src/embed.c src/extract.c src/image_io.c src/lodepng.c src/utils.c
	$(CC) -std=c99 -Wall -mwindows -Iinclude -o $@ $^ -lgdi32

clean:
	del /Q src\*.o bitcloak.exe 2>nul || exit 0

