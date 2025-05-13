
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <string.h>
#include "embed.h"
#include "extract.h"
#include "image_io.h"
#include "crypto.h"
#include "utils.h"

#define ID_HOME_EMBED 101
#define ID_HOME_EXTRACT 102
#define ID_EMBED_IMAGE 201
#define ID_EMBED_FILE 202
#define ID_EMBED_OUTPUT 203
#define ID_EMBED_BACK 204
#define ID_EMBED_RUN 205
#define ID_EXTRACT_IMAGE 301
#define ID_EXTRACT_OUTPUT 302
#define ID_EXTRACT_BACK 303
#define ID_EXTRACT_RUN 304

HWND hwndMain, embedWidgets[10], extractWidgets[10], homeWidgets[2];
HWND hKeyInputEmbed, hKeyInputExtract, hUseXorEmbed, hUseXorExtract;
HWND hStatusImageEmbed, hStatusFileEmbed, hStatusImageExtract;
char inputImage[MAX_PATH] = "", inputFile[MAX_PATH] = "", outputFile[MAX_PATH] = "", encryptionKey[256] = "";
int useXor = 0;

void ToggleWidgets(HWND* widgets, int count, BOOL show) {
    for (int i = 0; i < count; ++i) {
        ShowWindow(widgets[i], show ? SW_SHOW : SW_HIDE);
    }
}

void BrowseFile(HWND hwnd, char* buffer, LPCSTR filter, HWND statusLabel) {
    OPENFILENAME ofn = { sizeof(ofn) };
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn)) {
        char msg[512];
        sprintf(msg, "%s uploaded successfully", buffer);
        SetWindowText(statusLabel, msg);
        
    }
}

void SaveFile(HWND hwnd, char* buffer, LPCSTR filter) {
    OPENFILENAME ofn = { sizeof(ofn) };
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    GetSaveFileName(&ofn);
}

