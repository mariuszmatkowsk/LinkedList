#include "LinkedList/LinkedList.hpp"

#include <print>

class Foo {
    unsigned* count_{};

public:
    explicit Foo(unsigned* count) : count_{count} {}

    ~Foo() {
        if (count_) {
            std::println("foo destruction");
            if (count_) (*count_)++;
        }
    }

    Foo(const Foo&) = delete;
    Foo& operator=(const Foo&) = delete;

    Foo(Foo&& other) noexcept : count_{other.count_} {
        other.count_ = nullptr;
    }
};

int main() {
    unsigned count{0};
    Foo foo{&count};
    {
        LinkedList<Foo> ll(std::move(foo));
        std::println("Linked list destoryed...");
    }

    std::println("Count = {}", count);

    return 0;
}
