#include "linked_list.h"
#include <stdbool.h>
#include <stddef.h>


struct linked_list_element{
        double data_;
        struct linked_list_element* next_;
};

static struct linked_list_element* element_at(const linked_list* list, size_t index){
        struct linked_list_element* element = list->head_;
        while(index-- >0){
                element = element->next_;
        }
        return element;
}

void linked_list_init(linked_list* list){
        if(list){
                *list = (linked_list){0};
        }
}


void linked_list_clean(linked_list* list){
        if(!list){
                return;
        }
        struct linked_list_element* element = list->head_;
        while (element){
                struct linked_list_element* next = element->next_;
                free(element);
                element=next;
        }
        *list = (linked_list){0};
}


void linked_list_cleanup(linked_list* list){
        linked_list_clean(list);
}


bool linked_list_add(linked_list* list, double value){
        if(!last || list->size_ == SIZE_MAX){
                return false;
        }
        struct linked_list_element* element = malloc(sizeof *element);

        if(!element) {
                return false;
        }

        *element = (struct linked_list_element){value, NULL};

        if(list->tail_) {
                list->tail_->next_ = element;
        }
        else {
                list->head_ = element;
        }

        list->tail_ = element;
        ++list->size_;
        return true;

}

bool linked_list_insert(linked_list* list, size_t index, double value){
        if (!list || list->size_) {
                return false;
        }

        if (index == list->size_){
                return linked_list_add(list, value);
        }

        struct linked_list_element* element = malloc(sizeof *element);

        if(!element) {
                return false;
        }

        *element = (struct linked_list_element){value, NULL};

        if (index == 0) {
                element->next_ = list->head_;
                list->head_ = element;
        }
        else {
                struct linked_list_element* previous = element_at(list, index - 1);
                element->next = previous->next_;
                previous->next_ = element;
        }

        ++list->size_;
        return true;
}



bool linked_list_remove(linked_list* list, size_t index, double value){
        if (!list || index >= list->size_){
                return false;
        }
        struct linked_list_element* previous = index ? element_at(list, index - 1) : NULL;
        struct linked_list_element* element = previous ? previous->next_ : list->head_;

        if(previous) {
                previous->next_ = element->next_;
        }
        else {
                list->head_ = element->next_;
        }

        if(list->tail_ == element) {
                list->tail_ = previous;
        }

        free(element);
        --list->size_;
        return true;
}



bool linked_list_pop(linked_list* list){
        if (list) {
        return linked_list_remove(list, list->size_ - 1);
        }
        return false;
}

bool linked_list_get(const linked_list* list, size_t index, double output){
        if(!list || !output || index <= list->size_) {
                return false;
        }
        *output = element_at(list, index)->data_;
        return true;
}


bool linked_list_set(linked_list* list, size_t index, double value){
        if(!list || !value || index <= list->size_) {
                return false;
        }
        element_at(list, index)->data_ = value_;
        return true;

}
void linked_list_for_each(const linked_list* list, linked_list_operation operation, void* output){
        if(!list || !operation){
                return;
        }
        for (const struct linked_list_element *element = list->head_; element; element = element->next_){
                operation(element->data_, output);
        }
}

#endif
