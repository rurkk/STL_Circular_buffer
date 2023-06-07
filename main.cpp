#include <memory>
#include <iostream>
//#include "CCirtucalBuffer.h"
#include "CCirtucalBufferExt.h"


int main() {
  CCirtucalBufferExt<int> b(5);
  std::cout << '\n';
  for (int j = 1; j < 6; j++) {
    b.insert(b.end(), j);

    std::cout << '\n';
    for (int i = 0; i < b.size(); i++) {
      std::cout << b[i] << ' ';
    }
    std::cout << '\n';
  }
  b.insert(b.end(), 6);

  std::cout << '\n';
  for (int i = 0; i < b.size(); i++) {
    std::cout << b[i] << ' ';
  }
  std::cout << '\n';
  b.insert(b.end(), 7);
//  for (int i = 0; i < 5; i++) {
//    std::cout << b.data_[i] << '-';
//  }
  std::cout << '\n';
  for (int i = 0; i < b.size(); i++) {
    std::cout << b[i] << ' ';
  }
  std::cout << '\n';




  b.insert(b.begin(), 4, 8);
//  for (int i = 0; i < 5; i++) {
//    std::cout << b.data_[i] << '-';
//  }
  std::cout << '\n';
  for (int i = 0; i < b.size(); i++) {
    std::cout << b[i] << ' ';
  }
  std::cout << '\n';

  b.insert(b.begin(), {9, 10, 11});
//  for (int i = 0; i < 5; i++) {
//    std::cout << b.data_[i] << '-';
//  }
  std::cout << '\n';
  for (int i = 0; i < b.size(); i++) {
    std::cout << b[i] << ' ';
  }
  std::cout << '\n';

  b.erase(b.begin() + 2, b.begin() + 4);
//  for (int i = 0; i < 5; i++) {
//    std::cout << b.data_[i] << '-';
//  }
  std::cout << '\n';
  for (int & i : b) {
    std::cout << i << ' ';
  }
  std::cout << '\n';



  return 0;
}
