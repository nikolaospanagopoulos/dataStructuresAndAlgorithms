#include "Vector.hpp"
int main() {
  Vector<int> myVec;

  myVec.push(10);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);
  myVec.push(20);

  std::cout << myVec.array[0] << std::endl;
  std::cout << myVec.array[10] << std::endl;

  std::cout << "size: " << myVec.getSize() << std::endl;
  return 0;
}
