#include <stdio.h>

int main(int argc, char **argv) {

    const char* filename = argv[1];

    const FILE* out = fopen(filename, "rb");

    if (out == NULL) {
        perror("fopen");
        return 1;
    }

    return 0;
}