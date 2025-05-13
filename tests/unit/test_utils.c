#include <assert.h>
#include "utils.h"

void test_current_time_ms() {
    double time1 = current_time_ms();
    double time2 = current_time_ms();
    assert(time2 >= time1); // Ensure time is increasing
}

int main() {
    test_current_time_ms();
    printf("All utils tests passed!\n");
    return 0;
}