/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

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

struct Point {
  int x, y;
  bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }
  bool operator<(const Point &rhs) const {
    return x == rhs.x ? y < rhs.y : x < rhs.x;
  }
};

struct Problem {
  int N = 5000;
  double SABA = 1, IWASHI = 1.1;
  array<Point, 5000> saba, iwashi;
  vector<vector<double>> saba_in_area, iwashi_in_area;
  vector<vector<double>> saba_on_vertical, iwashi_on_vertical;
  vector<vector<double>> saba_on_horizontal, iwashi_on_horizontal;
  vector<vector<double>> saba_at_corner, iwashi_at_corner;
  int MAX_VERTEX = 1000;
  int MAX_SUM_LENGTH = 400000;
  int MAX_LENGTH = 100000;
  Problem()
      : saba_in_area(250, vector<double>(250, 0)), iwashi_in_area(250, vector<double>(250, 0)),
        saba_on_vertical(250, vector<double>(251, 0)), iwashi_on_vertical(250, vector<double>(251, 0)),
        saba_on_horizontal(251, vector<double>(250, 0)), iwashi_on_horizontal(251, vector<double>(250, 0)),
        saba_at_corner(251, vector<double>(251, 0)), iwashi_at_corner(251, vector<double>(251, 0)) {
    input();
  }
  void input() {
    cin >> N;
    for (Point &s : saba) {
      cin >> s.x >> s.y;
    }
    for (Point &i : iwashi) {
      cin >> i.x >> i.y;
    }
    for (Point &s : saba) {
      if (s.y % 400 == 0) {
        if (s.x % 400 == 0) {
          saba_at_corner[s.y / 400][s.x / 400] += SABA;
        } else {
          saba_on_horizontal[s.y / 400][s.x / 400] += SABA;
        }
      } else {
        if (s.x % 400 == 0) {
          saba_on_vertical[s.y / 400][s.x / 400] += SABA;
        } else {
          saba_in_area[s.y / 400][s.x / 400] += SABA;
        }
      }
    }
    for (Point &s : iwashi) {
      if (s.y % 400 == 0) {
        if (s.x % 400 == 0) {
          iwashi_at_corner[s.y / 400][s.x / 400] += IWASHI;
        } else {
          iwashi_on_horizontal[s.y / 400][s.x / 400] += IWASHI;
        }
      } else {
        if (s.x % 400 == 0) {
          iwashi_on_vertical[s.y / 400][s.x / 400] += IWASHI;
        } else {
          iwashi_in_area[s.y / 400][s.x / 400] += IWASHI;
        }
      }
    }
  }
};

struct Answer {
  double score;
  int m;
  vector<Point> v;
  void print() {
    cout << m << endl;
    for (Point &p : v) {
      cout << p.x << ' ' << p.y << endl;
    }
  }
};

struct State {
  double score = 0;
  int vertex = 0;
  int len = 0;
  array<array<bool, 250>, 250> area{false};
  Treap<Point, std::less<Point>, false> tp;
  array<array<int, 250>, 250> border_cnt{0};
};

struct Solver {
  Problem P = Problem{};
  mt19937 mt = mt19937{0};
  array<int, 4> dx = {1, 0, -1, 0}, dy = {0, 1, 0, -1};
  array<int, 8> ddx = {1, 1, 0, -1, -1, -1, 0, 1}, ddy = {0, 1, 1, 1, 0, -1, -1, -1};

  void init_state(State &state) {
    double best_score = -10000;
    Point best_area = {-1, -1};
    int x, y;
    double score;
    for (int i = 0; i < 100; ++i) {
      score = 0;
      x = mt() % 250;
      y = mt() % 250;
      score += P.saba_in_area[y][x] + P.saba_on_horizontal[y][x] + P.saba_on_horizontal[y + 1][x] +
               P.saba_on_vertical[y][x] + P.saba_on_vertical[y][x + 1] + P.saba_at_corner[y][x] +
               P.saba_at_corner[y + 1][x] + P.saba_at_corner[y][x + 1] + P.saba_at_corner[y + 1][x + 1];
      score -= P.iwashi_in_area[y][x] + P.iwashi_on_horizontal[y][x] + P.iwashi_on_horizontal[y + 1][x] +
               P.iwashi_on_vertical[y][x] + P.iwashi_on_vertical[y][x + 1] + P.iwashi_at_corner[y][x] +
               P.iwashi_at_corner[y + 1][x] + P.iwashi_at_corner[y][x + 1] + P.iwashi_at_corner[y + 1][x + 1];
      if (score >= best_score) {
        best_score = score;
        best_area = {x, y};
      }
    }
    state.score = best_score;
    for (int d = 0; d < 4; ++d) {
      int nx = best_area.x + dx[d];
      int ny = best_area.y + dy[d];
      if (!(0 <= nx && nx < 250 && 0 <= ny && ny < 250)) {
        continue;
      }
      state.border_cnt[best_area.y][best_area.x]++;
    }
    if (state.border_cnt[best_area.y][best_area.x] > 0) {
      state.tp.insert(best_area);
    }
    state.vertex = 4;
    state.len = 4 * 400;
    state.area[best_area.y][best_area.x] = true;
  }

