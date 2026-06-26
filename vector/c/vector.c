#include "vector.h"
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static enum STATUS expand_capacity(struct vector *vector) {
  void *expand =
      realloc(vector->memory, (vector->capacity * 2) * vector->data_size);
  if (expand == NULL) {
    return ALLOC_ERROR;
  }
  vector->memory = expand;

  vector->capacity *= 2;
  return OK;
}


enum STATUS vector_find(struct vector *vector, void *element_to_find,
                        int *index_found) {
  if (vector == NULL || element_to_find == NULL || index_found == NULL) {
    return NULL_PTR;
  }
  if (vector->find_func == NULL) {
    return MISSING_ARG;
  }
  for (size_t i = 0; i < vector->size; i++) {
    if (vector->find_func((char *)vector->memory + (i * vector->data_size),
                          element_to_find)) {
      *index_found = i;
      return OK;
    }
  }
  *index_found = -1;
  return OK;
}

enum STATUS vector_init(struct vector **vector, size_t data_size,
                        size_t capacity) {

  if (vector == NULL) {
    return NULL_PTR;
  }
  void *vector_heap = malloc(sizeof(struct vector));
  if (vector_heap == NULL) {
    return ALLOC_ERROR;
  }
  *vector = vector_heap;
  (*vector)->capacity = capacity == 0 ? 10 : capacity;
  (*vector)->size = 0;
  (*vector)->data_size = data_size;
  (*vector)->find_func = NULL;
  pthread_rwlock_init(&(*vector)->lock, NULL);

  void *vector_memory = malloc((*vector)->capacity * data_size);
  if (vector_memory == NULL) {
    free(*vector);
    return ALLOC_ERROR;
  }

  (*vector)->memory = vector_memory;
  return OK;
}

enum STATUS vector_delete(struct vector *vector, int index,
                          void (*delete_func)(void *to_delete)) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  if (index < 0 || index >= vector->size) {
    return OUT_OF_BOUNDS;
  }
  if (delete_func != NULL) {
    delete_func((char *)vector->memory + (index * vector->data_size));
  }

  memmove((char*)vector->memory + ((index) * vector->data_size), (char*)vector->memory + ((index+1) * vector->data_size),(vector->data_size * (vector->size-index-1)));


  vector->size--;

  return OK;
}

enum STATUS vector_add_element_at_index(struct vector *vector, void *element,
                                        int index) {
  if (vector == NULL || element == NULL) {
    return NULL_PTR;
  }
  if (index < 0 || index > vector->size) {
    return OUT_OF_BOUNDS;
  }
  if (vector->size == vector->capacity) {
    if (expand_capacity(vector) == ALLOC_ERROR) {
      return ALLOC_ERROR;
    }
  }
  vector->size++;

  memmove((char *)vector->memory + (vector->data_size * (index + 1)),
          (char *)vector->memory + (index * vector->data_size),
          vector->data_size * (vector->size - 1 - index));

  memcpy((char *)vector->memory + (index * vector->data_size), element,
    vector->data_size);

  return OK;
}

enum STATUS vector_push(struct vector *vector, void *element) {
  if (vector == NULL || element == NULL || vector->memory == NULL) {
    return NULL_PTR;
  }
  if (vector->size == vector->capacity) {
    if (expand_capacity(vector) == ALLOC_ERROR) {
      return ALLOC_ERROR;
    }
  }
  memcpy((char *)vector->memory + (vector->size * vector->data_size), element,
         vector->data_size);

  vector->size++;
  return OK;
}

enum STATUS vector_push_front(struct vector *vector, void *element) {
  if (vector == NULL || element == NULL || vector->memory == NULL) {
    return NULL_PTR;
  }
  if (vector->size == vector->capacity) {
    if (expand_capacity(vector) == ALLOC_ERROR) {
      return ALLOC_ERROR;
    }
  }
  vector->size++;

 memmove((char*)vector->memory + (vector->data_size), vector->memory, vector->data_size * (vector->size-1));
 memcpy((char *)vector->memory, element, vector->data_size);
  return OK;
}

enum STATUS vector_get_element_by_index(struct vector *vector, int index,
                                        void **found) {
  if (vector == NULL || found == NULL) {
    return NULL_PTR;
  }
  if (index < 0 || index >= vector->size) {
    return OUT_OF_BOUNDS;
  }
  *found = malloc(vector->data_size);
  if (*found == NULL) {
    return ALLOC_ERROR;
  }
  memcpy(*found, (char *)vector->memory + (index * vector->data_size),
         vector->data_size);
  return OK;
}
enum STATUS vector_shift_left(struct vector *vector) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  if (vector->size <= 1) {
    return OK;
  }

  void *first_element = malloc(vector->data_size);
  if (first_element == NULL) {
    return ALLOC_ERROR;
  }
  memcpy(first_element, vector->memory, vector->data_size);

  memmove(vector->memory, (char*)vector->memory + (vector->data_size), vector->data_size * (vector->size-1));

  memcpy((char *)vector->memory + ((vector->size - 1) * vector->data_size),
         first_element, vector->data_size);

  free(first_element);
  return OK;
}

enum STATUS vector_shift_right(struct vector *vector) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  if (vector->size <= 1) {
    return OK;
  }
  void *last_element = malloc((vector->data_size));
  if (last_element == NULL) {
    return ALLOC_ERROR;
  }
  memcpy(last_element,
         (char *)vector->memory + ((vector->size - 1) * vector->data_size),
         vector->data_size);


  memmove((char*)vector->memory + vector->data_size, vector->memory, vector->data_size * (vector->size - 1));


  memcpy((char *)vector->memory, last_element, vector->data_size);

  free(last_element);

  return OK;
}
enum STATUS vector_free(struct vector *vector) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  free(vector->memory);
  free(vector);

  return OK;
}
