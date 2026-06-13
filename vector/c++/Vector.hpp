#include <iostream>
#include <utility>

template <typename T> class Vector {
public:
  size_t size;
  T *array;
  size_t capacity;

  Vector() : size(0), capacity(10), array(nullptr) {
    array = new T[capacity];
    std::cout << "vector initializing with capacity " << capacity << "\n";
  }
  
  void expandCapacity() {
    T *newArray = new T[capacity * 2];
    for (size_t i = 0; i < size; i++) {
      newArray[i] = array[i];
    }
    std::cout << "expanding capacity\n";
    std::swap(array, newArray);

    delete[] newArray;
  }

  int push(T element) {
    if (size == capacity) {
      expandCapacity();
    }
    size++;
    array[size - 1] = element;

    return size;
  }
  ~Vector() { delete[] array; }
};
