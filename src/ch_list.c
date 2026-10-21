#include "../include/ch_list.h"
#include <stdint.h>
#include <stdlib.h>

struct ch_list_element {
  void* data_;
  struct ch_list_element* next_;
};

static struct ch_list_element* element_at(const ch_list* list, size_t index) {
  struct ch_list_element* element = list->head_;
  while (index-- > 0) {
    element = element->next_;
  }
  return element;
}

void ch_list_init(ch_list* list) {
  if (!list) {
    return;
  }
  list->head_ = NULL;
  list->tail_ = NULL;
  list->size_ = 0;
}

void ch_list_cleanup(ch_list* list) {
  if (!list) {
    return;
  }
  struct ch_list_element* element = list->head_;
  while (element) {
    struct ch_list_element* next = element->next_;
    free(element);
    element = next;
  }
  list->head_ = NULL;
  list->tail_ = NULL;
  list->size_ = 0;
}

bool ch_list_add_at(ch_list* list, void* value) {
  if (!list || list->size_ == SIZE_MAX) {
    return false;
  }

  struct ch_list_element* element = malloc(sizeof(*element));
  if (!element) {
    return false;
  }

  *element = (struct ch_list_element){value, NULL};

  if (list->tail_) {
    list->tail_->next_ = element;
  } else {
    list->head_ = element;
  }

  list->tail_ = element;
  ++list->size_;
  return true;
}

bool ch_list_insert_at(ch_list* list, size_t index, void* value) {
  if (!list || index > list->size_) {
    return false;
  }

  if (index == list->size_) {
    return ch_list_add_at(list, value);
  }

  struct ch_list_element* element = malloc(sizeof(*element));
  if (!element) {
    return false;
  }

  *element = (struct ch_list_element){value, NULL};

  if (index == 0) {
    element->next_ = list->head_;
    list->head_ = element;
  } else {
    struct ch_list_element* previous = element_at(list, index - 1);
    element->next_ = previous->next_;
    previous->next_ = element;
  }

  ++list->size_;
  return true;
}

bool ch_list_remove_at(ch_list* list, size_t index) {
  if (!list || index >= list->size_) {
    return false;
  }
  struct ch_list_element* previous = index ? element_at(list, index - 1) : NULL;
  struct ch_list_element* element = previous ? previous->next_ : list->head_;

  if (previous) {
    previous->next_ = element->next_;
  } else {
    list->head_ = element->next_;
  }

  if (list->tail_ == element) {
    list->tail_ = previous;
  }

  free(element);
  --list->size_;
  return true;
}

bool ch_list_pop(ch_list* list) {
  if (list && list->size_ > 0) {
    return ch_list_remove_at(list, list->size_ - 1);
  }
  return false;
}

bool ch_list_get(const ch_list* list, size_t index, void** output) {
  if (!list || !output || index >= list->size_) {
    return false;
  }
  *output = element_at(list, index)->data_;
  return true;
}

bool ch_list_set(ch_list* list, size_t index, void* value) {
  if (!list || index >= list->size_) {
    return false;
  }
  element_at(list, index)->data_ = value;
  return true;
}

void ch_list_for_each(const ch_list* list, ch_list_operation operation, void* inout) {
  if (!list || !operation) {
    return;
  }
  for (const struct ch_list_element* element = list->head_; element; element = element->next_) {
    operation(element->data_, inout);
  }


bool ch_list_push_front(ch_list* list, void* value) {
    if (!list || list->size_ == SIZE_MAX) {
      return NULL;
    }

    struct ch_list_element* element = malloc(sizeof *element);

    if (!element) {
      return false;
    }

    *element = (struct ch_list_element){.data_ = value, .next_ = NULL};

    if (list->head_) {
      element->next_ = list->head_;
    } else {
      list->head_ = element;
    }

    list->head_ = element;
    ++list->size_;
    return true;
  }


  void *ch_list_find(const ch_list *list, ch_list_match match, const void *key) {
    if (!list || list->size_ == SIZE_MAX) {
      return NULL;
    }

    for (struct ch_list_element *element = list->head_; element; element = element->next_) {
      if (match(element->data_, key)) {
        return element->data_;
      }
    }
  return NULL;

  }

  bool *ch_list_remove(ch_list *list, ch_list_match match, const void *key) {
    if (!list || list->size_ == SIZE_MAX) {
      return false;
    }

    struct ch_list_element* previous = NULL;
    struct ch_list_element* element = list->head_;
    while (element) {
      if (match(element->data_, key)) {
        if (previous == NULL) {
          list->head_ = element->next_;
        } else {
          previous->next_ = element->next_;
        }

        if (element == list->tail_) {
          list->tail_ = previous;
        }

        free(element);
        --list->size_;
        return true;
      }
      previous = element;
      element = element->next_;
    }

    return false;

  }

  size_t ch_list_size(const ch_list* list) {
    if (!list) {
      return 0;
    }
    return list->size_;
  }


}