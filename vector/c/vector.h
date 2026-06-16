#pragma once
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct vector {
  size_t size;
  void *memory;
  size_t data_size;
  size_t capacity;
  bool (*find_func)(void *to_compare1, void *to_compare2);
  pthread_rwlock_t lock;
};


enum STATUS { OK, ALLOC_ERROR, NULL_PTR, OUT_OF_BOUNDS, MISSING_ARG };

enum STATUS vector_find(struct vector *vector, void *element_to_find,
                        int *index_found);

enum STATUS vector_init(struct vector **vector, size_t data_size,
                        size_t capacity);

enum STATUS vector_delete(struct vector *vector, int index,
                          void (*delete_func)(void *to_delete));

enum STATUS vector_add_element_at_index(struct vector *vector, void *element,
                                        int index);

enum STATUS vector_push(struct vector *vector, void *element);
enum STATUS vector_push_ts(struct vector *vector, void *element);

enum STATUS vector_push_front(struct vector *vector, void *element);

enum STATUS vector_get_element_by_index(struct vector *vector, int index,
                                        void **found);

enum STATUS vector_shift_left(struct vector *vector);

enum STATUS vector_shift_right(struct vector *vector);

enum STATUS vector_free(struct vector *vector);
//THREAD SAFE
//
enum STATUS vector_push_ts(struct vector *vector, void *element);

enum STATUS vector_find_ts(struct vector *vector, void *element_to_find,
                           int *index_found) ;
enum STATUS vector_add_element_at_index_ts(struct vector *vector, void *element,
                                           int index) ;
enum STATUS vector_push_front_ts(struct vector *vector, void *element);

enum STATUS vector_delete_ts(struct vector *vector, int index,
                          void (*delete_func)(void *to_delete));

enum STATUS vector_shift_left_ts(struct vector *vector);

enum STATUS vector_shift_right_ts(struct vector *vector);

enum STATUS vector_free_ts(struct vector *vector);

enum STATUS vector_get_element_by_index_ts(struct vector *vector, int index,
                                           void **found);
