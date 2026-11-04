
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    LINE_FLAG_NONE = 0u,
    LINE_FLAG_LONG = 1u << 0,     /* at least 5 characters */
    LINE_FLAG_DIGIT = 1u << 1,    /* contains digit '0'..'9' */
    LINE_FLAG_NATIONAL = 1u << 2, /* contains byte >= 0x80 */
    LINE_FLAG_UPPER = 1u << 3,    /* starts with 'A'..'Z' */
};

typedef struct DYN_STRING {
    char* data;
    size_t capacity;
    size_t size;
} DYN_STRING;

typedef struct STACK_NODE {
    struct STACK_NODE* next;
    void* data;
} STACK_NODE;

typedef struct STACK {
    STACK_NODE* top;
    size_t data_size;
} STACK;

#define DYN_STRING_DEF_CAPACITY 8

/* DYNAMIC STRING */

void dyn_string_init(DYN_STRING* self) {
    self->capacity = DYN_STRING_DEF_CAPACITY;
    self->size = 0;

    self->data = malloc(self->capacity + 1);

    if (self->data == NULL) {
        fprintf(stderr, "dyn_string_init: malloc failed\n");
        exit(1);
    }

    self->data[0] = '\0';
}

void dyn_string_destroy(DYN_STRING* self) {
    free(self->data);

    self->data = NULL;
    self->capacity = 0;
    self->size = 0;
}

void dyn_string_append_char(DYN_STRING* self, char c) {
    if (self->size + 1 > self->capacity) {
        size_t new_capacity = self->capacity * 2;

        char* new_data = realloc(self->data, new_capacity + 1);

        if (new_data == NULL) {
            fprintf(stderr, "dyn_string_append_char: realloc failed\n");
            exit(1);
        }

        self->data = new_data;
        self->capacity = new_capacity;
    }

    self->data[self->size] = c;
    self->size++;

    self->data[self->size] = '\0';
}

size_t dyn_string_utf8_length(const DYN_STRING* self) {
    size_t count = 0;

    const unsigned char* p =
        (const unsigned char*)self->data;

    while (*p != '\0') {
        if ((*p & 0xC0) != 0x80) {
            count++;
        }

        p++;
    }

    return count;
}

const char* dyn_string_get_data(const DYN_STRING* self) {
    return self->data;
}

/* LINE FLAGS */

uint32_t line_flags(const DYN_STRING* line) {
    uint32_t flags = LINE_FLAG_NONE;

    const unsigned char* p =
        (const unsigned char*)line->data;

    if (dyn_string_utf8_length(line) >= 5) {
        flags |= LINE_FLAG_LONG;
    }

    if (*p >= 'A' && *p <= 'Z') {
        flags |= LINE_FLAG_UPPER;
    }

    while (*p != '\0') {
        if (*p >= '0' && *p <= '9') {
            flags |= LINE_FLAG_DIGIT;
        }

        if (*p >= 0x80) {
            flags |= LINE_FLAG_NATIONAL;
        }

        p++;
    }

    return flags;
}

_Bool flags_test_all(uint32_t flags, uint32_t mask) {
    return (flags & mask) == mask;
}

/* STACK */

void stack_init(STACK* self, size_t data_size) {
    self->top = NULL;
    self->data_size = data_size;
}

void stack_destroy(STACK* self) {
    STACK_NODE* node = self->top;

    while (node != NULL) {
        STACK_NODE* next = node->next;

        free(node->data);
        free(node);

        node = next;
    }

    self->top = NULL;
}

void stack_push(STACK* self, const void* data) {
    STACK_NODE* node = malloc(sizeof(STACK_NODE));

    if (node == NULL) {
        fprintf(stderr, "stack_push: malloc failed\n");
        exit(1);
    }

    node->data = malloc(self->data_size);

    if (node->data == NULL) {
        fprintf(stderr, "stack_push: malloc failed\n");
        free(node);
        exit(1);
    }

    memcpy(node->data, data, self->data_size);

    node->next = self->top;
    self->top = node;
}

_Bool stack_pop(STACK* self, void* data) {
    STACK_NODE* node = self->top;

    if (node == NULL) {
        return false;
    }

    memcpy(data, node->data, self->data_size);

    self->top = node->next;

    free(node->data);
    free(node);

    return true;
}

/* PROCESS LINE */

void process_line(STACK* stack, DYN_STRING* line, uint32_t mask) {
    if (flags_test_all(line_flags(line), mask)) {
        stack_push(stack, line);
    } else {
        dyn_string_destroy(line);
    }

    dyn_string_init(line);
}

/* PROCESS FILE */

void process_txt_file(FILE* f, uint32_t mask) {
    STACK stack;
    DYN_STRING line;

    int c;

    stack_init(&stack, sizeof(DYN_STRING));
    dyn_string_init(&line);

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            process_line(&stack, &line, mask);
        } else {
            dyn_string_append_char(&line, (char)c);
        }
    }

    /* Process the final line if it has content. */
    if (line.size > 0) {
        process_line(&stack, &line, mask);
    }

    dyn_string_destroy(&line);

    /* Print lines in reverse order. */
    while (stack_pop(&stack, &line)) {
        printf("%s (0x%02X)\n",
               dyn_string_get_data(&line),
               (unsigned int)line_flags(&line));

        dyn_string_destroy(&line);
    }

    stack_destroy(&stack);
}

/* MAIN */

int main(int argc, char* argv[]) {
    FILE* f;
    uint32_t mask = LINE_FLAG_NONE;

    if (argc < 2) {
        fprintf(stderr, "usage: dyn_string filename [mask]\n");
        return 1;
    }

    f = fopen(argv[1], "r");

    if (f == NULL) {
        perror("fopen");
        return 1;
    }

    if (argc >= 3) {
        mask = (uint32_t)strtoul(argv[2], NULL, 0);
    }

    process_txt_file(f, mask);

    fclose(f);

    return 0;
}
