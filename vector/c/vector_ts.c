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

enum STATUS vector_shift_left_ts(struct vector *vector) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_wrlock(&vector->lock);
  if (vector->size <= 1) {
    pthread_rwlock_unlock(&vector->lock);
    return OK;
  }

  void *first_element = malloc(vector->data_size);
  if (first_element == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return ALLOC_ERROR;
  }
  memcpy(first_element, vector->memory, vector->data_size);

  for (size_t i = 0; i < vector->size - 1; i++) {
    memmove((char *)vector->memory + ((i)*vector->data_size),
            (char *)vector->memory + ((i + 1) * vector->data_size),
            vector->data_size);
  }
  memcpy((char *)vector->memory + ((vector->size - 1) * vector->data_size),
         first_element, vector->data_size);

  free(first_element);
  pthread_rwlock_unlock(&vector->lock);
  return OK;
}

enum STATUS vector_shift_right_ts(struct vector *vector) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_wrlock(&vector->lock);
  if (vector->size <= 1) {
    pthread_rwlock_unlock(&vector->lock);
    return OK;
  }
  void *last_element = malloc((vector->data_size));
  if (last_element == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return ALLOC_ERROR;
  }
  memcpy(last_element,
         (char *)vector->memory + ((vector->size - 1) * vector->data_size),
         vector->data_size);

  for (size_t i = vector->size - 1; i > 0; i--) {
    memmove((char *)vector->memory + (i * vector->data_size),
            (char *)vector->memory + ((i - 1) * vector->data_size),
            vector->data_size);
  }
  memcpy((char *)vector->memory, last_element, vector->data_size);

  free(last_element);

  pthread_rwlock_unlock(&vector->lock);
  return OK;
}
enum STATUS vector_delete_ts(struct vector *vector, int index,
                          void (*delete_func)(void *to_delete)) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_wrlock(&vector->lock);
  if (index < 0 || index >= vector->size) {
  pthread_rwlock_unlock(&vector->lock);
    return OUT_OF_BOUNDS;
  }
  if (delete_func != NULL) {
    delete_func((char *)vector->memory + (index * vector->data_size));
  }

  for (size_t i = index; i < vector->size - 1; i++) {
    memmove((char *)vector->memory + (i * vector->data_size),
            (char *)vector->memory + ((i + 1) * vector->data_size),
            vector->data_size);
  }
  vector->size--;

  pthread_rwlock_unlock(&vector->lock);
  return OK;
}
enum STATUS vector_free_ts(struct vector *vector) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_wrlock(&vector->lock);
  free(vector->memory);
  pthread_rwlock_unlock(&vector->lock);
  pthread_rwlock_destroy(&vector->lock);
  free(vector);

  return OK;
}
enum STATUS vector_push_ts(struct vector *vector, void *element) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_wrlock(&vector->lock);
  if (element == NULL || vector->memory == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return NULL_PTR;
  }
  if (vector->size == vector->capacity) {
    if (expand_capacity(vector) == ALLOC_ERROR) {
      pthread_rwlock_unlock(&vector->lock);
      return ALLOC_ERROR;
    }
  }
  memcpy((char *)vector->memory + (vector->size * vector->data_size), element,
         vector->data_size);

  vector->size++;
  pthread_rwlock_unlock(&vector->lock);
  return OK;
}
enum STATUS vector_push_front_ts(struct vector *vector, void *element) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_wrlock(&vector->lock);
  if (element == NULL || vector->memory == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return NULL_PTR;
  }
  if (vector->size == vector->capacity) {
    if (expand_capacity(vector) == ALLOC_ERROR) {
      pthread_rwlock_unlock(&vector->lock);
      return ALLOC_ERROR;
    }
  }
  vector->size++;
  for (size_t i = vector->size - 1; i > 0; i--) {
    memmove((char *)vector->memory + (i * vector->data_size),
            (char *)vector->memory + ((i - 1) * vector->data_size),
            vector->data_size);
  }
  memcpy((char *)vector->memory, element, vector->data_size);
  pthread_rwlock_unlock(&vector->lock);
  return OK;
}
enum STATUS vector_add_element_at_index_ts(struct vector *vector, void *element,
                                           int index) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_wrlock(&vector->lock);
  if (element == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return NULL_PTR;
  }
  if (index < 0 || index > vector->size) {
    pthread_rwlock_unlock(&vector->lock);
    return OUT_OF_BOUNDS;
  }
  if (vector->size == vector->capacity) {
    if (expand_capacity(vector) == ALLOC_ERROR) {
      pthread_rwlock_unlock(&vector->lock);
      return ALLOC_ERROR;
    }
  }
  vector->size++;
  for (size_t i = vector->size - 1; i > index; i--) {
    memmove((char *)vector->memory + (i * vector->data_size),
            (char *)vector->memory + ((i - 1) * vector->data_size),
            vector->data_size);
  }
  memcpy((char *)vector->memory + (index * vector->data_size), element,
         vector->data_size);

  pthread_rwlock_unlock(&vector->lock);
  return OK;
}
enum STATUS vector_find_ts(struct vector *vector, void *element_to_find,
                           int *index_found) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_rdlock(&vector->lock);
  if (element_to_find == NULL || index_found == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return NULL_PTR;
  }
  if (vector->find_func == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return MISSING_ARG;
  }
  for (size_t i = 0; i < vector->size; i++) {
    if (vector->find_func((char *)vector->memory + (i * vector->data_size),
                          element_to_find)) {
      *index_found = i;
      pthread_rwlock_unlock(&vector->lock);
      return OK;
    }
  }
  *index_found = -1;
  pthread_rwlock_unlock(&vector->lock);
  return OK;
}
enum STATUS vector_get_element_by_index_ts(struct vector *vector, int index,
                                           void **found) {
  if (vector == NULL) {
    return NULL_PTR;
  }
  pthread_rwlock_rdlock(&vector->lock);
  if (found == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return NULL_PTR;
  }
  if (index < 0 || index >= vector->size) {
    pthread_rwlock_unlock(&vector->lock);
    return OUT_OF_BOUNDS;
  }
  *found = malloc(vector->data_size);
  if (*found == NULL) {
    pthread_rwlock_unlock(&vector->lock);
    return ALLOC_ERROR;
  }

  memcpy(*found, (char *)vector->memory + (index * vector->data_size),
         vector->data_size);

  pthread_rwlock_unlock(&vector->lock);
  return OK;
}
