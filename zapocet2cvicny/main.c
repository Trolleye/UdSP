
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    MEAS_FLAG_NONE = 0u,
    MEAS_FLAG_FROST = 1u << 0, /* teplota je zaporna */
    MEAS_FLAG_NATIONAL = 1u << 1, /* nazov ma narodny znak */
    MEAS_FLAG_INVALID = 1u << 2, /* nazov nie je platne UTF-8 */
    MEAS_FLAG_ABOVE_AVG = 1u << 3, /* nad priemerom suboru */
};

#define MEAS_STATION_NAME_LENGTH 32

typedef struct {
    char name[MEAS_STATION_NAME_LENGTH];
    double temperature;
    uint32_t flags;
} MEASUREMENT;

typedef struct QUEUE_NODE {
    struct QUEUE_NODE* next;
    void* data;
} QUEUE_NODE;

typedef struct {
    QUEUE_NODE* head;
    QUEUE_NODE* tail;
} QUEUE;

void sum_temperature(void* data, void* inout) {
    const MEASUREMENT* m = (const MEASUREMENT*)data;
    double* sum = (double*)inout;
    *sum += m->temperature;
}

void mark_above_average(void* data, void* inout) {
    MEASUREMENT* m = (MEASUREMENT*)data;
    double average = *(const double*)inout;
    if (m->temperature > average) {
        m->flags |= MEAS_FLAG_ABOVE_AVG;
    }
}

void print_matching(void* data, void* inout) {
    const MEASUREMENT* m = (const MEASUREMENT*)data;
    uint32_t mask = *(const uint32_t*)inout;
    if ((m->flags & mask) == mask) {
        printf("%s: %.1f [0x%02X]\n", m->name, m->temperature, m->flags);
    }
}

size_t str_copy_word(char* dest, size_t capacity, const char* src) {
    if (capacity == 0) return 0;

    const char* start = src;
    char* d = dest;
    size_t max_chars = capacity - 1;

    while (*src != '\0' && *src != '\n' && *src != ' ') {
        if ((size_t)(d - dest) < max_chars) {
            *d = *src;
            d++;
        }
        src++;
    }

    *d = '\0';

    return (size_t)(src - start);
}

_Bool str_is_valid_utf8(const char* s) {
    if (s == NULL) return false;

    while (*s != '\0') {
        size_t length = 0;

        if ((*s & 0x80) == 0x00) length = 1;
        else if ((*s & 0xE0) == 0xC0) length = 2;
        else if ((*s & 0xF0) == 0xE0) length = 3;
        else if ((*s & 0xF8) == 0xF0) length = 4;
        else return false;

        s++;

        for (size_t i = 1; i < length; i++) {
            if (*s == '\0') return false;
            if ((*s & 0xC0) != 0x80) return false;
            s++;
        }
    }

    return true;
}

uint32_t measurement_flags(const MEASUREMENT* m) {
    if (m == NULL) return 0;
    uint32_t flags = 0;

    if (!str_is_valid_utf8(m->name)) {
        flags |= MEAS_FLAG_INVALID;
    } else {
        const char* p = m->name;
        while (*p != '\0') {
            if ((*p & 0x80) == 0x80) {
                flags |= MEAS_FLAG_NATIONAL;
                break;
            }
            p++;
        }
    }
    if (m->temperature < 0) flags |= MEAS_FLAG_FROST;

    return flags;
}

void queue_init(QUEUE* self) {
    self->head = NULL;
    self->tail = NULL;
}

void queue_destroy(QUEUE* self) {
    if (self == NULL) return;

    QUEUE_NODE* current = self->head;
    while (current != NULL) {
        QUEUE_NODE* next_node = current->next;
        free(current);
        current = next_node;
    }

    self->head = NULL;
    self->tail = NULL;
}

_Bool queue_enqueue(QUEUE* self, void* data) {
    QUEUE_NODE* new_node = malloc(sizeof(QUEUE_NODE));
    if (self == NULL) return false;
    if (new_node == NULL) {
        fprintf(stderr, "queue_enqueue: malloc failed\n");
        exit(1);
    }

    new_node->next = NULL;
    new_node->data = data;

    if (self->head == NULL) {
        self->head = new_node;
    } else {
        self->tail->next = new_node;
    }

    self->tail = new_node;

    return true;
}

void* queue_dequeue(QUEUE* self) {
    if (self->head == NULL) return NULL;
    void* temp_data = self->head->data;
    QUEUE_NODE* new_head = self->head->next;
    if (self->head == self->tail) self->tail = new_head;
    free(self->head);
    self->head = new_head;
    return temp_data;
}

void queue_for_each(const QUEUE* self, void (*operation)(void* data, void* inout), void* inout) {
    if (self == NULL) exit(1);
    QUEUE_NODE* current = self->head;
    while (current != NULL) {
        operation(current->data, inout);
        current = current->next;
    }
}
void process_txt_file(FILE* f, uint32_t mask) {
    if (f == NULL) return;

    QUEUE q;
    queue_init(&q);

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        MEASUREMENT* m = malloc(sizeof(MEASUREMENT));
        if (m == NULL) {
            fprintf(stderr, "process_txt_file: malloc failed\n");
            exit(1);
        }

        size_t copied = str_copy_word(m->name, sizeof(m->name), buffer);

        char* endptr;
        m->temperature = strtod(buffer + copied, &endptr);

        if (endptr == buffer + copied) {
            free(m);
            continue;
        }

        m->flags = measurement_flags(m);

        if (!queue_enqueue(&q, m)) {
            free(m);
            break;
        }
    }

    if (q.head == NULL) {
        printf("Subor neobsahuje merania.\n");
        queue_destroy(&q);
        return;
    }

    double sum = 0.0;
    queue_for_each(&q, sum_temperature, &sum);

    size_t count = 0;
    QUEUE_NODE* current = q.head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    double average = 0;
    if (count > 0) {
        average = sum/count;
    }

    queue_for_each(&q, mark_above_average, &average);

    queue_for_each(&q, print_matching, &mask);

    void* data = NULL;
    while ((data = queue_dequeue(&q)) != NULL) {
        free(data);
    }

    queue_destroy(&q);
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

    uint32_t mask = MEAS_FLAG_ABOVE_AVG;
    if (argc >= 3) {
        mask = (uint32_t)strtoul(argv[2], NULL, 0);
    }

    process_txt_file(f, mask);

    fclose(f);

    return 0;
}