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
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x) { cout << (x ? "Yes" : "No") << endl; }
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
bool grid_check(int x, int y, int X, int Y) { return (0 <= x && x < X && 0 <= y && y < Y); }
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
 * @tparam T データ型
 * @tparam op 二項演算 T x T -> T
 * @tparam e op の単位元を返す関数
 *
 * @pre op(op(a,b),c) == op(a,op(b,c))
 * @pre op(a,e()) == op(e(),a) == a
 */
template <class T, auto op, auto e>
concept seg_concept = requires(T a, T b) {
  { op(a, b) } -> std::same_as<T>;
  { e() } -> std::same_as<T>;
};

/**
 * @brief セグメント木上の二分探索の際に渡す比較関数についての制約
 */
template <class T, auto comp>
concept seg_comp = requires(T a, T b) {
  { comp(a, b) } -> std::convertible_to<bool>;
};

/**
 * @brief 永続セグメント木
 * 過去のすべてのバージョンに対してアクセス・更新が可能 (fully persistent)
 * ポインタを利用したセグメント木をベースとする(インデックスで代用)
 *
 * @tparam T データ型
 * @tparam op 二項演算 T x T -> T
 * @tparam e op の単位元を返す関数
 *
 * @pre op(op(a,b),c) == op(a,op(b,c))
 * @pre op(a,e()) == op(e(),a) == a
 */
template <class T, auto op, auto e>
  requires seg_concept<T, op, e>
class PersistentSegmentTree {
  struct Node {
    T val;
    int left_child, right_child;
    Node() : val(e()), left_child(-1), right_child(-1) {}
    explicit Node(const T &val) : val(val), left_child(-1), right_child(-1) {}
    Node(const T &val, int left, int right) : val(val), left_child(left), right_child(right) {}
  };

  int n;                         ///< 元の配列の要素数
  int size;                      ///< 葉ノードの数
  std::vector<Node> all_nodes;   ///< すべてのノードを保持
  std::vector<int> version_root; ///< 各バージョンの根のインデックス

  /**
   * @brief 左右子ノードのインデックスをセットする
   */
  void set_children() {
    for (int i = 1; i < size; ++i) {
      all_nodes[i].left_child = i * 2;
      all_nodes[i].right_child = i * 2 + 1;
    }
  }

  /**
   * @brief 初期化時に，各ノードの積をセットする
   */
  void init_merge() {
    for (int i = size - 1; i >= 1; --i) {
      all_nodes[i].val = op(all_nodes[i * 2].val, all_nodes[i * 2 + 1].val);
    }
  }

  /**
   * @brief バージョンverの木におけるpos番目の葉ノードの値を取得
   */
  T get_impl(int pos, int index, int l, int r) const {
    if (l + 1 == r) {
      return all_nodes[index].val;
    }
    int m = (l + r) >> 1;
    if (pos < m) {
      return get_impl(pos, all_nodes[index].left_child, l, m);
    } else {
      return get_impl(pos, all_nodes[index].right_child, m, r);
    }
  }

  /**
   * @brief
   * バージョンverの木におけるpos番目の葉ノードの値をxに更新して新たなバージョンの木を作る
   * @return 新しいノードのインデックス
   */
  int set_impl(int pos, const T &x, int index, int l, int r) {
    if (l + 1 == r) {
      return make_new_node(x);
    }
    int m = (l + r) >> 1;
    if (pos < m) {
      int new_left_child_index = set_impl(pos, x, all_nodes[index].left_child, l, m);
      return make_new_node(new_left_child_index, all_nodes[index].right_child);
    } else {
      int new_right_child_index = set_impl(pos, x, all_nodes[index].right_child, m, r);
      return make_new_node(all_nodes[index].left_child, new_right_child_index);
    }
  }

