#ifndef CH_LIST_H
#define CH_LIST_H
#include <stdbool.h>
#include <stddef.h>

struct ch_list_element;

typedef struct ch_list {
    struct ch_list_element *head_;
    struct ch_list_element *tail_;
    size_t size_;
} ch_list;

typedef void (*ch_list_operation)(void* data, void* inout);
typedef int (*ch_list_match)(const void *value, const void *key);

void ch_list_init(ch_list* list);
void ch_list_cleanup(ch_list* list);

bool ch_list_add_at(ch_list* list, void* value);
bool ch_list_insert_at(ch_list* list, size_t index, void* value);

bool ch_list_remove_at(ch_list* list, size_t index);
bool ch_list_pop(ch_list* list);

bool ch_list_get(const ch_list* list, size_t index, void** output);
bool ch_list_set(ch_list* list, size_t index, void* value);

void ch_list_for_each(const ch_list* list, ch_list_operation operation, void* inout);

void *ch_list_find(const ch_list *list, ch_list_match match, const void* key);
bool ch_list_remove(ch_list *list, ch_list_match match, const void *key);
bool ch_list_push_front(ch_list *list, void *value);
size_t ch_list_size(const ch_list *list);



#endif