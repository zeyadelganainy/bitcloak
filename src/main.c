// src/main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_io.h"
#include "embed.h"
#include "extract.h"

void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s embed   -i <input.bmp>  -m <message>     -o <output.bmp>\n"
        "  %s extract -i <input.bmp>  -o <output.txt>\n",
        prog, prog);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *cmd = argv[1];
    const char *in_img = NULL;
    const char *out_path = NULL;
    const char *message = NULL;

    // parse flags
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            in_img = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            out_path = argv[++i];
        }
        else if (strcmp(argv[i], "-m") == 0 && i+1 < argc) {
            message = argv[++i];
        }
        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (!in_img || !out_path) {
        fprintf(stderr, "Error: input and output paths are required.\n");
        print_usage(argv[0]);
        return 1;
    }

    // Load image
    int width, height;
    uint8_t *pixels = load_bmp(in_img, &width, &height);
    if (!pixels) {
        fprintf(stderr, "Failed to load image '%s'\n", in_img);
        return 1;
    }
    size_t row_padded = ((width * 3 + 3) & ~3);
    size_t img_size = row_padded * height;

    if (strcmp(cmd, "embed") == 0) {
        if (!message) {
            fprintf(stderr, "Error: embed requires -m <message>\n");
            print_usage(argv[0]);
            free(pixels);
            return 1;
        }
        // Embed and save
        embed_message(pixels, img_size, message);
        save_bmp(out_path, pixels, width, height);
        printf("✅ Embedded \"%s\" into %s\n", message, out_path);
    }
    else if (strcmp(cmd, "extract") == 0) {
        // Extract, print and/or save to file
        char *hidden = extract_message(pixels, img_size);
        printf("🔓 Extracted: %s\n", hidden);

        // Write to output file
        FILE *f = fopen(out_path, "w");
        if (f) {
            fprintf(f, "%s", hidden);
            fclose(f);
            printf("✅ Message written to %s\n", out_path);
        } else {
            perror("Failed to open output file");
        }

        free(hidden);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", cmd);
        print_usage(argv[0]);
        free(pixels);
        return 1;
    }

    free(pixels);
    return 0;
}
