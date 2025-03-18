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
    using node = std::conditional_t<
        std::is_const_v<T>, const Node<std::remove_const_t<T>>, Node<T>>;
    using node_pointer   = node*;
    using node_reference = node&;

    node_pointer current_;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = node;
    using difference_type   = std::ptrdiff_t;
    using pointer           = node_pointer;
    using reference         = node_reference;

    explicit LinkedListIterator(node_pointer current) : current_(current) {}

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
    using node = Node<T>;
    using node_pointer = node*;

    node_pointer root_{};

    void copy_from(const LinkedList& other) {
        node_pointer* current = &root_;
        for (node_pointer other_current{other.root_}; other_current;
                other_current = other_current->next, current = &((*current)->next)) {
            *current = new Node{other_current->data};
        }
    }

public:
    using value_type      = T;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator        = LinkedListIterator<T>;
    using const_iterator  = LinkedListIterator<const T>;

    LinkedList() : root_{nullptr} {}

    explicit LinkedList(T data)
        : root_{new node{std::move(data)}}
    {}

    explicit LinkedList(std::initializer_list<T> elements) : root_{nullptr} {
        if (elements.size() == 0) return;

        root_ = new node{*elements.begin()};

        auto current = root_;
        for (auto it = elements.begin() + 1; it != elements.end(); ++it) {
            current->next = new node{*it};
            current = current->next;
        }
    }

    LinkedList(const LinkedList& other) : root_{nullptr} {
       copy_from(other);
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this;

        clear();
        copy_from(other);
        return *this;
    }

    LinkedList(LinkedList&& other) noexcept {
        swap(*this, other);
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        // copy & swap idiom
        LinkedList tmp(std::move(other));
        swap(*this, tmp);
        return *this;
    }

    ~LinkedList() {
        clear();
    }

    void clear() {
        for (node_pointer current{root_}; current;) {
            const auto next = current->next;
            delete current;
            current = next;
        }
        root_ = nullptr;
    }

    friend void swap(LinkedList& l1, LinkedList& l2) noexcept {
        using std::swap;
        swap(l1.root_, l2.root_);
    }

    [[nodiscard]]
    auto begin() { return iterator{root_}; }

    [[nodiscard]]
    auto end() { return iterator{nullptr}; }

    [[nodiscard]]
    auto begin() const { return const_iterator{root_}; }

    [[nodiscard]]
    auto end() const { return const_iterator{nullptr}; }

    [[nodiscard]]
    bool is_empty() const {
        return root_ == nullptr;
    }

    [[nodiscard]]
    bool contains(const_reference value) const {
        return std::find(begin(), end(), value) != end();
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
        for (; current->next; current = current->next);
        return current->data; 
    }

    [[nodiscard]]
    const_reference back() const {
        auto current = root_;
        for (; current->next; current = current->next);
        return current->data;
    }

    void push_front(value_type data) {
        const auto next = root_;
        root_ = new node{std::move(data), next};
    }

    void pop_front() {
        if (!root_) return;

        const auto next = root_->next;
        delete root_;
        root_ = next;
    }

    void push_back(value_type data) {
        const auto new_node = new node{std::move(data)};

        if (!root_) {
            root_ = new_node;
            return;
        } 

        auto current = root_;
        for (; current->next; current = current->next);
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
        for (; current->next; prev = current, current = current->next);
        delete current;
        prev->next = nullptr;
    }

    friend auto operator==(const LinkedList& lhs, const LinkedList& rhs) {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
};

