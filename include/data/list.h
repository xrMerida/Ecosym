#pragma once

#include "data/node.h"
#include <cassert>
#include <cstddef>

namespace data {
template <typename T> class list {
  public:
    using link = data::d_link;
    using node = data::node<T, link>;
    using iterator = node::iterator;
    using size_type = std::size_t;

    list() = default;
    ~list() { clear(); }

    list(const list &) = delete;
    list(list &&) = delete;
    list &operator=(const list &) = delete;
    list &operator=(list &&) = delete;

    [[nodiscard]] size_type size() const { return size_; }
    [[nodiscard]] bool empty() const { return size_ == 0; }

    iterator begin() { return iterator(sentinel.next); }
    iterator end() { return iterator(&sentinel); }

    const iterator begin() const { return iterator(sentinel.next); }
    const iterator end() const { return iterator(&sentinel); }

    void insert(const iterator &pos, const T &data) {
        link *next = pos.base();
        auto *n = new node{{next->prev, next}, data};
        n->prev->next = n;
        next->prev = n;
        size_++;
    }
    void push_back(const T &data) { insert(end(), data); }
    void push_front(const T &data) { insert(begin(), data); }

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
    size_type remove(const T &data) {
        size_type removed = 0;
        link *aliased = nullptr;
        for (auto it = begin(); it != end();) {
            if (*it != data) {
                ++it;
                continue;
            }
            if (&(*it) == &data) {
                aliased = it.base();
                ++it;
                continue;
            }
            it = erase(it);
            removed++;
        }
        if (aliased) {
            erase(iterator(aliased));
            removed++;
        }
        return removed;
    }
    void clear() {
        link *l = sentinel.next;
        while (l != &sentinel) {
            auto old = static_cast<node *>(l);
            l = l->next;
            delete old;
        }
        sentinel.next = &sentinel;
        sentinel.prev = &sentinel;
        size_ = 0;
    }

  private:
    link sentinel{};
    size_type size_ = 0;
};
} // namespace data

#ifndef NDEBUG
namespace data::test {
inline void list_self_check() {
    list<int> test;
    test.push_back(2);
    test.push_front(1);
    test.insert(test.begin(), 4);

    int sum = 0;
    for (int value : test) {
        sum += value;
    }
    (void)sum;

    test.erase(test.begin());
    test.remove(*test.begin());

    test.clear();
    (void)test.size();
}
} // namespace data::test
#endif
