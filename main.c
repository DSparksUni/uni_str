#include <stdio.h>

#define UNI_STR_IMPLEMENTATION
#include "uni_str.h"

int main(size_t argc, char** argv) {
    puts("Args:");
    for(size_t i = 1; i < argc; i++)
        printf("%zu.) %s\n", i, argv[i]);

    return 0;
}