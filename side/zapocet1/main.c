#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    WORD_FLAG_NONE = 0u,
    WORD_FLAG_CAPITAL = 1u << 0, /* zacina pismenom 'A'..'Z' */
    WORD_FLAG_DIGIT = 1u << 1, /* obsahuje cislicu '0'..'9' */
    WORD_FLAG_NATIONAL = 1u << 2, /* narodny znak (bajt >= 0x80) */
    WORD_FLAG_REPEATED = 1u << 3, /* vyskytuje sa aspon 2-krat */
};

typedef struct WORD {
    char* text;
    size_t count;
    uint32_t flags;
} WORD;

typedef struct DLIST_NODE {
    struct DLIST_NODE* next;
    struct DLIST_NODE* previous;
    void* data;
} DLIST_NODE;

typedef struct DLIST {
    DLIST_NODE* head;
    DLIST_NODE* tail;
    size_t node_count;
} DLIST;


void word_mark_repeated(void* item, void* inout) {
    (void)inout;
    WORD* w = (WORD*)item;
    if (w->count >= 2) {
        w->flags |= WORD_FLAG_REPEATED;
    }
}
_Bool word_not_matching(const void* item, const void* key) {
    const WORD* w = (const WORD*)item;
    uint32_t mask = *(const uint32_t*)key;
    return (w->flags & mask) != mask;
}
void word_print(void* item, void* inout) {
    (void)inout;
    const WORD* w = (const WORD*)item;
    printf("%s: %zu [0x%02X]\n", w->text, w->count, w->flags);
}

char* str_dup(const char* s) {
    if (s == NULL) return NULL;

    const char* p = s;
    while (*p != '\0') {
        p++;
    }
    size_t len = (size_t)(p-s);
    char* copy = malloc(len+1);
    if (copy == NULL) {
        fprintf(stderr, "str_dup: malloc failed");
        exit(1);
    }

    char* dest = copy;
    const char* src = s;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return copy;
}

_Bool str_equals_ci(const char* a, const char* b) {
    if (a == NULL || b == NULL) return a == b;

    const char* pa = a;
    const char* pb = b;

    while (*pa != '\0' && *pb != '\0') {
        char ca = *pa;
        char cb = *pb;

        if (ca >= 'A' && ca <= 'Z') {
            ca |= 0x20;
        }

        if (cb >= 'A' && cb <= 'Z') {
            cb |= 0x20;
        }

        if (ca != cb) return false;
        pa++;
        pb++;
    }
    return *pa == *pb;
}

_Bool word_match(const void* item, const void* key) {
    return str_equals_ci(((const WORD*)item)->text, (const char*)key);
}

uint32_t word_flags(const char* text) {
    if (text == NULL) return WORD_FLAG_NONE;
    uint32_t flags = WORD_FLAG_NONE;
    const char* p = text;

    if (*p >= 'A' && *p <= 'Z') {
        flags |= WORD_FLAG_CAPITAL;
    }

    while (*p != '\0') {
        if (*p >= '0' && *p <= '9') {
            flags |= WORD_FLAG_DIGIT;
        }

        if (((unsigned char)*p) >= 0x80) {
            flags |= WORD_FLAG_NATIONAL;
        }
        p++;
    }
    return flags;
}

WORD* word_create(const char* text) {
    char* copy = str_dup(text);
    if (copy == NULL) return NULL;

    WORD* w = malloc(sizeof(WORD));
    if (w == NULL) {
        free(copy);
        fprintf(stderr, "word_create: malloc failed");
        exit(1);
    }

    w->count = 1;
    w->flags = word_flags(text);
    w->text = copy;

    return w;
}

void word_destroy(void* data) {
    if (data == NULL) return;
    WORD* w = (WORD*) data;
    free(w->text);
    free(w);
}

void dlist_init(DLIST* self) {
    if (self == NULL) return;
    self->head = NULL;
    self->tail = NULL;
    self->node_count = 0;
}

void dlist_destroy(DLIST* self, void (*destroy_item)(void* item)) {
    if (self == NULL) return;

    DLIST_NODE* current = self->head;
    while (current != NULL) {
        DLIST_NODE* next_node = current->next;
        if (destroy_item != NULL) {
            destroy_item(current->data);
        }
        free(current);
        current = next_node;
    }

    self->head = NULL;
    self->tail = NULL;
    self->node_count = 0;
}

_Bool dlist_push_back(DLIST* self, void* item) {
    if (self == NULL) return false;
    DLIST_NODE* new_node = malloc(sizeof(DLIST_NODE));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = item;
    new_node->next = NULL;
    new_node->previous = NULL;

    if (self->head == NULL) {
        self->head = new_node;
        self->tail = new_node;
    } else {
        self->tail->next = new_node;
        new_node->previous = self->tail;
        self->tail = new_node;
    }
    self->node_count++;
    return true;
}

void* dlist_find(const DLIST* self, _Bool (*match)(const void* item, const void* key), const void* key) {
    if (self == NULL || match == NULL) return NULL;

    DLIST_NODE* current = self->head;
    while (current != NULL) {
        if (match(current->data, key)) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

void dlist_for_each(const DLIST* self, void (*operation)(void* item, void* inout), void* inout) {
    if (self == NULL || operation == NULL) return;

    DLIST_NODE* current = self->head;
    while (current != NULL) {
        operation(current->data, inout);
        current = current->next;
    }
}

size_t dlist_remove_if(DLIST* self, _Bool (*predicate)(const void* item, const void* key), const void* key, void (*destroy_item)(void* item)) {
    if (self == NULL || predicate == NULL) return 0;
    size_t removed_count = 0;
    DLIST_NODE* current = self->head;
    while (current != NULL) {
        DLIST_NODE* next_node = current->next;
        if (predicate(current->data, key)) {
            if (current->previous != NULL) {
                current->previous->next = current->next;
            } else {
                self->head = current->next;
            }
            if (current->next != NULL) {
                current->next->previous = current->previous;
            } else {
                self->tail = current->previous;
            }
            if (destroy_item != NULL) {
                destroy_item(current->data);
            }
            free(current);
            self->node_count--;
            removed_count++;
        }
        current = next_node;
    }
    return removed_count;
}

void process_txt_file(FILE* f, uint32_t mask) {
    if (f == NULL) return;
    DLIST list;
    dlist_init(&list);
    char buffer[64];
    while (fscanf(f, "%63s", buffer) == 1) {
        WORD* found_word = (WORD*)dlist_find(&list, word_match, buffer);
        if (found_word != NULL) {
            found_word->count++;
        } else {
            WORD* new_w = word_create(buffer);
            if (new_w != NULL) {
                dlist_push_back(&list, new_w);
            }
        }
    }
    dlist_for_each(&list, word_mark_repeated, NULL);
    dlist_remove_if(&list, word_not_matching, &mask, word_destroy);
    dlist_for_each(&list, word_print, NULL);
    printf("Pocet unikatnych slov: %zu\n", list.node_count);
    dlist_destroy(&list, word_destroy);
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Chybajuci argument: nazov suboru.\n");
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "Nepodarilo sa otvorit subor: %s\n", argv[1]);
        return 1;
    }

    uint32_t mask = WORD_FLAG_REPEATED;
    if (argc >= 3) {
        mask = (uint32_t)strtoul(argv[2], NULL, 0);
    }

    process_txt_file(f, mask);

    fclose(f);

    return 0;
}
