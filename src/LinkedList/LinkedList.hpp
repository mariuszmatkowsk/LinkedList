#pragma once

#include <algorithm>
#include <utility>

template <typename T>
struct Node {
    T     data{};
    Node* next{};

    explicit Node(T data, Node* next = nullptr)
        : data{std::move(data)}, next{next}
    {}
};

template <typename T>
class LinkedListIterator {
    using NodeType = std::conditional_t<
        std::is_const_v<T>, const Node<std::remove_const_t<T>>, Node<T>>;

    NodeType* current_;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = NodeType;
    using difference_type   = std::ptrdiff_t;
    using pointer           = NodeType*;
    using reference         = NodeType&&;

    explicit LinkedListIterator(NodeType* current) : current_(current) {}

    T& operator*()  const { return  current_->data; };
    T* operator->() const { return &current_->data; };

    LinkedListIterator& operator++() {
        current_ = current_->next;
        return *this;
    }

    friend auto operator==(
            const LinkedListIterator& lhs, const LinkedListIterator& rhs) {
        return lhs.current_ == rhs.current_;
    }

    friend auto operator!=(
            const LinkedListIterator& lhs, const LinkedListIterator& rhs) {
        return lhs.current_ != rhs.current_;
    }
};

template <typename T>
class LinkedList {
    Node<T>* root_{};

public:
    using value_type      = T;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator        = LinkedListIterator<T>;
    using const_iterator  = LinkedListIterator<const T>;

    LinkedList() : root_{nullptr} {}

    explicit LinkedList(T data)
        : root_{new Node{std::move(data)}}
    {}

    explicit LinkedList(std::initializer_list<T> elements) : root_{nullptr} {
        if (elements.size() == 0) return;

        root_ = new Node{*elements.begin()};

        auto current = root_;
        for (auto it = elements.begin() + 1; it != elements.end(); ++it) {
            current->next = new Node{*it};
            current = current->next;
        }
    }

    LinkedList(const LinkedList& other) : root_{nullptr} {
        if (!other.root_) return;

        root_ = new Node{other.root_->data};

        auto current = root_;
        auto other_current = other.root_->next;
        while (other_current) {
            current->next = new Node{other_current->data};
            current = current->next;
            other_current = other_current->next;
        }
    }

    LinkedList& operator=(const LinkedList&)     = default;

    LinkedList(LinkedList&&)            noexcept = default;
    LinkedList& operator=(LinkedList&&) noexcept = default;

    ~LinkedList() {
        auto current = root_;
        while (current) {
            const auto next = current->next;
            delete current;
            current = next;
        }
        root_ = nullptr;
    }

    [[nodiscard]]
    auto begin() { return iterator{root_}; };

    [[nodiscard]]
    auto end() { return iterator{nullptr}; };

    [[nodiscard]]
    auto begin() const { return const_iterator{root_}; };

    [[nodiscard]]
    auto end() const { return const_iterator{nullptr}; };

    [[nodiscard]]
    bool is_empty() const {
        return root_ == nullptr;
    }

    [[nodiscard]]
    reference front() {
        return root_->data;
    }

    [[nodiscard]]
    const_reference front() const {
        return root_->data;
    }

    [[nodiscard]]
    reference back() {
        auto current = root_;
        while (current->next) {
            current = current->next;
        }
        return current->data; 
    }

    [[nodiscard]]
    const_reference back() const {
        auto current = root_;
        while (current->next) {
            current = current->next;
        }
        return current->data;
    }

    void push_front(value_type data) {
        const auto next = root_;
        root_ = new Node{std::move(data), next};
    }

    void pop_front() {
        if (!root_) return;

        const auto next = root_->next;
        delete root_;
        root_ = next;
    }

    void push_back(value_type data) {
        const auto new_node = new Node{std::move(data)};

        if (!root_) {
            root_ = new_node;
            return;
        } 

        auto current = root_;
        while (current->next) {
            current = current->next;
        } 
        current->next = new_node;
    }

    void pop_back() {
        if (!root_) return;

        if (!root_->next) {
            delete root_;
            root_ = nullptr;
            return;
        }

        auto prev = root_;
        auto current = root_->next;
        while (current->next) {
            prev = current;
            current = current->next;
        }
        delete current;
        prev->next = nullptr;
    }

    friend auto operator==(const LinkedList& lhs, const LinkedList& rhs) {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
};