void InitGUI(HWND hwnd, HINSTANCE hInstance) {
    hwndMain = hwnd;

    // Welcome screen
    homeWidgets[0] = CreateWindow("BUTTON", "Embed", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        150, 80, 200, 40, hwnd, (HMENU)ID_HOME_EMBED, hInstance, NULL);
    homeWidgets[1] = CreateWindow("BUTTON", "Extract", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        150, 140, 200, 40, hwnd, (HMENU)ID_HOME_EXTRACT, hInstance, NULL);

    // Embed widgets
    embedWidgets[0] = CreateWindow("BUTTON", "Browse Image", WS_CHILD, 20, 20, 120, 24, hwnd, (HMENU)ID_EMBED_IMAGE, hInstance, NULL);
    hStatusImageEmbed = embedWidgets[1] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 150, 20, 300, 20, hwnd, NULL, hInstance, NULL);
    embedWidgets[2] = CreateWindow("BUTTON", "Browse File", WS_CHILD, 20, 50, 120, 24, hwnd, (HMENU)ID_EMBED_FILE, hInstance, NULL);
    hStatusFileEmbed = embedWidgets[3] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 150, 50, 300, 20, hwnd, NULL, hInstance, NULL);
    embedWidgets[4] = CreateWindow("STATIC", "Key:", WS_CHILD, 20, 90, 60, 20, hwnd, NULL, hInstance, NULL);
    hKeyInputEmbed = embedWidgets[5] = CreateWindow("EDIT", "", WS_CHILD | WS_BORDER, 80, 90, 150, 24, hwnd, NULL, hInstance, NULL);
    hUseXorEmbed = embedWidgets[6] = CreateWindow("BUTTON", "Use XOR", WS_CHILD | BS_AUTOCHECKBOX, 240, 90, 100, 24, hwnd, NULL, hInstance, NULL);
    embedWidgets[7] = CreateWindow("BUTTON", "Save Output", WS_CHILD, 20, 130, 120, 24, hwnd, (HMENU)ID_EMBED_OUTPUT, hInstance, NULL);
    embedWidgets[8] = CreateWindow("BUTTON", "Embed", WS_CHILD, 150, 170, 100, 30, hwnd, (HMENU)ID_EMBED_RUN, hInstance, NULL);
    embedWidgets[9] = CreateWindow("BUTTON", "Back", WS_CHILD, 270, 170, 100, 30, hwnd, (HMENU)ID_EMBED_BACK, hInstance, NULL);
    ToggleWidgets(embedWidgets, 10, FALSE);

    // Extract widgets
    extractWidgets[0] = CreateWindow("BUTTON", "Browse Image", WS_CHILD, 20, 20, 120, 24, hwnd, (HMENU)ID_EXTRACT_IMAGE, hInstance, NULL);
    hStatusImageExtract = extractWidgets[1] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 150, 20, 300, 20, hwnd, NULL, hInstance, NULL);
    extractWidgets[2] = CreateWindow("STATIC", "Key:", WS_CHILD, 20, 60, 60, 20, hwnd, NULL, hInstance, NULL);
    hKeyInputExtract = extractWidgets[3] = CreateWindow("EDIT", "", WS_CHILD | WS_BORDER, 80, 60, 150, 24, hwnd, NULL, hInstance, NULL);
    hUseXorExtract = extractWidgets[4] = CreateWindow("BUTTON", "Use XOR", WS_CHILD | BS_AUTOCHECKBOX, 240, 60, 100, 24, hwnd, NULL, hInstance, NULL);
    extractWidgets[5] = CreateWindow("BUTTON", "Save Output", WS_CHILD, 20, 100, 120, 24, hwnd, (HMENU)ID_EXTRACT_OUTPUT, hInstance, NULL);
    extractWidgets[6] = CreateWindow("BUTTON", "Extract", WS_CHILD, 150, 140, 100, 30, hwnd, (HMENU)ID_EXTRACT_RUN, hInstance, NULL);
    extractWidgets[7] = CreateWindow("BUTTON", "Back", WS_CHILD, 270, 140, 100, 30, hwnd, (HMENU)ID_EXTRACT_BACK, hInstance, NULL);
    ToggleWidgets(extractWidgets, 8, FALSE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        InitGUI(hwnd, ((LPCREATESTRUCT)lParam)->hInstance);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_HOME_EMBED:
            ToggleWidgets(homeWidgets, 2, FALSE);
            ToggleWidgets(extractWidgets, 8, FALSE);
            ToggleWidgets(embedWidgets, 10, TRUE);
            break;
        case ID_HOME_EXTRACT:
            ToggleWidgets(homeWidgets, 2, FALSE);
            ToggleWidgets(embedWidgets, 10, FALSE);
            ToggleWidgets(extractWidgets, 8, TRUE);
            break;
        case ID_EMBED_BACK:
            ToggleWidgets(embedWidgets, 10, FALSE);
            ToggleWidgets(extractWidgets, 8, FALSE);
            ToggleWidgets(homeWidgets, 2, TRUE);
            break;
        case ID_EXTRACT_BACK:
            ToggleWidgets(extractWidgets, 8, FALSE);
            ToggleWidgets(embedWidgets, 10, FALSE);
            ToggleWidgets(homeWidgets, 2, TRUE);
            break;
        case ID_EMBED_IMAGE:
            BrowseFile(hwnd, inputImage, "PNG Files\0*.png\0", hStatusImageEmbed);
            break;
        case ID_EMBED_FILE:
            BrowseFile(hwnd, inputFile, "All Files\0*.*\0", hStatusFileEmbed);
            break;
        case ID_EXTRACT_IMAGE:
            BrowseFile(hwnd, inputImage, "PNG Files\0*.png\0", hStatusImageExtract);
            break;
        case ID_EMBED_OUTPUT:
        case ID_EXTRACT_OUTPUT:
            SaveFile(hwnd, outputFile, "All Files\0*.*\0");
            break;
        case ID_EMBED_RUN:
        case ID_EXTRACT_RUN:
            MessageBox(hwnd, "Run operation would go here.", "Info", MB_OK);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BitCloakWindow";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Welcome to BitCloak", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL);
    if (!hwnd) return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
