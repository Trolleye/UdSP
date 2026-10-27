#include <stdio.h>
#include <stdlib.h>

#include "ch_hex.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Fixed: Added space and asterisk for pointer type
    const char *filename = argv[1];

    // Fixed: Removed 'const' because file streams must be modifiable by I/O functions
    FILE *out = fopen(filename, "rb");

    if (out == NULL) {
        perror("fopen");
        return 1;
    }

    if (fseek(out, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(out);
        return 1;
    }

    long size = ftell(out);
    if (size < 0) {
        perror("ftell");
        fclose(out);
        return 1;
    }

    rewind(out);

    // Fixed: Added asterisk for pointer type
    unsigned char *data = (unsigned char *)malloc(size);
    if (data == NULL) {
        perror("malloc");
        fclose(out);
        return 1;
    }

    size_t bytes_read = fread(data, 1, size, out);

    ch_hexdump(data, bytes_read);

    fclose(out);
    free(data);

    return 0;
}