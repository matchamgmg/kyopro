/* Treap
 *
 * ランダムアクセス可能な平衡二分木
 * メソッド
 *  insert(val)
 *  erase(val)
 *  operator[]
 *  contains(val)
 *  lower_bound(val)
 */

#include <bits/stdc++.h>

template <class KeyType, class ValueType> struct Treap {
  struct Node {
  private:
    float priority;
    Node *left, *right;

  public:
    ValueType value;
  };

  int n;
  std::vector<Node> nodes;

  struct TreapIterator {
  private:
    Node *p = nullptr;
    explicit TreapIterator(Node *node) : p(node) {}
    friend class Treap;

  public:
    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using pointer = ValueType *;
    using reference = ValueType &;

    TreapIterator() = default;

    reference operator*() const { return p->value; }

    pointer operator->() const { return std::addressof(p->value); }

    TreapIterator &operator++() {
      p = p->right;
      return *this;
    }

    TreapIterator operator++(int) {
      TreapIterator old = *this;
      ++(*this);
      return old;
    }

    TreapIterator &operator--() {
      p = p->left;
      return *this;
    }

    TreapIterator operator--(int) {
      TreapIterator old = *this;
      --(*this);
      return old;
    }

    friend bool operator==(TreapIterator a, TreapIterator b) { return a.p == b.p; }
  };

  TreapIterator begin() { return TreapIterator }
};

int main() {}