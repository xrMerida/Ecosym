#pragma once

namespace data {

/// @brief A base for singly-linked node structures
/// @note the default constructor assigns %next to itself
struct s_link {
    s_link() = default;
    s_link(s_link *_next) : next(_next) {}
    s_link *next = this;
};

/// @brief A base for doubly-linked node structure
/// @note the default constructor assigns %next and %prev to itself
struct d_link {
    d_link() = default;
    d_link(d_link *_prev, d_link *_next) : prev(_prev), next(_next) {}
    d_link *prev = this;
    d_link *next = this;
};

/// @brief A linked node structure
/// @tparam T The type of data to be stored in the node
/// @tparam L The link type used by the node
template <typename T, typename L> struct node : public L {
    using link = L;

    T data{};

    class iterator {
      public:
        using node = data::node<T, L>;
        explicit iterator(link *current) : current_(current) {}

        /// @brief A read-write pointer to the holding node
        link *base() const { return current_; }

        T &operator*() const { return static_cast<node *>(current_)->data; }
        T *operator->() const { return &static_cast<node *>(current_)->data; }

        iterator &operator++() {
            current_ = current_->next;
            return *this;
        }

        iterator &operator--()
            requires requires(link *p) { p->prev; }
        {
            current_ = current_->prev;
            return *this;
        }

        bool operator==(const iterator &rhs) const {
            return current_ == rhs.current_;
        }
        bool operator!=(const iterator &rhs) const {
            return current_ != rhs.current_;
        }

      private:
        link *current_;
    };
};
} // namespace data
