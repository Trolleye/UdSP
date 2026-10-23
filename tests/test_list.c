#include <stdio.h>
#include "../include/ch_buf.h"
#include "../include/ch_list.h"
#include "ch_test.h"

int main(void) {
    ch_list list;
    void* out = NULL;
    ch_list_init(&list);

    CH_CHECK(ch_list_size(&list) == 0);

    CH_CHECK(ch_list_add_at(&list, &(int){20}) == true);
    CH_CHECK(ch_list_push_front(&list, &(int){10}) == true);
    CH_CHECK(ch_list_size(&list) == 2);

    CH_CHECK(ch_list_insert_at(&list, 2, &(int){40}) == true);
    CH_CHECK(ch_list_insert_at(&list, 2, &(int){30}) == true);
    CH_CHECK(ch_list_size(&list) == 4);

    CH_CHECK(ch_list_get(&list, 2, &out) == true);
    CH_CHECK(out != NULL && *(int*)out == 30);

    CH_CHECK(ch_list_set(&list, 2, &(int){99}) == true);
    CH_CHECK(ch_list_get(&list, 2, &out) == true);
    CH_CHECK(*(int*)out == 99);

    CH_CHECK(ch_list_remove_at(&list, 0) == true);
    CH_CHECK(ch_list_size(&list) == 3);

    CH_CHECK(ch_list_pop(&list) == true);
    CH_CHECK(ch_list_size(&list) == 2);

    ch_list_cleanup(&list);
    CH_CHECK(ch_list_size(&list) == 0);

    return ch_test_result();
}