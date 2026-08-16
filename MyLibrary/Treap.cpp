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
  inline static std::mt19937 mt{std::random_device{}()};
  std::deque<Node> nodes;
  Compare comp{};
  Node *root = nullptr;
  Node sentinel;

  /**
   * @brief 部分木のサイズ
   */
  int size(Node *t) const {
    return t ? t->siz : 0;
  }

  /**
   * @brief 部分木のサイズを更新
   */
  void update(Node *t) const {
    if (t) {
      t->siz = 1 + size(t->left) + size(t->right);
    }
  }

  /**
   * @brief 部分木のサイズを更新しながら親ノードを辿る
   */
  void update_up(Node *t) {
    while (t) {
      update(t);
      t = t->parent;
    }
  }

  /**
   * @brief 乱数を取得
   */
  uint32_t next_priority() {
    return mt();
  }

  /**
   * @brief operator<
   */
  bool less_value(const ValueType &a, const ValueType &b) const {
    return comp(a, b);
  }

  /**
   * @brief oprator==
   */
  bool equal_value(const ValueType &a, const ValueType &b) const {
    return !comp(a, b) && !comp(b, a);
  }

  /**
   * @brief 子ノードを付け替える
   *      /           /             /           /
   *     par   ->    par     or    par   ->    par
   *    /   \       /   \         /   \       /   \
   *   old   ?     new   ?       ?    old    ?    new
   *
   * @pre old_childの親ノード情報は更新済み
   */
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

  /**
   * @brief 左回転
   *       /           /
   *      x    ->     r
   *     / \         / \
   *    l   r       x  rr
   *       / \     / \
   *      rl rr   l  rl
   * @pre pri(x) < pri(r)
   * で，それより下層のノードの優先度の大小関係は保たれている
   */
  void rotate_left(Node *x) {
    Node *r = x->right;
    assert(r);
    Node *p = x->parent;
    // ここはおまけ
    x->right = r->left;
    if (x->right) {
      x->right->parent = x;
    }
    // ここが本命
    r->left = x;
    x->parent = r;
    replace_child(p, x, r);
    update(x);
    update(r);
  }

  /**
   * @brief 右回転
   *       /           /
   *      x    ->     l
   *     / \         / \
   *    l   r       ll  x
   *   / \             / \
   *  ll lr           lr  r
   * @pre pri(x) < pri(l)
   * で，それより下層のノードの優先度の大小関係は保たれている
   */
  void rotate_right(Node *x) {
    Node *l = x->left;
    assert(l);
    Node *p = x->parent;
    // ここはおまけ
    x->left = l->right;
    if (x->left) {
      x->left->parent = x;
    }
    // ここが本命
    l->right = x;
    x->parent = l;
    replace_child(p, x, l);
    update(x);
    update(l);
  }

  /**
   * @brief ノードを検索
   * 二分木上を根から探索
   * @return ノードのポインタ．見つからなければnullptr
   */
  Node *find_node(const ValueType &val) const {
    Node *cur = root;

    while (cur) {
      if (less_value(val, cur->value)) {
        cur = cur->left;
      } else if (less_value(cur->value, val)) {
        cur = cur->right;
      } else {
        return cur;
      }
    }

    return nullptr;
  }

  /**
   * @brief ノードtを削除
   * ノードtを葉ノードまでおろしてから，葉ノードを削除
   * @todo 使わなくなったdeque上の領域を再利用
   */
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

  /**
   * @brief 左からk番目のノードのポインタを返す
   */
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

  /**
   * @brief 最も左のノードのポインタを返す
   */
  Node *min_node(Node *t) const {
    if (!t) {
      return nullptr;
    }
    while (t->left) {
      t = t->left;
    }
    return t;
  }

  /**
   * @brief 最も右のノードのポインタを返す
   */
  Node *max_node(Node *t) const {
    if (!t) {
      return nullptr;
    }
    while (t->right) {
      t = t->right;
    }
    return t;
  }

  // ================================================
  // TreapIterator
  // ================================================

  struct TreapIterator {
  private:
    Node *node = nullptr;   ///< イテレータがさすノード
    Treap *owner = nullptr; ///< イテレータが属するTreap
    explicit TreapIterator(Node *n, Treap *o) : node(n), owner(o) {}
    friend struct Treap;

  public:
    /**
     * @brief 標準iteratorAPIとの連携
     */
    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = const ValueType &;
    using pointer = const ValueType *;

    TreapIterator() = default;

    reference operator*() const {
      assert(node != &owner->sentinel);
      return node->value;
    }

    pointer operator->() const {
      assert(node != &owner->sentinel);
      return std::addressof(node->value);
    }

    /**
     * @brief 前置インクリメント
     * 更新後のイテレータの参照を返す
     * ひとつ右のノードは，右子ノードの左子ノードを辿って行った葉ノード or 右親ノード
     */
    TreapIterator &operator++() {
      assert(node != &owner->sentinel);
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
        node = par ? par : &owner->sentinel;
      }
      return *this;
    }

    /**
     * @brief 後置インクリメント
     * 更新前のイテレータのコピーを返す
     */
    TreapIterator operator++(int) {
      TreapIterator old = *this;
      ++(*this);
      return old;
    }

    /**
     * @brief 前置デクリメント
     * 更新後のイテレータの参照を返す
     * itr == end() のときは，最大要素を指すイテレータを返す
     */
    TreapIterator &operator--() {
      if (node == &owner->sentinel) {
        node = owner->root;
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
        node = par ? par : &owner->sentinel;
      }
      return *this;
    }

    /**
     * @brief 後置デクリメント
     * 更新前のイテレータのコピーを返す
     */
    TreapIterator operator--(int) {
      TreapIterator old = *this;
      --(*this);
      return old;
    }

    friend bool operator==(const TreapIterator &a, const TreapIterator &b) {
      return a.node == b.node && a.owner == b.owner;
    }
  };

