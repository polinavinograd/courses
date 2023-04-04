#include "src/hello.hpp"
#include <cstdlib>
#include <string>

int main(int argc, char* argv[]) {
    std::string user = std::getenv("USER");
    bool is_good = hello(user);
    return is_good? EXIT_SUCCESS: EXIT_FAILURE;
}