  void next_state(State &state, int turn, int max_turn) {
    double progress = (double)(turn) / max_turn;
    int r = mt() % state.tp.size();
    Point p = state.tp[r];
    vector<int> dd;
    for (int d = 0; d < 4; ++d) {
      int nx = p.x + dx[d];
      int ny = p.y + dy[d];
      if (!(0 <= nx && nx < 250 && 0 <= ny && ny < 250)) {
        continue;
      }
      if (state.area[ny][nx]) {
        continue;
      }
      dd.push_back(d);
    }
    if (dd.empty()) {
      return;
    }
    int dr = mt() % dd.size();
    int dir = dd[dr];
    Point np = {p.x + dx[dir], p.y + dy[dir]};
    bool d0 = false;
    int cnt = 0;
    for (int d = 0; d < 4; ++d) {
      if (d == 2) {
        continue;
      }
      int nx = np.x + dx[(dir + d) % 4];
      int ny = np.y + dy[(dir + d) % 4];
      if (!(0 <= nx && nx < 250 && 0 <= ny && ny < 250)) {
        continue;
      }
      if (d == 0) {
        d0 = state.area[ny][nx];
      } else {
        cnt += state.area[ny][nx];
      }
    }
    if (d0 && cnt == 0) {
      return;
    }
    bool ng = false;
    for (int d = 1; d < 8; d += 2) {
      if (!(0 <= np.x + ddx[d] && np.x + ddx[d] < 250 && 0 <= np.y + ddy[d] && np.y + ddy[d] < 250)) {
        continue;
      }
      ng |=
          (state.area[np.y + ddy[d]][np.x + ddx[d]] && !state.area[np.y + ddy[(d + 7) % 8]][np.x + ddx[(d + 7) % 8]] &&
           !state.area[np.y + ddy[(d + 1) % 8]][np.x + ddx[(d + 1) % 8]]);
    }
    if (ng) {
      return;
    }
    double diff = P.saba_in_area[np.y][np.x] - P.iwashi_in_area[np.y][np.x];
    int corner_cnt = 0;
    {
      int ul = 0, ur = 0, dl = 0, dr = 0;
      if (np.y == 0 || !state.area[np.y - 1][np.x]) {
        diff += P.saba_on_horizontal[np.y][np.x] - P.iwashi_on_horizontal[np.y][np.x];
        ul++, ur++;
      }
      if (np.y == 249 || !state.area[np.y + 1][np.x]) {
        diff += P.saba_on_horizontal[np.y + 1][np.x] - P.iwashi_on_horizontal[np.y + 1][np.x];
        dl++, dr++;
      }
      if (np.x == 0 || !state.area[np.y][np.x - 1]) {
        diff += P.saba_on_vertical[np.y][np.x] - P.iwashi_on_vertical[np.y][np.x];
        ul++, dl++;
      }
      if (np.x == 249 || !state.area[np.y][np.x + 1]) {
        diff += P.saba_on_vertical[np.y][np.x + 1] - P.iwashi_on_vertical[np.y][np.x + 1];
        ur++, dr++;
      }
      if (ul == 2) {
        diff += P.saba_at_corner[np.y][np.x] - P.iwashi_at_corner[np.y][np.x];
        corner_cnt++;
      }
      if (ur == 2) {
        diff += P.saba_at_corner[np.y][np.x + 1] - P.iwashi_at_corner[np.y][np.x + 1];
        corner_cnt++;
      }
      if (dl == 2) {
        diff += P.saba_at_corner[np.y + 1][np.x] - P.iwashi_at_corner[np.y + 1][np.x];
        corner_cnt++;
      }
      if (dr == 2) {
        diff += P.saba_at_corner[np.y + 1][np.x + 1] - P.iwashi_at_corner[np.y + 1][np.x + 1];
        corner_cnt++;
      }
    }
    if (diff < 0) {
      int prob = mt() % (int)(abs(diff) * abs(diff) + 1);
      if (prob * pow(progress, 0.6) > 0.14) {
        return;
      }
    } else if (diff == 0) {
      int prob = mt() % 7;
      if (prob > 3) {
        return;
      }
    }
    int diff_len = 0;
    {
      if (corner_cnt == 0) {
        diff_len = -2 * 400;
      } else if (corner_cnt == 1) {
        diff_len = 0;
      } else if (corner_cnt == 2) {
        diff_len = 2 * 400;
      } else {
        return;
      }
    }
    if (state.len + diff_len > 400000) {
      return;
    }
    array<int, 4> vertex_cnt{0};
    for (int d = 0; d < 8; d += 2) {
      if (!(0 <= np.x + ddx[d] && np.x + ddx[d] < 250 && 0 <= np.y + ddy[d] && np.y + ddy[d] < 250)) {
        continue;
      } else if (!state.area[np.y + ddy[d]][np.x + ddx[d]]) {
        continue;
      }
      if (0 <= np.x + ddx[(d + 7) % 8] && np.x + ddx[(d + 7) % 8] < 250 && 0 <= np.y + ddy[(d + 7) % 8] &&
          np.y + ddy[(d + 7) % 8] < 250 && state.area[np.y + ddy[(d + 7) % 8]][np.x + ddx[(d + 7) % 8]]) {
        vertex_cnt[d / 2]++;
      }
      if (0 <= np.x + ddx[(d + 1) % 8] && np.x + ddx[(d + 1) % 8] < 250 && 0 <= np.y + ddy[(d + 1) % 8] &&
          np.y + ddy[(d + 1) % 8] < 250 && state.area[np.y + ddy[(d + 1) % 8]][np.x + ddx[(d + 1) % 8]]) {
        vertex_cnt[(d / 2 + 1) % 4]++;
      }
    }
    array<int, 3> dist{0};
    for (int v : vertex_cnt) {
      dist[v]++;
    }
    int diff_vertex = 0;
    {
      if (dist[0] == 4) {
        diff_vertex = 0;
      } else if (dist[0] == 3) {
        if (dist[1] == 1) {
          diff_vertex = 1;
        } else {
          diff_vertex = -2;
        }
      } else if (dist[0] == 2) {
        if (dist[1] == 2) {
          diff_vertex = 4;
        } else if (dist[1] == 1) {
          diff_vertex = 0;
        } else {
          diff_vertex = -4;
        }
      } else if (dist[0] == 1) {
        if (dist[1] == 2) {
          diff_vertex = 2;
        } else {
          diff_vertex = -2;
        }
      } else {
        diff_vertex = 0;
      }
    }
    if (state.vertex + diff_vertex > 1000) {
      return;
    }
    state.score += diff;
    state.len += diff_len;
    state.vertex += diff_vertex;
    state.area[np.y][np.x] = true;
    for (int d = 0; d < 4; ++d) {
      int nx = np.x + dx[d];
      int ny = np.y + dy[d];
      if (!(0 <= nx && nx < 250 && 0 <= ny && ny < 250)) {
        continue;
      }
      if (state.area[ny][nx]) {
        state.border_cnt[ny][nx]--;
        if (state.border_cnt[ny][nx] == 0) {
          state.tp.erase(Point{nx, ny});
        }
      } else {
        state.border_cnt[np.y][np.x]++;
      }
    }
    if (state.border_cnt[np.y][np.x] > 0) {
      state.tp.insert(np);
    }
  }

