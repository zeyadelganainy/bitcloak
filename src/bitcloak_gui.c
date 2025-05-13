#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <string.h>

// Define control IDs
#define ID_HOME_EMBED 101
#define ID_HOME_EXTRACT 102
#define ID_EMBED_IMAGE 201
#define ID_EMBED_FILE 202
#define ID_EMBED_ENCRYPTION_NONE 203
#define ID_EMBED_ENCRYPTION_XOR 204
#define ID_EMBED_ENCRYPTION_AES 205
#define ID_EMBED_PASSPHRASE 206
#define ID_EMBED_RUN 207
#define ID_EMBED_BACK 208

// Global variables
HWND hwndMain, homeWidgets[2], embedWidgets[8];
char inputImage[MAX_PATH] = "", inputFile[MAX_PATH] = "", outputFile[MAX_PATH] = "", passphrase[256] = "";
int encryptionType = 0; // 0 = None, 1 = XOR, 2 = AES

// Function to toggle visibility of widgets
void ToggleWidgets(HWND* widgets, int count, BOOL show) {
    for (int i = 0; i < count; ++i) {
        ShowWindow(widgets[i], show ? SW_SHOW : SW_HIDE);
    }
}

// Function to browse for a file
void BrowseFile(HWND hwnd, char* buffer, LPCSTR filter) {
    OPENFILENAME ofn = { sizeof(ofn) };
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    GetOpenFileName(&ofn);
}

// Function to run CLI commands
void RunCLICommand(const char* command) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcess(NULL, (LPSTR)command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        MessageBox(NULL, "Failed to run CLI command.", "Error", MB_OK | MB_ICONERROR);
    }
}

// Initialize GUI components
void InitGUI(HWND hwnd, HINSTANCE hInstance) {
    hwndMain = hwnd;

    // Home screen
    homeWidgets[0] = CreateWindow("BUTTON", "Embed", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        150, 80, 200, 40, hwnd, (HMENU)ID_HOME_EMBED, hInstance, NULL);
    homeWidgets[1] = CreateWindow("BUTTON", "Extract", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        150, 140, 200, 40, hwnd, (HMENU)ID_HOME_EXTRACT, hInstance, NULL);

    // Embed screen
    embedWidgets[0] = CreateWindow("BUTTON", "Select PNG", WS_CHILD, 20, 20, 120, 24, hwnd, (HMENU)ID_EMBED_IMAGE, hInstance, NULL);
    embedWidgets[1] = CreateWindow("BUTTON", "Select File", WS_CHILD, 20, 60, 120, 24, hwnd, (HMENU)ID_EMBED_FILE, hInstance, NULL);
    embedWidgets[2] = CreateWindow("BUTTON", "None", WS_CHILD | BS_AUTORADIOBUTTON, 20, 100, 100, 24, hwnd, (HMENU)ID_EMBED_ENCRYPTION_NONE, hInstance, NULL);
    embedWidgets[3] = CreateWindow("BUTTON", "XOR", WS_CHILD | BS_AUTORADIOBUTTON, 130, 100, 100, 24, hwnd, (HMENU)ID_EMBED_ENCRYPTION_XOR, hInstance, NULL);
    embedWidgets[4] = CreateWindow("BUTTON", "AES", WS_CHILD | BS_AUTORADIOBUTTON, 240, 100, 100, 24, hwnd, (HMENU)ID_EMBED_ENCRYPTION_AES, hInstance, NULL);
    embedWidgets[5] = CreateWindow("EDIT", "", WS_CHILD | WS_BORDER, 20, 140, 200, 24, hwnd, (HMENU)ID_EMBED_PASSPHRASE, hInstance, NULL);
    embedWidgets[6] = CreateWindow("BUTTON", "Embed", WS_CHILD, 20, 180, 120, 30, hwnd, (HMENU)ID_EMBED_RUN, hInstance, NULL);
    embedWidgets[7] = CreateWindow("BUTTON", "Back", WS_CHILD, 150, 180, 120, 30, hwnd, (HMENU)ID_EMBED_BACK, hInstance, NULL);
    ToggleWidgets(embedWidgets, 8, FALSE);
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        InitGUI(hwnd, ((LPCREATESTRUCT)lParam)->hInstance);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_HOME_EMBED:
            ToggleWidgets(homeWidgets, 2, FALSE);
            ToggleWidgets(embedWidgets, 8, TRUE);
            break;
        case ID_EMBED_BACK:
            SetWindowText(embedWidgets[5], ""); // Clear passphrase field
            ToggleWidgets(embedWidgets, 8, FALSE);
            ToggleWidgets(homeWidgets, 2, TRUE);
            break;
        case ID_EMBED_IMAGE:
            BrowseFile(hwnd, inputImage, "PNG Files\0*.png\0");
            MessageBox(hwnd, inputImage, "Selected PNG", MB_OK);
            break;
        case ID_EMBED_FILE:
            BrowseFile(hwnd, inputFile, "All Files\0*.*\0");
            MessageBox(hwnd, inputFile, "Selected File", MB_OK);
            break;
        case ID_EMBED_ENCRYPTION_NONE:
            encryptionType = 0;
            break;
        case ID_EMBED_ENCRYPTION_XOR:
            encryptionType = 1;
            break;
        case ID_EMBED_ENCRYPTION_AES:
            encryptionType = 2;
            break;
        case ID_EMBED_RUN:
            GetWindowText(embedWidgets[5], passphrase, sizeof(passphrase));
            snprintf(outputFile, sizeof(outputFile), "output.png"); // Default output file
            char command[1024];
            snprintf(command, sizeof(command),
                "bitcloak.exe embed --image \"%s\" --file \"%s\" --output \"%s\" --encryption %s --passphrase \"%s\"",
                inputImage, inputFile, outputFile,
                encryptionType == 0 ? "none" : (encryptionType == 1 ? "xor" : "aes"),
                passphrase);

            RunCLICommand(command);
            MessageBox(hwnd, "Embedding process completed!", "Success", MB_OK);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Entry point
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