  /**
   * @brief 新しいノードを作成
   * 葉ノード以外
   * @return 新しいノードのインデックス
   */
  int make_new_node(int l, int r) {
    T val = op(all_nodes[l].val, all_nodes[r].val);
    all_nodes.emplace_back(val, l, r);
    return static_cast<int>(all_nodes.size()) - 1;
  }

  /**
   * @brief 新しいノードを作成
   * 葉ノード
   * @return 新しいノードのインデックス
   */
  int make_new_node(const T &x) {
    all_nodes.emplace_back(x);
    return static_cast<int>(all_nodes.size()) - 1;
  }

  /**
   * @brief 区間クエリ
   * バージョンverの木における，半開区間[l, r)に対する積を取得
   */
  T prod_impl(int index, int l, int r, int left, int right) const {
    if (r <= left || right <= l) {
      return e();
    }
    if (l <= left && right <= r) {
      return all_nodes[index].val;
    }
    int m = (left + right) >> 1;
    return op(prod_impl(all_nodes[index].left_child, l, r, left, m),
              prod_impl(all_nodes[index].right_child, l, r, m, right));
  }

  /**
   * @brief 区間コピー
   * バージョンv1の木における半開区間[l,
   * r)をバージョンv2の木に移植して新しい木を作る
   * @return 新しく構築した部分木の根ノードのインデックス
   */
  int copy_impl(int index1, int index2, int l, int r, int left, int right) {
    if (r <= left || right <= l) {
      return index2;
    }
    if (l <= left && right <= r) {
      return index1;
    }
    int m = (left + right) >> 1;
    return make_new_node(
        copy_impl(all_nodes[index1].left_child, all_nodes[index2].left_child, l, r, left, m),
        copy_impl(all_nodes[index1].right_child, all_nodes[index2].right_child, l, r, m, right));
  }

  /**
   * @brief セグメント木上の二分探索
   * @return -1 境界が見つからなかったとき
   */
  template <auto comp>
  int l_fix_lower_bound_impl(int index, int l, int left, int right, T &product, const T &x) {
    if (right <= l) {
      return -1;
    }

    if (l <= left) {
      T next = op(product, all_nodes[index].val);
      if (comp(next, x)) {
        product = next;
        return -1;
      }
      if (left + 1 == right) {
        return right;
      }
    }

    int m = (left + right) >> 1;

    int res = l_fix_lower_bound_impl<comp>(all_nodes[index].left_child, l, left, m, product, x);
    if (res != -1) {
      return res;
    }

    return l_fix_lower_bound_impl<comp>(all_nodes[index].right_child, l, m, right, product, x);
  }

  /**
   * @brief セグメント木上の二分探索
   * @return -1 境界が見つからなかったとき
   */
  template <auto comp>
  int l_fix_upper_bound_impl(int index, int l, int left, int right, T &product, const T &x) {
    if (right <= l) {
      return -1;
    }

    if (l <= left) {
      T next = op(product, all_nodes[index].val);
      if (!comp(x, next)) {
        product = next;
        return -1;
      }
      if (left + 1 == right) {
        return right;
      }
    }

    int m = (left + right) >> 1;

    int res = l_fix_upper_bound_impl<comp>(all_nodes[index].left_child, l, left, m, product, x);
    if (res != -1) {
      return res;
    }

    return l_fix_upper_bound_impl<comp>(all_nodes[index].right_child, l, m, right, product, x);
  }

  /**
   * @brief セグメント木上の二分探索
   * @return size 境界が見つからなかったとき
   */
  template <auto comp>
  int r_fix_lower_bound_impl(int index, int r, int left, int right, T &product, const T &x) {
    if (r <= left) {
      return size;
    }

    if (right <= r) {
      T next = op(all_nodes[index].val, product);
      if (comp(x, next)) {
        product = next;
        return size;
      }
      if (left + 1 == right) {
        return left;
      }
    }

    int m = (left + right) >> 1;

    int res = r_fix_lower_bound_impl<comp>(all_nodes[index].right_child, r, m, right, product, x);
    if (res != size) {
      return res;
    }

    return r_fix_lower_bound_impl<comp>(all_nodes[index].left_child, r, left, m, product, x);
  }