  void next_state2(State &state, int turn, int max_turn) {
    double progress = double(turn) / max_turn;
    double prob = double(mt()) / mt.max();
    if (prob >= pow(progress, 0.5)) {
      return;
    }
    if (state.tp.size() <= 1) {
      return;
    }
    int r = mt() % state.tp.size();
    Point p = state.tp[r];
    array<int, 2> d0{0};
    for (int d = 0; d < 4; ++d) {
      int nx = p.x + dx[d];
      int ny = p.y + dy[d];
      if (!(0 <= nx && nx < 250 && 0 <= ny && ny < 250)) {
        continue;
      }
      if (state.area[ny][nx]) {
        d0[d & 1]++;
      }
    }
    if (d0[0] * d0[1] == 0 && d0[0] + d0[1] == 2) {
      return;
    }
    bool ng = false;
    for (int d = 1; d < 8; d += 2) {
      if (!(0 <= p.x + ddx[d] && p.x + ddx[d] < 250 && 0 <= p.y + ddy[d] && p.y + ddy[d] < 250)) {
        continue;
      }
      ng |= (!state.area[p.y + ddy[d]][p.x + ddx[d]] && state.area[p.y + ddy[(d + 7) % 8]][p.x + ddx[(d + 7) % 8]] &&
             state.area[p.y + ddy[(d + 1) % 8]][p.x + ddx[(d + 1) % 8]]);
    }
    if (ng) {
      return;
    }
    double diff = -P.saba_in_area[p.y][p.x] + P.iwashi_in_area[p.y][p.x];
    int corner_cnt = 0;
    {
      int ul = 0, ur = 0, dl = 0, dr = 0;
      if (p.y == 0 || !state.area[p.y - 1][p.x]) {
        diff += -P.saba_on_horizontal[p.y][p.x] + P.iwashi_on_horizontal[p.y][p.x];
        ul++, ur++;
      }
      if (p.y == 249 || !state.area[p.y + 1][p.x]) {
        diff += -P.saba_on_horizontal[p.y + 1][p.x] + P.iwashi_on_horizontal[p.y + 1][p.x];
        dl++, dr++;
      }
      if (p.x == 0 || !state.area[p.y][p.x - 1]) {
        diff += -P.saba_on_vertical[p.y][p.x] + P.iwashi_on_vertical[p.y][p.x];
        ul++, dl++;
      }
      if (p.x == 249 || !state.area[p.y][p.x + 1]) {
        diff += -P.saba_on_vertical[p.y][p.x + 1] + P.iwashi_on_vertical[p.y][p.x + 1];
        ur++, dr++;
      }
      if (ul == 2) {
        diff += -P.saba_at_corner[p.y][p.x] + P.iwashi_at_corner[p.y][p.x];
        corner_cnt++;
      }
      if (ur == 2) {
        diff += -P.saba_at_corner[p.y][p.x + 1] + P.iwashi_at_corner[p.y][p.x + 1];
        corner_cnt++;
      }
      if (dl == 2) {
        diff += -P.saba_at_corner[p.y + 1][p.x] + P.iwashi_at_corner[p.y + 1][p.x];
        corner_cnt++;
      }
      if (dr == 2) {
        diff += -P.saba_at_corner[p.y + 1][p.x + 1] + P.iwashi_at_corner[p.y + 1][p.x + 1];
        corner_cnt++;
      }
    }
    if (diff < 0) {
      prob = mt() % (int)(abs(diff) * abs(diff) + 1);
      if (prob * pow(progress, 0.7) > 0.09) {
        return;
      }
    } else if (diff == 0) {
      if (corner_cnt == 1) {
        prob = mt() % 7;
        if (prob > 4) {
          return;
        }
      } else if (corner_cnt == 2) {
        prob = mt() % 6;
        if (prob > 3) {
          return;
        }
      } else {
        prob = mt() % 6;
        if (prob > 2) {
          return;
        }
      }
    }
    int diff_len = 0;
    {
      if (corner_cnt == 0) {
        diff_len = 2 * 400;
      } else if (corner_cnt == 1) {
        diff_len = 0;
      } else if (corner_cnt == 2) {
        diff_len = -2 * 400;
      } else {
        return;
      }
    }
    if (state.len + diff_len > 400000) {
      return;
    }
    array<int, 4> vertex_cnt{0};
    for (int d = 0; d < 8; d += 2) {
      if (!(0 <= p.x + ddx[d] && p.x + ddx[d] < 250 && 0 <= p.y + ddy[d] && p.y + ddy[d] < 250)) {
        continue;
      } else if (!state.area[p.y + ddy[d]][p.x + ddx[d]]) {
        continue;
      }
      if (0 <= p.x + ddx[(d + 7) % 8] && p.x + ddx[(d + 7) % 8] < 250 && 0 <= p.y + ddy[(d + 7) % 8] &&
          p.y + ddy[(d + 7) % 8] < 250 && state.area[p.y + ddy[(d + 7) % 8]][p.x + ddx[(d + 7) % 8]]) {
        vertex_cnt[d / 2]++;
      }
      if (0 <= p.x + ddx[(d + 1) % 8] && p.x + ddx[(d + 1) % 8] < 250 && 0 <= p.y + ddy[(d + 1) % 8] &&
          p.y + ddy[(d + 1) % 8] < 250 && state.area[p.y + ddy[(d + 1) % 8]][p.x + ddx[(d + 1) % 8]]) {
        vertex_cnt[(d / 2 + 1) % 4]++;
      }
    }
    array<int, 3> dist{0};
    for (int v : vertex_cnt) {
      dist[v]++;
    }
    int diff_vertex = 0;
    {
      if (dist[0] == 4) {
        diff_vertex = 0;
      } else if (dist[0] == 3) {
        if (dist[1] == 1) {
          diff_vertex = -1;
        } else {
          diff_vertex = 2;
        }
      } else if (dist[0] == 2) {
        if (dist[1] == 2) {
          diff_vertex = -4;
        } else if (dist[1] == 1) {
          diff_vertex = 0;
        } else {
          diff_vertex = 4;
        }
      } else if (dist[0] == 1) {
        if (dist[1] == 2) {
          diff_vertex = -2;
        } else {
          diff_vertex = 2;
        }
      } else {
        diff_vertex = 0;
      }
    }
    if (state.vertex + diff_vertex > 1000) {
      return;
    }
    state.score += diff;
    state.len += diff_len;
    state.vertex += diff_vertex;
    state.area[p.y][p.x] = false;
    for (int d = 0; d < 4; ++d) {
      int nx = p.x + dx[d];
      int ny = p.y + dy[d];
      if (!(0 <= nx && nx < 250 && 0 <= ny && ny < 250)) {
        continue;
      }
      if (state.area[ny][nx]) {
        state.border_cnt[ny][nx]++;
        if (state.border_cnt[ny][nx] > 0) {
          state.tp.insert(Point{nx, ny});
        }
      } else {
        state.border_cnt[p.y][p.x]--;
      }
    }
    if (state.border_cnt[p.y][p.x] == 0) {
      state.tp.erase(p);
    }
  }

