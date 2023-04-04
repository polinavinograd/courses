#include <iostream>
#include <string>

bool hello(std::string user) {
    std::cout << "STATIC Hello, " << user <<  ", nice to meet you" << std::endl;
    return std::cout.good();
}