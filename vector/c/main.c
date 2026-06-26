#include "vector.h"

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
  vector_push_front(vec, &num);
  vector_push_front(vec, &num2);
  vector_push_front(vec, &num3);
  vector_push_front(vec, &num4);

  vector_add_element_at_index(vec, &num5, 1);

  vector_delete(vec, 0,NULL);
  vector_delete(vec, 1,NULL);




  void *found = NULL;

  for (size_t i = 0; i < vec->size; i++) {
    vector_get_element_by_index(vec, i, &found);
    printf("number: %d\n", *(int *)found);
	free(found);
  }

  struct person{
	  char*name;
	  int age;
  };
  struct person *p = malloc(sizeof(struct person));
  p->age=15;
  p->name = "nikos";



  struct vector *struct_vec;
  vector_init(&struct_vec, sizeof(struct person), 10);

  vector_push(struct_vec, p);

  void*foundPerson=NULL;
  for(size_t i=0;i<struct_vec->size;i++){
	  vector_get_element_by_index(struct_vec, i, &foundPerson);
	  printf("name: %s, age: %d \n",((struct person*)foundPerson)->name,((struct person*)foundPerson)->age);
	  free(foundPerson);
  }


  // int to_find = 122;
  // int found_index = -1;

  // vector_find(vec, &to_find, &found_index);

  // printf("element: %d was found at index: %d\n", to_find, found_index);
  free(p);

  vector_free(vec);
  vector_free(struct_vec);

  return 0;
}
