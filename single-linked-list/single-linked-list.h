#pragma once
#include <cstddef>
#include <string>
#include <utility>
#include <cassert>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node)
            : node_(node) {

        }

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
            : node_(other.node_) {

        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !operator==(rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !operator==(rhs);
        }

        BasicIterator& operator++() noexcept {
            assert(node_);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            assert(node_);
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_);
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

    template <typename T>
    void ListInitialization(T& other) {
        SingleLinkedList<Type> tmp;
        for (auto item : other) {
            tmp.PushFront(item);
        }
        SingleLinkedList<Type> tmp2;
        for (auto item : tmp) {
            tmp2.PushFront(item);
        }
        swap(tmp2);
    }

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{ &head_ };
    }

    SingleLinkedList() {
        size_ = 0u;
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        ListInitialization(values);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        ListInitialization(other);
    }

    ~SingleLinkedList() {
        Clear();
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0u;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            auto tmp = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete tmp;
        }
        size_ = 0;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }

    void PopFront() noexcept {
        assert(!IsEmpty());
        auto tmp = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete tmp;
        --size_;

    }
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList<Type> tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        auto insertedNode = new Node(value, pos.node_->next_node);
        pos.node_->next_node = insertedNode;
        ++size_;
        return Iterator{ insertedNode };
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());
        if (size_ == 1) {
            --size_;
            return end();
        }
        else {
            auto tmp = pos.node_->next_node;
            pos.node_->next_node = pos.node_->next_node->next_node;
            delete tmp;
            --size_;
            return Iterator{ pos.node_->next_node };
        }
    }

private:
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (std::equal(lhs.begin(), lhs.end(), rhs.begin())) && (lhs.GetSize() == rhs.GetSize());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}