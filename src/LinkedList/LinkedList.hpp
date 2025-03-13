#pragma once

template <typename T>
struct Node {
    T     data{};
    Node* next{};
};

template <typename T>
class LinkedList {
    Node<T>* root_{};

public:
    constexpr LinkedList() : root_{nullptr} {}
};

