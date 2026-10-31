#pragma once

#include "data/node.h"
#include <cassert>
#include <cstddef>

namespace data {
/// @brief doubly-linked list
template <typename T> class list {
public:
    using link = data::d_link;
    using node = data::node<T, link>;
    using iterator = node::iterator;
    using const_iterator = node::const_iterator;

    list() = default;
    ~list() { clear(); }

    list(const list &) = delete;
    list(list &&) = delete;
    list &operator=(const list &) = delete;
    list &operator=(list &&) = delete;

    [[nodiscard]] size_t size() const { return size_; }
    [[nodiscard]] bool empty() const { return size_ == 0; }

    /// @brief Returns a read-write iterator to the first element of the list
    iterator begin() { return iterator(sentinel.next); }

    /// @brief Returns a read-write iterator to the sentinel of the list
    /// @note Must not be instantiated
    /// Iterator points to one past the last element of the doubly-linked list
    /// reducing the iterator by one will make it point to the last element of
    /// the list
    iterator end() { return iterator(&sentinel); }

    /// @brief Returns a read-only iterator to the first element of the list
    const_iterator begin() const { return cbegin(); }
    const_iterator cbegin() const { return const_iterator(sentinel.next); }

    /// @brief Returns a read-only iterator to the sentinel of the list
    /// @note Must not be instantiated
    /// Iterator points to one past the last element of the doubly-linked list
    /// reducing the iterator by one will make it point to the last element of
    /// the list
    const_iterator end() const { return cend(); }
    const_iterator cend() const {
        return const_iterator(const_cast<link *>(&sentinel));
    }

    /// @brief Insert an element in the list
    /// @param pos Iterator holding the position before which to insert
    /// @param data Data to be inserted
    ///
    /// This function does not check the %position belongs to this list to
    /// achieve constant time
    constexpr void insert(const iterator &pos, const T &data) {
        link *next = pos.base();
        auto *n = new node{{next->prev, next}, data};
        n->prev->next = n;
        next->prev = n;
        size_++;
    }
    /// @brief Inserts %data at the end of the list
    constexpr void push_back(const T &data) { insert(end(), data); }
    /// @brief Inserts %data at the beginning of the list
    constexpr void push_front(const T &data) { insert(begin(), data); }

    /// @brief Removes the data in the given position
    /// @param pos Iterator containing the node to erase
    ///
    /// This function does not check the %position belongs to this list to
    /// achieve constant time
    iterator erase(const iterator &pos) {
        assert(pos != end());

        auto *old = static_cast<node *>(pos.base());
        link *next = old->next;
        (old->next)->prev = old->prev;
        (old->prev)->next = old->next;
        delete old;
        size_--;
        return iterator(next);
    }

    /// @brief Clears every element from the list
    void clear() {
        link *l = sentinel.next;
        while (l != &sentinel) {
            node *old = static_cast<node *>(l);
            l = l->next;
            delete old;
        }
        sentinel.next = &sentinel;
        sentinel.prev = &sentinel;
        size_ = 0;
    }

private:
    link sentinel{};
    size_t size_ = 0;
};
} // namespace data

#ifndef NDEBUG
namespace data::test {
inline void list_self_check() {
    list<int> test;
    assert(test.size() == 0 && test.empty());
    test.push_back(2);
    test.push_front(1);
    test.insert(++test.begin(), 4);

    int sum = 0;
    for (int value : test) {
        sum += value;
    }
    assert(sum == 7);

    assert(*test.cbegin() == 1);
    assert(*test.cend() == *test.end());

    test.erase(++test.begin());
    assert(test.size() == 2);
    test.erase(--test.end());
    assert(test.size() == 1);
    assert(*test.begin() == 1);

    test.clear();
    assert(test.empty());
}
} // namespace data::test
#endif
