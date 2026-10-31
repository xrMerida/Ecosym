#pragma once

#include "data/node.h"
#include <cassert>
#include <cstddef>

namespace data {
template <typename T> class stack {
public:
    using link = data::s_link;
    using node = data::node<T, link>;

    stack(const stack &) = delete;
    stack(stack &&) = delete;
    stack &operator=(const stack &) = delete;
    stack &operator=(stack &&) = delete;

    stack() = default;
    ~stack() { clear(); }

    [[nodiscard]] size_t size() const { return size_; }
    [[nodiscard]] bool empty() const { return size_ == 0; }

    /// @brief Returns a read-write reference to element the the top of the
    /// %stack
    /// @note Must check container is not empty
    [[nodiscard]] T &top() {
        assert(top_);
        return top_->data;
    }
    /// @brief Returns a read-only reference to element the top of the
    /// %stack
    /// @note Must check container is not empty
    [[nodiscard]] const T &top() const {
        assert(top_);
        return top_->data;
    }

    /// @brief Pushes %data onto the top of the %stack
    /// @param data The value to be stacked
    void push(const T &data) {
        node *n = new node;
        n->next = top_;
        n->data = data;
        top_ = n;
        size_++;
    }
    /// @brief Removes the last element of the %stack
    /// @note no_op on empty stack
    void pop() {
        if (empty()) return;

        link *old = top_;
        top_ = static_cast<node *>(old->next);
        delete static_cast<node *>(old);
        size_--;
    }

    /// @brief Clears every element contained in the list
    void clear() {
        while (!empty()) pop();
    }

private:
    node *top_ = nullptr;
    size_t size_ = 0;
};
} // namespace data

#ifndef NDEBUG
namespace data::test {
inline void stack_self_check() {
    stack<int> test;

    assert(test.empty());
    assert(test.size() == 0);
    test.pop();
    assert(test.empty());

    test.push(1);
    test.push(2);
    test.push(3);
    assert(!test.empty());
    assert(test.size() == 3);
    assert(test.top() == 3);

    test.top() = 10;
    assert(test.top() == 10);
    const stack<int> &const_test = test;
    assert(const_test.top() == 10);

    test.pop();
    assert(const_test.size() == 2);
    assert(test.top() == 2);
    test.pop();
    assert(test.size() == 1);
    assert(test.top() == 1);
    test.pop();
    assert(const_test.empty());
    assert(test.size() == 0);

    test.pop();
    test.push(4);
    assert(test.top() == 4);
    test.clear();
    assert(test.empty());
    assert(test.size() == 0);
}
} // namespace data::test
#endif
