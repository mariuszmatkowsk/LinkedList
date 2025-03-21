#include "LinkedList/LinkedList.hpp"

#include <gtest/gtest.h>

#include <utility>
#include <format>
#include <string>

TEST(LinkedList, canCreateEmptyLinkedList) {
    LinkedList<int> ll{};

    EXPECT_TRUE(ll.is_empty());
    EXPECT_EQ(ll.size(), 0);
}

TEST(LinkedList, canCreateLinkedListWithInitElement) {
    LinkedList<int> ll{5};

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.size(), 1);
    EXPECT_EQ(*(ll.front()), 5);
}

TEST(LinkedList, shouldDeduceType) {
    LinkedList ll{7};

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.size(), 1);
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
    EXPECT_EQ(l1.size(), 1);

    EXPECT_TRUE(l1.front().has_value());
    EXPECT_EQ(l1.front(), 6);

    EXPECT_TRUE(l2.front().has_value());
    EXPECT_EQ(l2.front().value(), 7);
}

TEST(LinkedList, frontOnEmptyList) {
    LinkedList<int> ll{};

    EXPECT_EQ(ll.size(), 0);
    EXPECT_FALSE(ll.front().has_value());
    EXPECT_EQ(ll.front(), std::unexpected(LinkedListError::EmptyList));
}

TEST(LinkedList, canModifyElementByFront) {
    LinkedList<int> ll{66};

    ll.front().value().get() = 77;

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.size(), 1);
    EXPECT_EQ(ll.front(), 77);
}

TEST(LinkedList, back) {
    LinkedList<int> l1{6};

    const LinkedList<int> l2{7};

    EXPECT_FALSE(l1.is_empty());
    EXPECT_EQ(l1.size(), 1);

    EXPECT_TRUE(l1.back().has_value());
    EXPECT_EQ(l1.back(), 6);
    EXPECT_TRUE(l2.back().has_value());
    EXPECT_EQ(l2.back(), 7);
}

TEST(LinkdeList, backOnEmptyList) {
    LinkedList<int> ll{};

    EXPECT_TRUE(ll.is_empty());
    EXPECT_EQ(ll.size(), 0);
    EXPECT_FALSE(ll.back().has_value());
    EXPECT_EQ(ll.back(), std::unexpected(LinkedListError::EmptyList));
}

TEST(LinkedList, canModifyElementByBack) {
    LinkedList<int> ll{66};

    ll.back().value().get() = 77;

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.size(), 1);
    EXPECT_EQ(ll.back(), 77);
}

TEST(LinkedList, pop_front) {
    LinkedList<int> ll{77};

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.size(), 1);

    ll.pop_front();

    EXPECT_TRUE(ll.is_empty());
    EXPECT_EQ(ll.size(), 0);
}

TEST(LinkedList, pop_back) {
    LinkedList<int> ll{77};

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.size(), 1);

    ll.pop_back();

    EXPECT_TRUE(ll.is_empty());
    EXPECT_EQ(ll.size(), 0);
}

TEST(LinkedList, pushFront) {
    LinkedList<int> ll{};

    ll.push_front(5);
    ll.push_front(6);
    ll.push_front(9);

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.size(), 3);

    // LIFO order
    EXPECT_EQ(ll.front(), 9);
    ll.pop_front();
    EXPECT_EQ(ll.size(), 2);
    EXPECT_EQ(ll.front(), 6);
    ll.pop_front();
    EXPECT_EQ(ll.size(), 1);
    EXPECT_EQ(ll.front(), 5);
}

TEST(LinkedList, pushBack) {
    LinkedList<int> ll{};

    ll.push_back(2);
    ll.push_back(3);
    ll.push_back(4);

    EXPECT_EQ(ll.size(), 3);

    // FIFO order
    EXPECT_EQ(ll.front(), 2);
    ll.pop_front();
    EXPECT_EQ(ll.size(), 2);
    EXPECT_EQ(ll.front(), 3);
    ll.pop_front();
    EXPECT_EQ(ll.size(), 1);
    EXPECT_EQ(ll.front(), 4);
}

TEST(LinkedList, initializeWithManyElements) {
    LinkedList ll{3, 45, 33, 77};

    EXPECT_FALSE(ll.is_empty());
    EXPECT_EQ(ll.size(), 4);

    EXPECT_EQ(ll.front(), 3);
    ll.pop_front();
    EXPECT_EQ(ll.size(), 3);
    EXPECT_EQ(ll.front(), 45);
    ll.pop_front();
    EXPECT_EQ(ll.size(), 2);
    EXPECT_EQ(ll.front(), 33);
    ll.pop_front();
    EXPECT_EQ(ll.size(), 1);
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

    EXPECT_EQ(l1.size(), 3);
    EXPECT_EQ(l2.size(), 3);

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

    EXPECT_EQ(ll.size(), 2);
    EXPECT_EQ(ll.front(), 1);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 2);
}

