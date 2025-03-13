#include "LinkedList/LinkedList.hpp"

#include <gtest/gtest.h>

TEST(LinkedList, canCreateEmptyLinkedList) {
    LinkedList<int> ll{};
}
