
#include "utils.h"
#include <windows.h>
#include <time.h>
#include <string.h>

void xor_cipher(uint8_t *data, size_t len, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < len; ++i) {
        data[i] ^= key[i % key_len];
    }
}


double current_time_ms() {
    static LARGE_INTEGER frequency;
    static int initialized = 0;
    LARGE_INTEGER counter;

    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }

    QueryPerformanceCounter(&counter);
    return (double)(counter.QuadPart * 1000.0 / frequency.QuadPart);
}
