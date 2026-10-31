#pragma once

namespace data {

/// @brief A base for singly-linked node structures
struct s_link {
    s_link *next = this;
};

/// @brief A base for doubly-linked node structure
struct d_link {
    d_link *prev = this;
    d_link *next = this;
};

/// @brief A linked node structure
/// @tparam T The type of data to be stored in the node
/// @tparam L The link type used by the node
template <typename T, typename link> struct node : link {

    T data{};

    template <typename Tp> class _iterator {
    public:
        using node = data::node<T, link>;

        explicit _iterator(link *current) : current_(current) {}

        /// @brief Returns the underlying link pointer
        link *base() const { return current_; }

        Tp &operator*() const { return static_cast<node *>(current_)->data; }
        Tp *operator->() const { return &static_cast<node *>(current_)->data; }

        _iterator &operator++() {
            current_ = current_->next;
            return *this;
        }

        _iterator &operator--()
            requires requires(link *p) { p->prev; }
        {
            current_ = current_->prev;
            return *this;
        }

        template <typename Tp2>
        bool operator==(const _iterator<Tp2> &rhs) const {
            return current_ == rhs.current_;
        }
        template <typename Tp2>
        bool operator!=(const _iterator<Tp2> &rhs) const {
            return !(*this == rhs);
        }

    private:
        template <typename> friend class _iterator;
        link *current_;
    };

    using iterator = _iterator<T>;
    using const_iterator = _iterator<const T>;
};
} // namespace data
