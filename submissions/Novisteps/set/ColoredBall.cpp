#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>;               // 大きい順
template <class T> using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = (s); i < (int)(n); i++)
#define rrep(i, s, n) for (int i = (n - 1); i >= (int)(s); i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
void pyes() {
  cout << "Yes" << endl;
}
void pno() {
  cout << "No" << endl;
}
void YN(bool x) {
  cout << (x ? "Yes" : "No") << endl;
}
template <class T> void v_cout(const vector<T> &a) {
  int n = a.size();
  rep(i, 0, n) cout << a[i] << " ";
  cout << endl;
}
template <class T> void vv_cout(const vector<T> &a) {
  int n = a.size();
  rep(i, 0, n) {
    rep(j, 0, a[i].size()) cout << a[i][j] << " ";
    cout << endl;
  }
}
bool grid_check(int x, int y, int X, int Y) {
  return (0 <= x && x < X && 0 <= y && y < Y);
}
template <class T> bool chmax(T &a, T b) {
  if (a < b) {
    a = b;
    return true;
  } else {
    return false;
  }
}
template <class T> bool chmin(T &a, T b) {
  if (a > b) {
    a = b;
    return true;
  } else {
    return false;
  }
}

/**
 * @brief 平衡二分探索木
 * @tparam ValueTpye : 要素の型．デフォルトコンストラクタが必要
 * @tparam Compare : ValueTypeの比較関数．struct ValueType のメソッドとしてbool operator<(const ValueType &rhs) const
 * が定義されていればOK
 * @tparam Multi : true...重複要素を別ノードとして管理．false...重複要素を持たない
 */