  /**
   * @brief セグメント木上の二分探索
   * @return size 境界が見つからなかったとき
   */
  template <auto comp>
  int r_fix_upper_bound_impl(int index, int r, int left, int right, T &product, const T &x) {
    if (r <= left) {
      return size;
    }

    if (right <= r) {
      T next = op(all_nodes[index].val, product);
      if (!comp(next, x)) {
        product = next;
        return size;
      }
      if (left + 1 == right) {
        return left;
      }
    }

    int m = (left + right) >> 1;

    int res = r_fix_upper_bound_impl<comp>(all_nodes[index].right_child, r, m, right, product, x);
    if (res != size) {
      return res;
    }

    return r_fix_upper_bound_impl<comp>(all_nodes[index].left_child, r, left, m, product, x);
  }

public:
  explicit PersistentSegmentTree(int n_) : n(n_) {
    assert(n > 0);
    size = 1;
    while (size < n) {
      size <<= 1;
    }
    all_nodes.assign(size * 2, Node{});
    set_children();
    version_root.emplace_back(1);
  }

  explicit PersistentSegmentTree(const std::vector<T> &vec) : n(static_cast<int>(vec.size())) {
    assert(n > 0);
    size = 1;
    while (size < n) {
      size <<= 1;
    }
    all_nodes.assign(size * 2, Node{});
    for (int i = 0; i < n; ++i) {
      all_nodes[i + size] = Node(vec[i]);
    }
    set_children();
    init_merge();
    version_root.emplace_back(1);
  }

  /**
   * @brief バージョンverの木におけるpos番目の葉ノードの値を取得
   */
  T get(int ver, int pos) const {
    assert(0 <= ver && ver < static_cast<int>(version_root.size()));
    assert(0 <= pos && pos < n);
    return get_impl(pos, version_root[ver], 0, size);
  }

  /**
   * @brief
   * バージョンverの木におけるpos番目の葉ノードの値をxに更新して新たなバージョンの木を作る
   * @return 新しいバージョン
   */
  int set(int ver, int pos, const T &x) {
    assert(0 <= ver && ver < static_cast<int>(version_root.size()));
    assert(0 <= pos && pos < n);
    int new_root_index = set_impl(pos, x, version_root[ver], 0, size);
    version_root.emplace_back(new_root_index);
    return static_cast<int>(version_root.size()) - 1;
  }

  /**
   * @brief 区間積取得
   * バージョンverの木における，半開区間[l, r)に対する積を取得
   */
  T prod(int ver, int l, int r) const {
    assert(0 <= ver && ver < static_cast<int>(version_root.size()));
    assert(0 <= l && l <= r && r <= n);
    return prod_impl(version_root[ver], l, r, 0, size);
  }

  /**
   * @brief 区間コピー
   * バージョンv1の木における半開区間[l,
   * r)をバージョンv2の木に移植して新しい木を作る
   * @return 新しいバージョン
   */
  int copy(int ver1, int ver2, int l, int r) {
    assert(0 <= ver1 && ver1 < static_cast<int>(version_root.size()));
    assert(0 <= ver2 && ver2 < static_cast<int>(version_root.size()));
    assert(0 <= l && l <= r && r <= n);
    int new_root_index = copy_impl(version_root[ver1], version_root[ver2], l, r, 0, size);
    version_root.emplace_back(new_root_index);
    return static_cast<int>(version_root.size()) - 1;
  }

