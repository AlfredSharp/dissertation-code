#include <stdio.h>
#include <stdlib.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "LeapC.h"
#include "ExampleConnection.h"

int main(int argc, char** argv) {
    int64_t time = LeapGetNow();
    printf("%" PRId64 "\n", time);
    return 0;
}
