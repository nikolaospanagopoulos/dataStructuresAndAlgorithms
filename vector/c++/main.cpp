#include "Vector.hpp"
int main() {
  Vector<int> myVec;

  myVec.push(10);
  myVec.push(20);
  myVec.pushFront(549);
  myVec.insertAt(4, 45);

  std::cout << "vector size: " << myVec.getSize() << std::endl;

  for (size_t i = 0; i < myVec.getSize(); i++) {
    if (myVec.at(i).has_value()) {
      std::cout << myVec.at(i).value() << std::endl;
    }
  }

  return 0;
}
