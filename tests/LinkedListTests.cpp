#include "LinkedList/LinkedList.hpp"

#include <gtest/gtest.h>

#include <utility>
#include <format>

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
    LinkedList<int> l1{6};

    const LinkedList<int> l2{7};

    EXPECT_FALSE(l1.is_empty());

    EXPECT_EQ(l1.front(), 6);
    EXPECT_EQ(l2.front(), 7);
}

TEST(LinkedList, canModifyElementByFront) {
    LinkedList<int> ll{66};

    ll.front() = 77;

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.front(), 77);
}

TEST(LinkedList, back) {
    LinkedList<int> l1{6};

    const LinkedList<int> l2{7};

    EXPECT_FALSE(l1.is_empty());

    EXPECT_EQ(l1.back(), 6);
    EXPECT_EQ(l2.back(), 7);
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
    LinkedList<int> l1{2, 3};
    LinkedList<int> l2{2, 3};

    EXPECT_EQ(l1, l2);

    l1.clear();
    l2.clear();

    EXPECT_EQ(l1, l2);
}

TEST(LinkedList, canCompareTwoLinkedLists_NotEqual) {
    LinkedList<int> l1{};
    LinkedList<int> l2{2, 3, 4};
    LinkedList<int> l3{2, 3};
    LinkedList<int> l4{2, 4, 3};
 
    EXPECT_NE(l1, l2);
    EXPECT_NE(l2, l3);
    EXPECT_NE(l2, l4);
}

TEST(LinkedList, begin) {
    LinkedList ll{2};

    EXPECT_EQ(*ll.begin(), 2);
}

TEST(LinkedList, emptyListBeginEqualEnd) {
    LinkedList<int> ll{};

    EXPECT_EQ(ll.begin(), ll.end());
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

TEST(LinkedList, canInitializeLinkedListFromAnother) {
    LinkedList<int> l1{1, 2, 3};
    LinkedList<int> l2 = l1;

    EXPECT_EQ(l1.front(), 1);
    l1.pop_front();
    EXPECT_EQ(l1.front(), 2);
    l1.pop_front();
    EXPECT_EQ(l1.front(), 3);

    EXPECT_EQ(l2.front(), 1);
    l2.pop_front();
    EXPECT_EQ(l2.front(), 2);
    l2.pop_front();
    EXPECT_EQ(l2.front(), 3);
}

TEST(LinkedList, copyAssignemtnOperatorDoNothingWhenSelfAssignment) {
    LinkedList ll{1, 2};

    ll = ll;

    EXPECT_EQ(ll.front(), 1);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 2);
}

TEST(LinkedList, copyAssignmentOperator) {
    LinkedList l1{1, 2};
    LinkedList l2{3, 4};

    l2 = l1;

    EXPECT_EQ(l1.front(), 1);
    l1.pop_front();
    EXPECT_EQ(l1.front(), 2);

    EXPECT_EQ(l2.front(), 1);
    l2.pop_front();
    EXPECT_EQ(l2.front(), 2);
}

TEST(LinkedList, swap) {
    LinkedList l1{1, 2};
    LinkedList l2{3, 4};

    swap(l1, l2);

    EXPECT_EQ(l1.front(), 3);
    l1.pop_front();
    EXPECT_EQ(l1.front(), 4);

    EXPECT_EQ(l2.front(), 1);
    l2.pop_front();
    EXPECT_EQ(l2.front(), 2);
}

TEST(LinkedList, moveCopyConstructor) {
    LinkedList l1{1, 2};
    LinkedList l2(std::move(l1));

    ASSERT_TRUE(l1.is_empty());

    EXPECT_EQ(l2.front(), 1);
    l2.pop_front();
    EXPECT_EQ(l2.front(), 2);
}

TEST(LinkedList, moveAssignmentOperator) {
    LinkedList l1{1, 2};
    LinkedList l2{3, 4};

    l1 = std::move(l2);

    ASSERT_TRUE(l2.is_empty());

    EXPECT_EQ(l1.front(), 3);
    l1.pop_front();
    EXPECT_EQ(l1.front(), 4);
}

TEST(LinkedList, contains_EmptyList) {
    LinkedList<int> ll{};

    EXPECT_FALSE(ll.contains(9));
}

TEST(LinkedList, contains) {
    LinkedList ll{1, 3, 5};

    EXPECT_TRUE(ll.contains(1));
    EXPECT_TRUE(ll.contains(3));
    EXPECT_TRUE(ll.contains(5));
    EXPECT_FALSE(ll.contains(9));
}

TEST(LinkedList, shouldWorkWithStdFormat) {
    LinkedList ll{1};

    std::string expected{"[1 -> NULL]"};
    EXPECT_EQ(std::format("{}", ll), expected);

    ll.clear();

    expected = "[NULL]";
    EXPECT_EQ(std::format("{}", ll), expected);

    ll.push_back(3);
    ll.push_back(7);
    ll.push_back(8);

    expected = "[3 -> 7 -> 8 -> NULL]";
    EXPECT_EQ(std::format("{}", ll), expected);
}

TEST(LinkedList, defaultConstructorShouldBeConstexpr) {
    constexpr LinkedList<int> ll{};
}

TEST(LinkedList, emptyInitializerListShouldBeConstexpr) {
    constexpr LinkedList<int> l{};
}

TEST(LinkedList, is_empty_constexpr) {
    constexpr LinkedList<int> ll;

    static_assert(ll.is_empty() == true, "Must be constexpr");
}

TEST(LinkedList, beginAndEndShouldBeConstexpr) {
    constexpr LinkedList<int> ll;

    static_assert(ll.begin() == ll.end(),
        "Constexpr begin and end should be equals for empty list");
}

TEST(LinkedList, containsShouldBeConstexpr) {
    constexpr LinkedList<int> ll;

    static_assert(ll.contains(3) == false, "Should return false");
}

