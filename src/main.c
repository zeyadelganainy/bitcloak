
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "image_io.h"
#include "embed.h"
#include "extract.h"
#include "utils.h"

void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s embed       -i <input.bmp>  -m <message>     -o <output.bmp>\n"
        "  %s embedfile   -i <input.bmp>  -f <inputfile>   -o <output.bmp> [--key <passphrase>]\n"
        "  %s extract     -i <input.bmp>  -o <output.txt>\n"
        "  %s extractfile -i <input.bmp>  -o <outputfile|-] [--key <passphrase>]\n",
        prog, prog, prog, prog);
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
    const char *file_path = NULL;
    const char *encryption_key = NULL;

    // Parse CLI arguments
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
        else if (strcmp(argv[i], "-f") == 0 && i+1 < argc) {
            file_path = argv[++i];
        }
        else if (strcmp(argv[i], "--key") == 0 && i+1 < argc) {
            encryption_key = argv[++i];
        }
        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (!in_img || !out_path) {
        fprintf(stderr, "Error: -i <input> and -o <output> are required.\n");
        print_usage(argv[0]);
        return 1;
    }

    // Load input image
    int width, height;
    uint8_t *pixels = load_bmp(in_img, &width, &height);
    if (!pixels) {
        fprintf(stderr, "Failed to load image: %s\n", in_img);
        return 1;
    }

    size_t row_padded = ((width * 3 + 3) & ~3);
    size_t img_size = row_padded * height;

    // Handle commands
    if (strcmp(cmd, "embed") == 0) {
        if (!message) {
            fprintf(stderr, "Error: embed requires -m <message>\n");
            return 1;
        }
        embed_message(pixels, img_size, message);
        save_bmp(out_path, pixels, width, height);
        printf("✅ Embedded message into %s\n", out_path);
    }
    else if (strcmp(cmd, "extract") == 0) {
        char *extracted = extract_message(pixels, img_size);
        printf("🔓 Extracted message: %s\n", extracted);
        FILE *f = fopen(out_path, "w");
        if (f) {
            fprintf(f, "%s", extracted);
            fclose(f);
            printf("✅ Message written to %s\n", out_path);
        } else {
            perror("Failed to write extracted message");
        }
        free(extracted);
    }
    else if (strcmp(cmd, "embedfile") == 0) {
        if (!file_path) {
            fprintf(stderr, "Error: embedfile requires -f <file>\n");
            return 1;
        }

        FILE *fp = fopen(file_path, "rb");
        if (!fp) {
            perror("Failed to open file to embed");
            return 1;
        }

        fseek(fp, 0, SEEK_END);
        size_t fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        uint8_t *fdata = malloc(fsize);
        fread(fdata, 1, fsize, fp);
        fclose(fp);

        if (encryption_key) {
            xor_cipher(fdata, fsize, encryption_key);
            printf("🔒 Encrypted file with XOR key.\n");
        }

        const char *filename = strrchr(file_path, '/');
        if (!filename) filename = strrchr(file_path, '\\');
        if (!filename) filename = file_path;
        else filename++; // skip slash

        embed_file(pixels, img_size, fdata, fsize, filename);
        save_bmp(out_path, pixels, width, height);
        printf("✅ Embedded file '%s' into %s (%zu bytes)\n", filename, out_path, fsize);
        free(fdata);
    }
    else if (strcmp(cmd, "extractfile") == 0) {
        size_t fsize;
        char recovered_name[256];
        uint8_t *data = extract_file(pixels, img_size, &fsize, recovered_name);

        if (encryption_key) {
            xor_cipher(data, fsize, encryption_key);
            printf("🔓 Decrypted file with XOR key.\n");
        }

        const char *final_output = (strcmp(out_path, "-") == 0) ? recovered_name : out_path;
        FILE *fp = fopen(final_output, "wb");
        if (!fp) {
            perror("Failed to write extracted file");
            return 1;
        }

        fwrite(data, 1, fsize, fp);
        fclose(fp);
        printf("✅ Extracted file to %s (%zu bytes)\n", final_output, fsize);
        free(data);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", cmd);
        print_usage(argv[0]);
        return 1;
    }

    free(pixels);
    return 0;
}
