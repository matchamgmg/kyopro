/* Treap
 *
 * ランダムアクセス可能な平衡二分木
 *
 * template parameter:
 *  ValueType:  データの型
 *  Compare:    比較関数
 *  Multi:      重複あり/なし
 *
 * methods:
 *  size() -> int
 *  empty() -> bool
 *  insert(val) -> bool
 *  erase(val)  -> bool
 *  erase(iterator) -> void
 *  erase_all(val)  -> int
 *  operator[]  -> val
 *  index(val) -> int
 *  index(iterator) -> int
 *  kth_element(idx) -> iterator
 *  contains(val) -> bool
 *  count(val) -> int
 *  lower_bound(val) -> iterator
 *  upper_bound(val) -> iterator
 *  order_of_lower_bound(val) -> int
 *  order_of_upper_bound(val) -> int
 *  find(val) -> iterator
 */

#include <bits/stdc++.h>

template <class ValueType, class Compare = std::less<ValueType>, bool Multi = false> struct Treap {
private:
  struct Node {
  private:
    ValueType value{};
    int siz = 1;
    uint32_t priority;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;

    friend struct Treap;
    friend struct TreapIterator;

  public:
    Node() = default;
    explicit Node(const ValueType &val, const uint32_t pri) : value(val), priority(pri) {}
  };

  int n = 0;
  std::mt19937 mt;
  std::deque<Node> nodes;
  Compare comp{};
  Node *root = nullptr;
  Node sentinel;

  int size(Node *t) const { return t ? t->siz : 0; }

  void update(Node *t) const {
    if (t) {
      t->siz = 1 + size(t->left) + size(t->right);
    }
  }

  void update_up(Node *t) {
    while (t) {
      update(t);
      t = t->parent;
    }
  }

  uint32_t next_priority() { return mt(); }

  bool less_value(const ValueType &a, const ValueType &b) const { return comp(a, b); }

  bool equal_value(const ValueType &a, const ValueType &b) const { return !comp(a, b) && !comp(b, a); }

  void replace_child(Node *parent, Node *old_child, Node *new_child) {
    if (!parent) {
      root = new_child;
    } else if (parent->left == old_child) {
      parent->left = new_child;
    } else {
      parent->right = new_child;
    }

    if (new_child) {
      new_child->parent = parent;
    }
  }

  void rotate_left(Node *x) {
    Node *y = x->right;
    assert(y);
    Node *p = x->parent;
    x->right = y->left;
    if (x->right) {
      x->right->parent = x;
    }
    y->left = x;
    x->parent = y;
    replace_child(p, x, y);
    update(x);
    update(y);
  }

  void rotate_right(Node *x) {
    Node *y = x->left;
    assert(y);
    Node *p = x->parent;
    x->left = y->right;
    if (x->left) {
      x->left->parent = x;
    }
    y->right = x;
    x->parent = y;
    replace_child(p, x, y);
    update(x);
    update(y);
  }

  Node *find_node(const ValueType &val) const {
    Node *cur = root;

    while (cur) {
      if (equal_value(val, cur->value)) {
        return cur;
      }

      if (less_value(val, cur->value)) {
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }

    return nullptr;
  }

  void erase_node(Node *t) {
    while (t->left || t->right) {
      if (!t->left) {
        rotate_left(t);
      } else if (!t->right) {
        rotate_right(t);
      } else if (t->left->priority > t->right->priority) {
        rotate_right(t);
      } else {
        rotate_left(t);
      }
    }

    Node *p = t->parent;
    replace_child(p, t, nullptr);
    update_up(p);
    --n;
  }

  Node *kth_node(int k) const {
    assert(0 <= k && k < n);
    Node *cur = root;

    while (cur) {
      int lsz = size(cur->left);

      if (k < lsz) {
        cur = cur->left;
      } else if (k == lsz) {
        return cur;
      } else {
        k -= lsz + 1;
        cur = cur->right;
      }
    }

    assert(false);
    return nullptr;
  }

  Node *min_node(Node *t) const {
    if (!t) {
      return nullptr;
    }
    while (t->left) {
      t = t->left;
    }
    return t;
  }

  Node *max_node(Node *t) const {
    if (!t) {
      return nullptr;
    }
    while (t->right) {
      t = t->right;
    }
    return t;
  }

  struct TreapIterator {
  private:
    Node *node = nullptr;
    Node *root = nullptr;
    Node *sentinel = nullptr;
    explicit TreapIterator(Node *n, Node *r, Node *s) : node(n), root(r), sentinel(s) {}
    friend struct Treap;

  public:
    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = const ValueType &;
    using pointer = const ValueType *;

    TreapIterator() = default;

    const ValueType &operator*() const {
      assert(node != sentinel);
      return node->value;
    }

    const ValueType *operator->() const {
      assert(node != sentinel);
      return std::addressof(node->value);
    }

    TreapIterator &operator++() {
      assert(node != sentinel);
      if (node->right) {
        node = node->right;
        while (node->left) {
          node = node->left;
        }
      } else {
        Node *par = node->parent;
        while (par && node == par->right) {
          node = par;
          par = node->parent;
        }
        node = par ? par : sentinel;
      }
      return *this;
    }

    TreapIterator operator++(int) {
      TreapIterator old = *this;
      ++(*this);
      return old;
    }

    TreapIterator &operator--() {
      if (node == sentinel) {
        node = root;
        assert(node);

        while (node->right) {
          node = node->right;
        }
        return *this;
      }

      if (node->left) {
        node = node->left;
        while (node->right) {
          node = node->right;
        }
      } else {
        Node *par = node->parent;
        while (par && node == par->left) {
          node = par;
          par = node->parent;
        }
        node = par ? par : sentinel;
      }
      return *this;
    }

    TreapIterator operator--(int) {
      TreapIterator old = *this;
      --(*this);
      return old;
    }

    friend bool operator==(TreapIterator a, TreapIterator b) { return a.node == b.node; }

    friend bool operator!=(TreapIterator a, TreapIterator b) { return !(a == b); }
  };

public:
  Treap() : mt(std::random_device{}()) {
    sentinel.parent = nullptr;
    sentinel.left = nullptr;
    sentinel.right = nullptr;
  }

  int size() const { return n; }

  bool empty() const { return n == 0; }

  bool contains(const ValueType &val) const { return find_node(val) != nullptr; }

  int count(const ValueType &val) const { return order_of_upper_bound(val) - order_of_lower_bound(val); }

  TreapIterator kth_element(int k) {
    Node *t = kth_node(k);
    return TreapIterator(t, root, &sentinel);
  }

  int index(const ValueType &val) const { return contains(val) ? order_of_lower_bound(val) : -1; }

  int index(const TreapIterator &itr) const {
    Node *t = itr.node;
    assert(t != &sentinel);

    int res = size(t->left);
    while (t->parent) {
      if (t == t->parent->right) {
        res += size(t->parent->left) + 1;
      }
      t = t->parent;
    }
    return res;
  }

  TreapIterator find(const ValueType &val) {
    Node *t = find_node(val);
    return TreapIterator(t ? t : &sentinel, root, &sentinel);
  }

  ValueType &operator[](int k) { return kth_node(k)->value; }

  const ValueType &operator[](int k) const { return kth_node(k)->value; }

  bool insert(const ValueType &val) {
    if (!root) {
      nodes.emplace_back(val, next_priority());
      root = &nodes.back();
      ++n;
      return true;
    }

    Node *cur = root;
    Node *par = nullptr;
    while (cur) {
      par = cur;

      if constexpr (!Multi) {
        if (equal_value(val, cur->value)) {
          return false;
        }
      }

      if (less_value(val, cur->value)) {
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }

    nodes.emplace_back(val, next_priority());
    Node *z = &nodes.back();

    z->parent = par;

    if (less_value(val, par->value)) {
      par->left = z;
    } else {
      par->right = z;
    }

    update_up(par);

    while (z->parent && z->priority > z->parent->priority) {
      if (z == z->parent->left) {
        rotate_right(z->parent);
      } else {
        rotate_left(z->parent);
      }
    }
    ++n;
    return true;
  }

  bool erase(const ValueType &val) {
    Node *t = find_node(val);
    if (!t) {
      return false;
    }
    erase_node(t);
    return true;
  }

  void erase(TreapIterator itr) {
    assert(itr.node != &sentinel);
    erase_node(itr.node);
  }

  int erase_all(const ValueType &val) {
    int cnt = 0;

    while (true) {
      Node *t = find_node(val);
      if (!t) {
        break;
      }
      erase_node(t);
      ++cnt;
    }
    return cnt;
  }

  TreapIterator begin() {
    Node *b = min_node(root);
    return TreapIterator(b ? b : &sentinel, root, &sentinel);
  }

  TreapIterator end() { return TreapIterator(&sentinel, root, &sentinel); }

  TreapIterator lower_bound(const ValueType &val) {
    Node *cur = root;
    Node *res = nullptr;

    while (cur) {
      if (!less_value(cur->value, val)) {
        res = cur;
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }

    return TreapIterator(res ? res : &sentinel, root, &sentinel);
  }

  TreapIterator upper_bound(const ValueType &val) {
    Node *cur = root;
    Node *res = nullptr;

    while (cur) {
      if (less_value(val, cur->value)) {
        res = cur;
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }

    return TreapIterator(res ? res : &sentinel, root, &sentinel);
  }

  // val未満の値の個数
  int order_of_lower_bound(const ValueType &val) const {
    int res = 0;
    Node *cur = root;

    while (cur) {
      if (less_value(cur->value, val)) {
        res += size(cur->left) + 1;
        cur = cur->right;
      } else {
        cur = cur->left;
      }
    }
    return res;
  }

  // val以下の値の個数
  int order_of_upper_bound(const ValueType &val) const {
    int res = 0;
    Node *cur = root;

    while (cur) {
      if (!less_value(val, cur->value)) {
        res += size(cur->left) + 1;
        cur = cur->right;
      } else {
        cur = cur->left;
      }
    }
    return res;
  }
};