public:
  Treap() {
    sentinel.parent = nullptr;
    sentinel.left = nullptr;
    sentinel.right = nullptr;
  }

  /**
   * @brief 要素数を返す
   */
  int size() const {
    return n;
  }

  bool empty() const {
    return n == 0;
  }

  bool contains(const ValueType &val) const {
    return find_node(val) != nullptr;
  }

  int count(const ValueType &val) const {
    return order_of_upper_bound(val) - order_of_lower_bound(val);
  }

  TreapIterator kth_element(int k) {
    Node *t = kth_node(k);
    return TreapIterator(t, this);
  }

  int index(const ValueType &val) const {
    return contains(val) ? order_of_lower_bound(val) : -1;
  }

  int index(const TreapIterator &itr) const {
    assert(itr.owner == this);
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
    return TreapIterator(t ? t : &sentinel, this);
  }

  ValueType &operator[](int k) {
    return kth_node(k)->value;
  }

  const ValueType &operator[](int k) const {
    return kth_node(k)->value;
  }

  /**
   * @brief 新しいノードを作って適切な位置に挿入
   */
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

  /**
   * @keyに一致するノードをひとつ削除
   */
  bool erase(const ValueType &val) {
    Node *t = find_node(val);
    if (!t) {
      return false;
    }
    erase_node(t);
    return true;
  }

  /**
   * @brief イテレータのさすノードを削除
   * @todo 削除したノードの次のノードを指すイテレータを返す
   */
  void erase(TreapIterator itr) {
    assert(itr.owner == this);
    assert(itr.node != &sentinel);
    erase_node(itr.node);
  }

  /**
   * @brief keyに一致する要素をすべて削除
   * @return 削除された要素数
   */
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

  /**
   * @todo clear()
   * @brief 全要素の削除
   */

  /**
   * @brief 最初の要素を指すイテレータを返す
   */
  TreapIterator begin() {
    Node *b = min_node(root);
    return TreapIterator(b ? b : &sentinel, this);
  }

  /**
   * @brief 最終要素の次を指すイテレータを返す
   */
  TreapIterator end() {
    return TreapIterator(&sentinel, this);
  }

  /**
   * @brief key以上の最初の要素を指すイテレータを返す（昇順のとき）
   */
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

    return TreapIterator(res ? res : &sentinel, this);
  }

  /**
   * @brief keyより大きい最初の要素を指すイテレータを返す（昇順のとき）
   */
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

    return TreapIterator(res ? res : &sentinel, this);
  }

  /**
   * @brief key未満の値の要素の個数を返す
   * index(lower_bound(key))とほぼ等価
   */
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

  /**
   * @brief key以下の値の要素の個数を返す
   * index(upper_bound(key))とほぼ等価
   */
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

int main() {
  int N = 20;
  std::vector<int> a(N);
  std::iota(a.begin(), a.end(), 0);
  Treap<int> tp;
  for (int i = 0; i < N; ++i) {
    tp.insert(i);
  }
  for (int i : tp) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
  tp.erase(tp[5]);
  for (int i : tp) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
  tp.insert(4);
  std::cout << tp.order_of_lower_bound(4) << std::endl;
  std::cout << tp.index(3) << std::endl;
  auto itr = tp.find(3);
  std::cout << tp.index(itr) << std::endl;
}