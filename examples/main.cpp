#include "LinkedList/LinkedList.hpp"

#include <print>
#include <iostream>

int main() {
    LinkedList ll{3, 4, 7, 9};

    std::println("{}", ll);

    std::cout << ll << std::endl;

    return 0;
}