TEST(LinkedList, copyAssignmentOperator) {
    LinkedList l1{1, 2};
    LinkedList l2{3, 4};

    l2 = l1;

    EXPECT_EQ(l1.size(), 2);
    EXPECT_EQ(l2.size(), 2);

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
    ASSERT_EQ(l1.size(), 0);

    EXPECT_EQ(l2.size(), 2);
    EXPECT_EQ(l2.front(), 1);
    l2.pop_front();
    EXPECT_EQ(l2.front(), 2);
}

TEST(LinkedList, moveAssignmentOperator) {
    LinkedList l1{1, 2};
    LinkedList l2{3, 4};

    l1 = std::move(l2);

    ASSERT_TRUE(l2.is_empty());
    ASSERT_EQ(l2.size(), 0);

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

TEST(LinkedList, shouldWorkWithOutputStream) {
    LinkedList ll{1, 3};

    std::stringstream ss;
    ss << ll;

    std::string expected{"[1 -> 3 -> NULL]"};
    EXPECT_EQ(ss.str(), expected);

    ll.clear();
    ss.str("");
    ss.clear();

    ss << ll;

    expected = "[NULL]";
    EXPECT_EQ(ss.str(), expected);
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

TEST(LinkedList, size_constexpr) {
    constexpr LinkedList<int> ll{};

    static_assert(ll.size() == 0, "Empty list size 0");
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

TEST(LinkedList, canBeUseWithForEachToModifyContent) {
    LinkedList ll{1, 2, 3};

    std::for_each(std::begin(ll), std::end(ll), [](auto& e) {
        e = e * 2;
    });

    EXPECT_TRUE(ll.front().has_value());
    EXPECT_EQ(ll.front().value(), 2);
    ll.pop_front();
    EXPECT_TRUE(ll.front().has_value());
    EXPECT_EQ(ll.front().value(), 4);
    ll.pop_front();
    EXPECT_TRUE(ll.front().has_value());
    EXPECT_EQ(ll.front().value(), 6);
    ll.pop_front();
    EXPECT_FALSE(ll.front().has_value());
    EXPECT_EQ(ll.front(), std::unexpected(LinkedListError::EmptyList));
}

TEST(LinkdeList, removeDoNothingWhenListEmpty) {
    LinkedList<double> ll{};

    ll.remove(3);

    EXPECT_TRUE(ll.is_empty());
    EXPECT_EQ(ll.size(), 0);
}

TEST(LinkedList, canRemoveFirstElement) {
    LinkedList ll{1, 3};

    ll.remove(1);

    EXPECT_EQ(ll.size(), 1);
    EXPECT_EQ(ll.front(), 3);
}

TEST(LinkedList, canRemoveMiddleElement) {
    LinkedList ll{1, 2, 3};

    ll.remove(2);

    EXPECT_EQ(ll.size(), 2);
    EXPECT_EQ(ll.front(), 1);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 3);
}

TEST(LinkedList, canRemoveLastElement) {
    LinkedList ll{1, 2, 3};

    ll.remove(3);

    EXPECT_EQ(ll.size(), 2);
    EXPECT_EQ(ll.front(), 1);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 2);
}

TEST(LinkdeList, removeRemovesOnlyFirstOccurrence) {
    LinkedList ll{1, 2, 2, 3};

    ll.remove(2);

    EXPECT_EQ(ll.size(), 3);
    EXPECT_EQ(ll.front(), 1);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 2);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 3);
}

TEST(LinkedList, removeAllDoNothingWhenListEmpty) {
    LinkedList<std::string> ll{};

    using namespace std::string_literals;
    ll.remove_all("Foo"s);

    EXPECT_TRUE(ll.is_empty());
    EXPECT_EQ(ll.size(), 0);
}

TEST(LinkdeList, removeAll) {
    LinkedList<int> ll{1, 3, 1, 4, 1};

    ll.remove_all(1);

    EXPECT_EQ(ll.size(), 2);
    EXPECT_EQ(ll.front(), 3);
    ll.pop_front();
    EXPECT_EQ(ll.front(), 4);
}

TEST(LinkedList, removeAllWhenListHasSameElements) {
    LinkedList<int> ll{1, 1, 1};

    ll.remove_all(1);

    EXPECT_TRUE(ll.is_empty());
    EXPECT_EQ(ll.size(), 0);
}

