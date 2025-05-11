// image_io.c
#include "image_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1) // ensure struct byte alignment
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1, bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

uint8_t* load_bmp(const char *filename, int *width, int *height) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Error opening BMP");
        return NULL;
    }

    BITMAPFILEHEADER fileHeader;
    fread(&fileHeader, sizeof(fileHeader), 1, f);
    if (fileHeader.bfType != 0x4D42) {
        fprintf(stderr, "Not a BMP file.\n");
        fclose(f);
        return NULL;
    }

    BITMAPINFOHEADER infoHeader;
    fread(&infoHeader, sizeof(infoHeader), 1, f);

    if (infoHeader.biBitCount != 24 || infoHeader.biCompression != 0) {
        fprintf(stderr, "Only uncompressed 24-bit BMP supported.\n");
        fclose(f);
        return NULL;
    }

    *width = infoHeader.biWidth;
    *height = infoHeader.biHeight;

    size_t row_padded = ((*width * 3 + 3) & (~3)); // align to 4 bytes
    size_t size = row_padded * (*height);

    uint8_t *raw = malloc(size);
    fseek(f, fileHeader.bfOffBits, SEEK_SET);
    fread(raw, 1, size, f);
    fclose(f);

    return raw;
}

void save_bmp(const char *filename, const uint8_t *data, int width, int height) {
    FILE *f_in = fopen("assets/cover.bmp", "rb");  // open original for headers
    if (!f_in) {
        perror("Failed to reopen original BMP for writing");
        exit(1);
    }

    FILE *f_out = fopen(filename, "wb");
    if (!f_out) {
        perror("Failed to open output BMP");
        fclose(f_in);
        exit(1);
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    fread(&fileHeader, sizeof(fileHeader), 1, f_in);
    fread(&infoHeader, sizeof(infoHeader), 1, f_in);

    fwrite(&fileHeader, sizeof(fileHeader), 1, f_out);
    fwrite(&infoHeader, sizeof(infoHeader), 1, f_out);

    size_t row_padded = ((width * 3 + 3) & ~3); // 4-byte alignment
    size_t data_size = row_padded * height;

    fwrite(data, 1, data_size, f_out);

    fclose(f_in);
    fclose(f_out);
}