  /**
   * @brief
   * バージョンverの木について左端lを固定したときの，comp(op(l,l+1,...r-1), x)
   * == true && comp(op(l,l+1,...r), x) == false を満たすrを求める
   * @pre rについて単調増加ならcomp : op(l,...,r) < x
   */
  template <auto comp>
    requires seg_comp<T, comp>
  int l_fix_lower_bound(int ver, int l, const T &x) {
    assert(0 <= ver && ver < static_cast<int>(version_root.size()));
    assert(0 <= l && l <= n);
    T product = e();
    int res = l_fix_lower_bound_impl<comp>(version_root[ver], l, 0, size, product, x);
    return res == -1 ? n : std::min(res, n);
  }

  /**
   * @brief
   * バージョンverの木について左端lを固定したときの，comp(x, op(l,l+1,...r-1))
   * == false && comp(x, op(l,l+1,...r)) == true を満たすrを求める
   * @pre rについて単調増加ならcomp : x < op(l,...,r)
   */
  template <auto comp>
    requires seg_comp<T, comp>
  int l_fix_upper_bound(int ver, int l, const T &x) {
    assert(0 <= ver && ver < static_cast<int>(version_root.size()));
    assert(0 <= l && l <= n);
    T product = e();
    int res = l_fix_upper_bound_impl<comp>(version_root[ver], l, 0, size, product, x);
    return res == -1 ? n : std::min(res, n);
  }

  /**
   * @brief
   * バージョンverの木について右端rを固定したときの，comp(x, op(l+1,...r-1))
   * == true && comp(x, op(l,l+1,...r-1)) == false を満たすlを求める
   * @pre lについて単調減少ならcomp : x > op(l,...,r-1)
   */
  template <auto comp>
    requires seg_comp<T, comp>
  int r_fix_lower_bound(int ver, int r, const T &x) {
    assert(0 <= ver && ver < static_cast<int>(version_root.size()));
    assert(0 <= r && r <= n);
    T product = e();
    int res = r_fix_lower_bound_impl<comp>(version_root[ver], r, 0, size, product, x);
    return res == size ? -1 : res;
  }

  /**
   * @brief
   * バージョンverの木について右端rを固定したときの，comp(op(l+1,...r-1), x)
   * == false && comp(op(l,...r-1), x) == true を満たすlを求める
   * @pre lについて単調減少ならcomp : op(l,...,r-1) > x
   */
  template <auto comp>
    requires seg_comp<T, comp>
  int r_fix_upper_bound(int ver, int r, const T &x) {
    assert(0 <= ver && ver < static_cast<int>(version_root.size()));
    assert(0 <= r && r <= n);
    T product = e();
    int res = r_fix_upper_bound_impl<comp>(version_root[ver], r, 0, size, product, x);
    return res == size ? -1 : res;
  }
};

const int INF = 1e9;
int op(int a, int b) { return max(a, b); }
int e() { return -INF; }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<int> A(N);
  rep(i, 0, N) { cin >> A[i]; }
  vector<int> val(A);
  sort(all(val));
  val.erase(unique(all(val)), val.end());
  // v_cout(val);
  vector<int> comp(N);
  rep(i, 0, N) {
    comp[i] = lower_bound(all(val), A[i]) - val.begin();
    comp[i]++;
  }
  // v_cout(comp);

  int M = val.size();

  PersistentSegmentTree<int, op, e> seg(M + 1);
  int version = 0;
  seg.set(version++, 0, 0);

  vector<vector<int>> G(N);
  rep(i, 0, N - 1) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    G[u].emplace_back(v);
    G[v].emplace_back(u);
  }

  vector<int> ans(N);
  auto dfs = [&](auto f, int u, int p, int ver) -> void {
    ver = seg.set(ver, comp[u], seg.prod(ver, 0, comp[u]) + 1);
    ans[u] = seg.prod(ver, 0, M + 1);
    for (int v : G[u]) {
      if (v == p) {
        continue;
      }
      f(f, v, u, ver);
    }
  };

  dfs(dfs, 0, -1, version);

  rep(i, 0, N) { cout << ans[i] << endl; }
}