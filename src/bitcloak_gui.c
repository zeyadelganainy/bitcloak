
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include "embed.h"
#include "extract.h"
#include "image_io.h"
#include "crypto.h"
#include "utils.h"

char inputImage[MAX_PATH] = "";
char inputFile[MAX_PATH] = "";
char outputImage[MAX_PATH] = "";
char encryptionKey[256] = "";
int useXor = 0;

void BrowseFile(HWND hwnd, char *buffer, LPCSTR filter) {
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    GetOpenFileName(&ofn);
}

void SaveFile(HWND hwnd, char *buffer, LPCSTR filter) {
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    GetSaveFileName(&ofn);
}

void OnEmbed() {
    int w, h;
    uint8_t *pixels = load_png(inputImage, &w, &h);
    if (!pixels) {
        MessageBox(NULL, "Failed to load input image", "Error", MB_ICONERROR);
        return;
    }

    FILE *fp = fopen(inputFile, "rb");
    if (!fp) {
        MessageBox(NULL, "Failed to open input file", "Error", MB_ICONERROR);
        free(pixels);
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    uint8_t *data = malloc(fsize);
    fread(data, 1, fsize, fp);
    fclose(fp);

    if (strlen(encryptionKey) > 0) {
        if (useXor) {
            xor_cipher(data, fsize, encryptionKey);
        } else {
            uint8_t *enc = NULL;
            size_t enc_len;
            if (aes_encrypt(data, fsize, encryptionKey, &enc, &enc_len) == 0) {
                free(data);
                data = enc;
                fsize = enc_len;
            }
        }
    }

    const char *filename = strrchr(inputFile, '\\');
    filename = filename ? filename + 1 : inputFile;
    size_t img_size = w * h * 3;
    embed_file(pixels, img_size, data, fsize, filename);
    save_png(outputImage, pixels, w, h);

    free(pixels);
    free(data);
    MessageBox(NULL, "File embedded successfully", "Success", MB_OK);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: BrowseFile(hwnd, inputImage, "PNG Files\0*.png\0"); break;
        case 2: BrowseFile(hwnd, inputFile, "All Files\0*.*\0"); break;
        case 3: SaveFile(hwnd, outputImage, "PNG Files\0*.png\0"); break;
        case 4: OnEmbed(); break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BitcloakGUI";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "BitCloak - Embed File", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

    CreateWindow("BUTTON", "Browse Image", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        20, 20, 120, 24, hwnd, (HMENU)1, hInstance, NULL);
    CreateWindow("BUTTON", "Browse File", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        20, 50, 120, 24, hwnd, (HMENU)2, hInstance, NULL);
    CreateWindow("BUTTON", "Save Output", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        20, 80, 120, 24, hwnd, (HMENU)3, hInstance, NULL);
    CreateWindow("BUTTON", "Embed", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        20, 120, 120, 30, hwnd, (HMENU)4, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
