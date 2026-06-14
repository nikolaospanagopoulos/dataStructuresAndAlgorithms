#include <iostream>
#include <optional>
#include <utility>

template <typename T> class Vector {
private:
  size_t size;
  T *array;
  size_t capacity;

public:
  Vector() : size(0), capacity(10), array(nullptr) {
    array = new T[capacity];
    std::cout << "vector initializing with capacity " << capacity << "\n";
  }
  size_t getSize() const { return size; }
  std::optional<T> at(int index) const {
    if (index >= 0 && index < size) {
      return array[index];
    }
    return {};
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
  size_t pushFront(T element) {
    if (size == capacity) {
      expandCapacity();
    }
    size++;
    for (size_t i = size - 1; i > 0; i--) {
      std::swap(array[i - 1], array[i]);
    }
    array[0] = element;

    return size;
  }

  size_t push(T element) {
    if (size == capacity) {
      expandCapacity();
    }
    size++;
    array[size - 1] = element;

    return size;
  }
  ~Vector() { delete[] array; }
  bool insertAt(int index, T element) {
    if (index > size || index < 0) {
      return false;
    }
    if (size == capacity) {
      expandCapacity();
    }
    size++;
    for (size_t i = size - 1; i > index; i--) {
      std::swap(array[i - 1], array[i]);
    }

    array[index] = element;
    return true;
  }
};
