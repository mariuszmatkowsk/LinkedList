#pragma once

#include <utility>

template <typename T>
struct Node {
    T     data{};
    Node* next{};

    explicit Node(T data, Node* next = nullptr)
        : data{std::move(data)}, next{std::move(next)}
    {}
};

template <typename T>
class LinkedList {
    using ElementType = T;
    using ElementTypeRef = T&;
    using ElementTypeConstRef = const T&;

    Node<T>* root_{};

public:
    LinkedList() : root_{nullptr} {}

    explicit LinkedList(T data)
        : root_{new Node{std::move(data)}}
    {}

    explicit LinkedList(std::initializer_list<T> elements) : root_{nullptr} {
        for (const auto& ele : elements) {
            push_back(ele);
        }
    }

    LinkedList(const LinkedList&) = default;
    LinkedList& operator=(const LinkedList&) = default;

    LinkedList(LinkedList&&) noexcept = default;
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
    bool is_empty() const {
        return root_ == nullptr;
    }

    [[nodiscard]]
    ElementTypeRef front() {
        return root_->data;
    }

    [[nodiscard]]
    ElementTypeConstRef front() const {
        return root_->data;
    }

    [[nodiscard]]
    ElementTypeRef back() {
        auto current = root_;
        while (current->next) {
            current = current->next;
        }
        return current->data; 
    }

    [[nodiscard]]
    ElementTypeConstRef back() const {
        auto current = root_;
        while (current->next) {
            current = current->next;
        }
        return current->data;
    }

    void push_front(T data) {
        const auto next = root_;
        root_ = new Node{std::move(data), next};
    }

    void pop_front() {
        if (!root_) return;

        const auto next = root_->next;
        delete root_;
        root_ = next;
    }

    void push_back(T data) {
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

    friend bool operator==(const LinkedList& lhs, const LinkedList& rhs) {
        auto left = lhs.root_;
        auto right = rhs.root_;

        while (left && right) {
            if (left->data != right->data) {
                return false;
            }

            left = left->next;
            right = right->next;
        }
        return left == right;
    }
};

