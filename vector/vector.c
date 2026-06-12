#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vector {
  size_t size;
  void *memory;
  size_t data_size;
  size_t capacity;
  bool (*find_func)(void *to_compare1, void *to_compare2);
};

enum STATUS { OK, ALLOC_ERROR, NULL_PTR, OUT_OF_BOUNDS, MISSING_ARG };

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

  void *vector_heap = malloc(sizeof(struct vector));
  if (vector_heap == NULL) {
    return ALLOC_ERROR;
  }
  *vector = vector_heap;
  (*vector)->capacity = capacity;
  (*vector)->data_size = data_size;
  (*vector)->find_func = NULL;

  void *vector_memory = malloc(capacity * data_size);
  if (vector_memory == NULL) {
    free(*vector);
    return ALLOC_ERROR;
  }

  (*vector)->memory = vector_memory;
  return OK;
}

static enum STATUS expand_capacity(struct vector *vector) {
  void *expand =
      realloc(vector->memory, (vector->capacity * 2) * vector->data_size);
  if (expand == NULL) {
    return ALLOC_ERROR;
  }
  printf("expanding....\n");
  vector->memory = expand;

  vector->capacity *= 2;
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
  for (size_t i = vector->size - 1; i > 0; i--) {
    memcpy((char *)vector->memory + (i * vector->data_size),
           (char *)vector->memory + ((i - 1) * vector->data_size),
           vector->data_size);
  }
  memcpy((char *)vector->memory, element, vector->data_size);
  return OK;
}
enum STATUS vector_get_element_by_index(struct vector *vector, size_t index,
                                        void **found) {
  if (vector == NULL || found == NULL) {
    return NULL_PTR;
  }
  if (index < 0 || index >= vector->size) {
    return OUT_OF_BOUNDS;
  }
  void *mem_for_found = malloc(vector->data_size);
  if (mem_for_found == NULL) {
    return ALLOC_ERROR;
  }
  *found = mem_for_found;

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

  for (size_t i = 0; i < vector->size - 1; i++) {
    memcpy((char *)vector->memory + ((i)*vector->data_size),
           (char *)vector->memory + ((i + 1) * vector->data_size),
           vector->data_size);
  }
  memcpy((char *)vector->memory + ((vector->size - 1) * vector->data_size),
         first_element, vector->data_size);

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

  for (size_t i = vector->size - 1; i > 0; i--) {
    memcpy((char *)vector->memory + (i * vector->data_size),
           (char *)vector->memory + ((i - 1) * vector->data_size),
           vector->data_size);
  }
  memcpy((char *)vector->memory, last_element, vector->data_size);

  free(last_element);

  return OK;
}

bool compare(void *num1, void *num2) {
  int first = *(int *)num1;
  int second = *(int *)num2;

  return first == second;
}

int main() {

  struct vector *vec;

  vector_init(&vec, sizeof(int), 10);
  vec->find_func = compare;

  int num = 10;

  int num2 = 60;
  int num3 = 23;
  int num4 = 12;
  int num5 = 90;
  vector_push(vec, &num);
  vector_push(vec, &num2);
  vector_push(vec, &num3);
  vector_push(vec, &num4);

  vector_shift_left(vec);
  vector_shift_left(vec);

  void *found = NULL;
  for (size_t i = 0; i < vec->size; i++) {
    vector_get_element_by_index(vec, i, &found);
    printf("number: %d\n", *(int *)found);
    if (found) {
      free(found);
    }
  }

  int to_find = 122;
  int found_index = -1;

  vector_find(vec, &to_find, &found_index);

  printf("element: %d was found at index: %d\n", to_find, found_index);

  return 0;
}
