#include "hello.hpp"
#include <iostream>

bool hello(std::string user) {
  std::cout << "SHARED Hello," << user << std::endl;
  return std::cout.good();
}