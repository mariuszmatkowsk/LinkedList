#pragma once

#include <memory>

template <typename T>
struct Node {
    T     data{};
    Node* next{};

    Node(T data, Node* next) : data{std::move(data)}, next{std::move(next)} {}
};

template <typename T>
class LinkedList {
    Node<T>* root_{};

public:
    LinkedList() : root_{nullptr} {}

    void push_front() {

    }

    void push_back() {

    }
};

