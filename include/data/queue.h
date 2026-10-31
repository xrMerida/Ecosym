#pragma once

#include "data/node.h"
#include <cassert>
#include <cstddef>

namespace data {
template <typename T> class queue {
public:
    using link = data::s_link;
    using node = data::node<T, link>;

    queue(const queue &) = delete;
    queue(queue &&) = delete;
    queue &operator=(const queue &) = delete;
    queue &operator=(queue &&) = delete;

    queue() = default;
    ~queue() { clear(); }

    [[nodiscard]] size_t size() const { return size_; }
    [[nodiscard]] bool empty() const { return size_ == 0; }
    /// @brief Returns a read-write reference to element the the front of the
    /// %queue
    /// @note Must check container is non-empty
    [[nodiscard]] T &front() {
        assert(front_);
        return front_->data;
    }
    /// @brief Returns a read-only reference to element the the front of the
    /// %queue
    /// @note Must check container is non-empty
    [[nodiscard]] const T &front() const {
        assert(front_);
        return front_->data;
    }

    /// @brief Pushes %data into the end of the %queue
    /// @param data The value to be enqueued
    void push(const T &data) {
        node *n = new node{{nullptr}, data};
        if (tail_) {
            link *aux = tail_;
            aux->next = n;
            tail_ = static_cast<node *>(aux->next);
        } else {
            front_ = tail_ = n;
        }
        size_++;
    }
    /// @brief Removes the first element of the %queue
    /// @note no_op on empty queue
    void pop() {
        if (empty()) return;

        link *old = front_;
        front_ = static_cast<node *>(old->next);
        if (front_ == nullptr) tail_ = nullptr;
        delete static_cast<node *>(old);
        size_--;
    }

    /// @brief Clears every element contained in the list
    void clear() {
        while (!empty()) pop();
    }

private:
    node *front_ = nullptr;
    node *tail_ = nullptr;
    size_t size_ = 0;
};
} // namespace data

#ifndef NDEBUG
namespace data::test {
inline void queue_self_check() {
    queue<int> test;
    assert(test.size() == 0 && test.empty());
    test.push(2);
    test.push(1);
    assert(test.size() == 2);

    assert(test.front() == 2);
    test.pop();
    const queue<int> &const_test = test;
    assert(const_test.front() == 1);
    test.pop();
    assert(const_test.empty());

    test.push(1);
    test.push(2);
    test.push(3);
    test.clear();
    assert(test.empty());
}
} // namespace data::test
#endif
