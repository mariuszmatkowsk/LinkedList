#pragma once

#include <algorithm>
#include <utility>
#include <format>
#include <ostream>
#include <expected>
#include <functional>

enum class LinkedListError {
    EmptyList,
};

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
    using node           = std::conditional_t<
                                        std::is_const_v<T>,
                                        const Node<std::remove_const_t<T>>,
                                        Node<T>>;
    using node_pointer   = node*;
    using node_reference = node&;

    node_pointer current_;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = node;
    using difference_type   = std::ptrdiff_t;
    using pointer           = node_pointer;
    using reference         = node_reference;

    explicit constexpr LinkedListIterator(node_pointer current)
        : current_(current) {}

    constexpr T& operator*()  const { return  current_->data; };
    constexpr T* operator->() const { return &current_->data; };

    constexpr LinkedListIterator& operator++() {
        current_ = current_->next;
        return *this;
    }

    friend constexpr auto operator==(
            const LinkedListIterator& lhs, const LinkedListIterator& rhs) {
        return lhs.current_ == rhs.current_;
    }

    friend constexpr auto operator!=(
            const LinkedListIterator& lhs, const LinkedListIterator& rhs) {
        return !(lhs == rhs);
    }
};

template <typename T>
class LinkedList {
    using node = Node<T>;
    using node_pointer = node*;

    node_pointer root_{};
    std::size_t  size_{};

    constexpr void copy_from(const LinkedList& other) {
        node_pointer* current = &root_;
        for (node_pointer other_current{other.root_}; other_current;
                other_current = other_current->next,
                current = &((*current)->next)) {
            *current = new Node{other_current->data};
        }
        size_ = other.size_;
    }

    std::expected<node_pointer, LinkedListError> get_last() const {
        if (!root_) return std::unexpected(LinkedListError::EmptyList);

        auto current = root_;
        for(; current->next; current = current->next);
        return current;
    }

    friend struct std::formatter<LinkedList>;

public:
    using value_type      = T;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator        = LinkedListIterator<T>;
    using const_iterator  = LinkedListIterator<const T>;

    constexpr LinkedList() : root_{nullptr}, size_{0} {}

    explicit LinkedList(T data) : root_{new node{std::move(data)}}, size_{1} {}

    explicit constexpr LinkedList(std::initializer_list<T> elements)
            : root_{nullptr}, size_{0} {
        node_pointer* current = &root_;
        for (auto it{elements.begin()}; it != elements.end();
                ++it, current = &((*current)->next)) {
            *current = new node(*it);
            ++size_;
        }
    }

    constexpr LinkedList(const LinkedList& other) : root_{nullptr}, size_{0} {
        copy_from(other);
    }

    constexpr LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this;

        clear();
        copy_from(other);
        return *this;
    }

    constexpr LinkedList(LinkedList&& other) noexcept { swap(*this, other); }

    constexpr LinkedList& operator=(LinkedList&& other) noexcept {
        // copy & swap idiom
        LinkedList tmp(std::move(other));
        swap(*this, tmp);
        return *this;
    }

    constexpr ~LinkedList() { clear(); }

    constexpr void clear() {
        for (node_pointer current{root_}; current;) {
            const auto next = current->next;
            delete current;
            current = next;
        }
        root_ = nullptr;
        size_ = 0;
    }

    friend constexpr void swap(LinkedList& l1, LinkedList& l2) noexcept {
        using std::swap;
        swap(l1.root_, l2.root_);
        swap(l1.size_, l2.size_);
    }

    [[nodiscard]]
    constexpr std::size_t size() const { return size_; };

    [[nodiscard]]
    constexpr auto begin() { return iterator{root_}; }

    [[nodiscard]]
    constexpr auto end() { return iterator{nullptr}; }

    [[nodiscard]]
    constexpr auto begin() const { return const_iterator{root_}; }

    [[nodiscard]]
    constexpr auto end() const { return const_iterator{nullptr}; }

    [[nodiscard]]
    constexpr auto is_empty() const { return root_ == nullptr; }

    [[nodiscard]]
    constexpr auto contains(const_reference value) const {
        return std::find(begin(), end(), value) != end();
    }

    [[nodiscard]]
    std::expected<std::reference_wrapper<value_type>, LinkedListError>
    front() {
        if (!root_) return std::unexpected(LinkedListError::EmptyList);
        return root_->data;
    }

    [[nodiscard]]
    std::expected<std::reference_wrapper<const value_type>, LinkedListError>
    front() const {
        if (!root_) return std::unexpected(LinkedListError::EmptyList);
        return root_->data;
    }

    [[nodiscard]]
    std::expected<std::reference_wrapper<value_type>, LinkedListError>
    back() {
        auto last = get_last();
        if (last) {
            return last.value()->data;
        }
        return std::unexpected(last.error());
    }

    [[nodiscard]]
    std::expected<std::reference_wrapper<const value_type>, LinkedListError>
    back() const {
        auto last = get_last();
        if (last) {
            return last.value()->data;
        }
        return std::unexpected(last.error());
    }

    void push_front(value_type data) {
        const auto next = root_;
        root_ = new node{std::move(data), next};
        ++size_;
    }

    void pop_front() {
        if (!root_) return;

        const auto next = root_->next;
        delete root_;
        root_ = next;
        --size_;
    }

    void push_back(value_type data) {
        const auto new_node = new node{std::move(data)};

        auto last = get_last();
        if (last) {
            last.value()->next = new_node;
        } else {
            root_ = new_node;
        }
        ++size_;
    }

    void pop_back() {
        if (!root_) return;

        if (!root_->next) {
            delete root_;
            root_ = nullptr;
            --size_;
            return;
        }

        auto prev = root_;
        auto current = root_->next;
        for (; current->next; prev = current, current = current->next);
        delete current;
        prev->next = nullptr;
        --size_;
    }

    friend constexpr auto operator==(
            const LinkedList& lhs, const LinkedList& rhs) {
        if (lhs.size() != rhs.size()) return false;
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    friend std::ostream& operator<<(std::ostream& os, const LinkedList& ll) {
        os << "[";
        std::for_each(std::begin(ll), std::end(ll), [&os](const auto element) {
            os << element << " -> "; });
        os << "NULL]";
        return os;
    };
};

template <typename T>
struct std::formatter<LinkedList<T>> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const LinkedList<T>& ll, std::format_context& ctx) const {
        const auto& out = ctx.out();
        std::format_to(out, "[");
        std::for_each(std::begin(ll), std::end(ll), [&out](const auto& element)
            { std::format_to(out, "{} {} ", element, "->"); });
        std::format_to(out, "NULL");
        return std::format_to(out, "]");
    }
};