  Answer encode_state(State &state) {
    Answer ans;
    ans.score = state.score;
    Point start = {-1, -1};
    for (int y = 0; y < 250; ++y) {
      for (int x = 0; x < 250; ++x) {
        if (state.area[y][x]) {
          start = {x, y};
          break;
        }
      }
      if (start != Point{-1, -1}) {
        break;
      }
    }
    ans.v.emplace_back(start.x * 400, start.y * 400);
    Point current = start;
    int dir = 0;

    while (true) {
      dir = (dir + 3) % 4;
      while (true) {
        bool ok = true;
        if (dir == 0) {
          if (current.x == 250) {
            ok = false;
          } else {
            ok &= (current.y < 250 && state.area[current.y][current.x]);
            ok &= (current.y == 0 || !state.area[current.y - 1][current.x]);
          }
        } else if (dir == 1) {
          if (current.y == 250) {
            ok = false;
          } else {
            ok &= (current.x > 0 && state.area[current.y][current.x - 1]);
            ok &= (current.x == 250 || !state.area[current.y][current.x]);
          }
        } else if (dir == 2) {
          if (current.x == 0) {
            ok = false;
          } else {
            ok &= (current.y > 0 && state.area[current.y - 1][current.x - 1]);
            ok &= (current.y == 250 || !state.area[current.y][current.x - 1]);
          }
        } else {
          if (current.y == 0) {
            ok = false;
          } else {
            ok &= (current.x < 250 && state.area[current.y - 1][current.x]);
            ok &= (current.x == 0 || !state.area[current.y - 1][current.x - 1]);
          }
        }
        if (ok) {
          break;
        } else {
          dir = (dir + 1) % 4;
        }
      }
      Point next_point = current;
      next_point.x = current.x + dx[dir];
      next_point.y = current.y + dy[dir];
      if (ans.v.size() >= 2) {
        bool ok = false;
        ok |=
            (ans.v[ans.v.size() - 2].x == next_point.x * 400 && ans.v[ans.v.size() - 2].x == ans.v[ans.v.size() - 1].x);
        ok |=
            (ans.v[ans.v.size() - 2].y == next_point.y * 400 && ans.v[ans.v.size() - 2].y == ans.v[ans.v.size() - 1].y);
        if (ok) {
          ans.v.pop_back();
        }
      }
      if (next_point == start) {
        break;
      }
      ans.v.emplace_back(next_point.x * 400, next_point.y * 400);
      swap(current, next_point);
    }
    ans.m = ans.v.size();
    return ans;
  }

  Answer Annealing() {
    State state;
    int T = 200000;
    init_state(state);
    for (int i = 0; i < T; ++i) {
      next_state(state, i, T);
      next_state2(state, i, T);
    }
    return encode_state(state);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver sol;
  Answer best_ans;
  best_ans.score = -10000;
  for (int t = 0; t < 16; ++t) {
    Answer ans = sol.Annealing();
    // ans.print();
    if (best_ans.score <= ans.score) {
      swap(best_ans, ans);
    }
  }
  best_ans.print();
}