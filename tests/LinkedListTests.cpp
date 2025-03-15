#include "LinkedList/LinkedList.hpp"

#include <gtest/gtest.h>

#include <utility>

TEST(LinkedList, canCreateEmptyLinkedList) {
    LinkedList<int> ll{};

    EXPECT_TRUE(ll.is_empty());
}

TEST(LinkedList, canCreateLinkedListWithInitElement) {
    LinkedList<int> ll{5};

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.front(), 5);
}

TEST(LinkedList, shouldDeduceType) {
    LinkedList ll{7};

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.front(), 7);
}

TEST(LinkedList, memoryShouldBeFreeOnce) {
    class Foo {
        unsigned* destruction_count_;

    public:
        explicit Foo(unsigned* destruction_count)
            : destruction_count_{destruction_count}
        {}

        Foo(const Foo&)            = delete;
        Foo& operator=(const Foo&) = delete;

        Foo(Foo&& other) noexcept
            : destruction_count_{
                std::exchange(other.destruction_count_, nullptr)}
        {}

        Foo& operator=(Foo&&) noexcept = delete;

        ~Foo() {
            if (destruction_count_)
                ++(*destruction_count_);
        } 
    };

    unsigned destruction_count{};
    {
        LinkedList<Foo> ll(Foo{&destruction_count});
    }

    EXPECT_EQ(destruction_count, 1);
}

TEST(LinkedList, front) {
    LinkedList<int> ll{6};

    const LinkedList<int> lll{7};

    EXPECT_FALSE(ll.is_empty());

    EXPECT_EQ(ll.front(), 6);
    EXPECT_EQ(lll.front(), 7);
}

TEST(LinkedList, canModifyElementByFront) {
    LinkedList<int> ll{66};

    ll.front() = 77;

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.front(), 77);
}

TEST(LinkedList, back) {
    LinkedList<int> ll{6};

    const LinkedList<int> lll{7};

    EXPECT_FALSE(ll.is_empty());

    EXPECT_EQ(ll.back(), 6);
    EXPECT_EQ(lll.back(), 7);
}

TEST(LinkedList, canModifyElementByBack) {
    LinkedList<int> ll{66};

    ll.back() = 77;

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.back(), 77);
}

TEST(LinkedList, pop_front) {
    LinkedList<int> ll{77};

    ll.pop_front();

    EXPECT_TRUE(ll.is_empty());
}

TEST(LinkedList, pop_back) {
    LinkedList<int> ll{77};

    ll.pop_back();

    EXPECT_TRUE(ll.is_empty());
}

TEST(LinkedList, pushFront) {
    LinkedList<int> ll{};

    ll.push_front(5);
    ll.push_front(6);
    ll.push_front(9);

    EXPECT_FALSE(ll.is_empty());

    // LIFO order
    EXPECT_EQ(ll.front(), 9);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 6);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 5);
}

TEST(LinkedList, pushBack) {
    LinkedList<int> ll{};

    ll.push_back(2);
    ll.push_back(3);
    ll.push_back(4);

    // FIFO order
    EXPECT_EQ(ll.front(), 2);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 3);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 4);
}

TEST(LinkedList, initializeWithManyElements) {
    LinkedList ll{3, 45, 33, 77};

    EXPECT_FALSE(ll.is_empty());

    EXPECT_EQ(ll.front(), 3);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 45);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 33);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 77);
}

TEST(LinkedList, canCompareTwoLinkedLists_Equal) {
    LinkedList<int> ll{2, 3};
    LinkedList<int> lll{2, 3};

    EXPECT_EQ(ll, lll);

    LinkedList<int> l1{};
    LinkedList<int> l2{};

    EXPECT_EQ(l1, l2);
}

TEST(LinkedList, canCompareTwoLinkedLists_NotEqual) {
    LinkedList<int> l{};
    LinkedList<int> ll{2, 3, 4};
    LinkedList<int> lll{2, 3};
    LinkedList<int> llll{2, 4, 3};
 
    EXPECT_NE(l, ll);
    EXPECT_NE(ll, lll);
    EXPECT_NE(ll, llll);
}

TEST(LinkedList, canUseWithRangeBasedLoop) {
    LinkedList ll{2, 3, 4};

    std::array<int, 3> expected{2, 3, 4};

    for (size_t i{}; const auto& ll_element : ll) {
        EXPECT_EQ(ll_element, expected[i++]);
    }
}

TEST(LinkedList, canModifyElementBytRangeBasedLoop) {
    LinkedList ll{1, 2, 3};

    for (auto& elem : ll) {
        elem = 4;
    }

    EXPECT_EQ(ll.front(), 4);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 4);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 4);
}

