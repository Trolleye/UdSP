#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdbool.h>
#include <stddef.h>


struct linked_list_element;

typedef struct linked_list {
        struct linked_list_element *head_;
        struct linked_list_element *tail_;
        size_t size_;
} linked_list;


typedef void (*linked_list_operatin)(double data, void* output);


void linked_list_init(linked_list* list);

void linked_list_cleanup(linked_list* list);
void linked_list_clean(linked_list* list);

bool linked_list_add(linked_list* list, double value);
bool linked_list_insert(linked_list* list, size_t index, double value);

bool linked_list_pop(linked_list* list);
bool linked_list_remove(linked_list* list, size_t index, double value);

bool linked_list_get(const linked_list* list, size_t index, double output);
bool linked_list_set(linked_list* list, size_t index, double value);

void linked_list_for_each(const linked_list* list, linked_list_operation operation, void* output);

#endif