template <class ValueType, class Compare = std::less<ValueType>, bool Multi = false> struct Treap {
private:
  struct Node {
    friend struct Treap;
    template <bool> friend struct TreapIteratorBase;

    ValueType value{};
    int siz = 1;
    uint32_t priority = 0;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;

    Node() = default;

    explicit Node(const ValueType &val, const uint32_t pri) : value(val), priority(pri) {}
    explicit Node(ValueType &&val, const uint32_t pri) : value(std::move(val)), priority(pri) {}
  };

  int n = 0;
  inline static std::mt19937 mt{std::random_device{}()};
  std::deque<Node> nodes;
  std::vector<Node *> free_list;
  Compare comp{};
  Node *root = nullptr;
  Node sentinel;

  /**
   * @brief 部分木のサイズ
   */
  int size(const Node *t) const {
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
   * @brief 新しいノードを作ってdequeに登録する
   * free_listに空きがあればそちらから優先的に使う
   * @return 新しいノードのポインタ
   * @tparam T = const ValueType& or ValueType&&
   */
  template <class T> Node *new_node(T &&val) {
    if (!free_list.empty()) {
      Node *t = free_list.back();
      free_list.pop_back();

      *t = Node(std::forward<T>(val), next_priority());
      ++n;
      return t;
    } else {
      nodes.emplace_back(std::forward<T>(val), next_priority());
      ++n;
      return &nodes.back();
    }
  }

  /**
   * @brief ノードを初期化してfree_listに登録する
   */
  void delete_node(Node *t) {
    t->parent = nullptr;
    t->left = nullptr;
    t->right = nullptr;
    free_list.push_back(t);
    --n;
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
   * @brief 新しいノードを作って適切な位置に挿入
   * @tparam T = const ValueType& or ValueType&&
   */
  template <class T> bool insert_impl(T &&val) {
    if (!root) {
      root = new_node(std::forward<T>(val));
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

    Node *z = new_node(std::forward<T>(val));

    z->parent = par;

    if (less_value(z->value, par->value)) {
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
    return true;
  }

  /**
   * @brief ノードtを削除
   * ノードtを葉ノードまでおろしてから，葉ノードを削除
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

    delete_node(t);
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

  /**
   * @brief lower_bound
   * @return 見つかったノードのポインタ
   */
  Node *lower_bound_node(const ValueType &val) const {
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

    return res;
  }

  /**
   * @brief upper_bound
   * @return 見つかったノードのポインタ
   */
  Node *upper_bound_node(const ValueType &val) const {
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

    return res;
  }

  // ================================================
  // TreapIterator
  // ================================================

  /**
   * @brief TreapIterator and ConstTreapIterator
   */
  template <bool IsConst> struct TreapIteratorBase {
  private:
    using TreapPointer = std::conditional_t<IsConst, const Treap *, Treap *>;
    using NodePointer = std::conditional_t<IsConst, const Node *, Node *>;

    NodePointer node = nullptr;
    TreapPointer owner = nullptr;

    explicit TreapIteratorBase(NodePointer n, TreapPointer o) : node(n), owner(o) {}
    friend struct Treap;

    template <bool> friend struct TreapIteratorBase;

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

    TreapIteratorBase() = default;

    /**
     * @brief TreapIterator -> ConstTreapIteratorへの変換コンストラクタ
     */
    template <bool B = IsConst>
      requires B
    TreapIteratorBase(const TreapIteratorBase<false> &itr) : node(itr.node), owner(itr.owner) {}

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
    TreapIteratorBase &operator++() {
      assert(node != &owner->sentinel);
      if (node->right) {
        node = node->right;
        while (node->left) {
          node = node->left;
        }
      } else {
        NodePointer par = node->parent;
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
    TreapIteratorBase operator++(int) {
      TreapIteratorBase old = *this;
      ++(*this);
      return old;
    }

    /**
     * @brief 前置デクリメント
     * 更新後のイテレータの参照を返す
     * itr == end() のときは，最大要素を指すイテレータを返す
     */
    TreapIteratorBase &operator--() {
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
        NodePointer par = node->parent;
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
    TreapIteratorBase operator--(int) {
      TreapIteratorBase old = *this;
      --(*this);
      return old;
    }

    friend bool operator==(const TreapIteratorBase &a, const TreapIteratorBase &b) {
      return a.node == b.node && a.owner == b.owner;
    }
  };

  using TreapIterator = TreapIteratorBase<false>;
  using ConstTreapIterator = TreapIteratorBase<true>;

public:
  Treap() = default;

  /**
   * @brief コピーコンストラクタ
   */
  Treap(const Treap &tr) : comp(tr.comp) {
    for (const ValueType &val : tr) {
      insert(val);
    }
  }

  /**
   * @brief ムーブコンストラクタ
   * @note deque内の要素のメモリ上の配置は変化しないので，ポインタを渡しても大丈夫
   * @note ムーブ元のイテレータは無効
   */
  Treap(Treap &&tr)
      : n(tr.n), nodes(std::move(tr.nodes)), free_list(std::move(tr.free_list)), comp(std::move(tr.comp)),
        root(tr.root) {
    tr.n = 0;
    tr.root = nullptr;
    tr.free_list.clear();
  }

  /**
   * @brief コピー代入演算子
   */
  Treap &operator=(const Treap &tr) {
    if (this == &tr) {
      return *this;
    }
    comp = tr.comp;
    clear();
    for (const ValueType &val : tr) {
      insert(val);
    }
    return *this;
  }

  /**
   * @brief ムーブ代入演算子
   * @note 古いイテレータは，ムーブ元・ムーブ先どちらも無効
   */
  Treap &operator=(Treap &&tr) noexcept {
    if (this == &tr) {
      return *this;
    }

    nodes = std::move(tr.nodes);
    free_list = std::move(tr.free_list);
    comp = std::move(tr.comp);
    n = tr.n;
    root = tr.root;

    tr.n = 0;
    tr.root = nullptr;
    tr.free_list.clear();

    return *this;
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

  ConstTreapIterator kth_element(int k) const {
    Node *t = kth_node(k);
    return ConstTreapIterator(t, this);
  }

  int index(const ValueType &val) const {
    return contains(val) ? order_of_lower_bound(val) : -1;
  }

  template <bool IsConst> int index(const TreapIteratorBase<IsConst> &itr) const {
    assert(itr.owner == this);
    const Node *t = itr.node;
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

  ConstTreapIterator find(const ValueType &val) const {
    Node *t = find_node(val);
    return ConstTreapIterator(t ? t : &sentinel, this);
  }

  const ValueType &operator[](int k) const {
    return kth_node(k)->value;
  }

  /**
   * @brief ノードの挿入（インスタンスを渡したとき）
   */
  bool insert(const ValueType &val) {
    return insert_impl(val);
  }

  /**
   * @brief ノードの挿入（右辺値を渡したとき）
   */
  bool insert(ValueType &&val) {
    return insert_impl(std::move(val));
  }

  /**
   * @brief ノードの挿入（コンストラクタの引数を渡したとき）
   */
  template <class... Args> bool emplace(Args &&...args) {
    ValueType val(std::forward<Args>(args)...);
    return insert_impl(std::move(val));
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
   * @note 削除されたノードを指すイテレータは無効
   */
  TreapIterator erase(const TreapIterator &itr) {
    assert(itr.owner == this);
    assert(itr.node != &sentinel);
    TreapIterator res = itr;
    ++res;
    erase_node(itr.node);
    return res;
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
   * @brief 全要素の削除
   * @note 既存のイテレータはすべて無効
   */
  void clear() {
    for (auto itr = this->begin(); itr != this->end(); ++itr) {
      Node *t = itr.node;
      free_list.push_back(t);
    }
    root = nullptr;
    n = 0;
  }

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
   * @brief 最初の要素を指すconstイテレータを返す
   */
  ConstTreapIterator begin() const {
    const Node *b = min_node(root);
    return ConstTreapIterator(b ? b : &sentinel, this);
  }

  /**
   * @brief 最終要素の次を指すconstイテレータを返す
   */
  ConstTreapIterator end() const {
    return ConstTreapIterator(&sentinel, this);
  }

  ConstTreapIterator cbegin() const {
    return begin();
  }

  ConstTreapIterator cend() const {
    return end();
  }

  /**
   * @brief key以上の最初の要素を指すイテレータを返す（昇順のとき）
   */
  TreapIterator lower_bound(const ValueType &val) {
    Node *t = lower_bound_node(val);
    return TreapIterator(t ? t : &sentinel, this);
  }

  /**
   * @brief keyより大きい最初の要素を指すイテレータを返す（昇順のとき）
   */
  TreapIterator upper_bound(const ValueType &val) {
    Node *t = upper_bound_node(val);
    return TreapIterator(t ? t : &sentinel, this);
  }

  /**
   * @brief key以上の最初の要素を指すconstイテレータを返す（昇順のとき）
   */
  ConstTreapIterator lower_bound(const ValueType &val) const {
    Node *t = lower_bound_node(val);
    return ConstTreapIterator(t ? t : &sentinel, this);
  }

  /**
   * @brief keyより大きい最初の要素を指すconstイテレータを返す（昇順のとき）
   */
  ConstTreapIterator upper_bound(const ValueType &val) const {
    Node *t = upper_bound_node(val);
    return ConstTreapIterator(t ? t : &sentinel, this);
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
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<Treap<int>> st(N);
  rep(i, 0, N) {
    int c;
    cin >> c;
    st[i].insert(c);
  }
  rep(q, 0, Q) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    if (st[a].size() > st[b].size()) {
      swap(st[a], st[b]);
    }
    {
      auto itr = st[a].begin();
      while (itr != st[a].end()) {
        st[b].emplace(*itr);
        itr = st[a].erase(itr);
      }
    }
    cout << st[b].size() << endl;
  }
}