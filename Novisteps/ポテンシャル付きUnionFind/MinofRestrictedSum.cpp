#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

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

template <class T, auto op, auto inv, auto e, auto eq>
concept puf_group = requires(T a, T b) {
  { op(a, b) } -> std::same_as<T>;
  { inv(a) } -> std::same_as<T>;
  { e() } -> std::same_as<T>;
  { eq(a, b) } -> std::same_as<bool>;
};

/**
 * @brief 群をポテンシャルとして持つポテンシャル付きUnionFind
 * @tparam T ポテンシャルの型
 * @tparam op ポテンシャル同士の演算
 * @tparam inv 逆元を返す関数
 * @tparam e 単位元を返す関数
 * @tparam eq 等価判定用関数
 */
template <class T, auto op, auto inv, auto e, auto eq>
  requires puf_group<T, op, inv, e, eq>
class PotentializedUnionFind {
  int n_;
  int group_num_;
  std::vector<int> parent_;
  std::vector<T> potential_; ///< 各頂点から親へのポテンシャル

public:
  PotentializedUnionFind(int n)
      : n_(n), group_num_(n), parent_(std::vector<int>(n, -1)),
        potential_(std::vector<T>(n, e())) {}

  int groups() {
    return group_num_;
  }

  int size(int x) {
    return -parent_[root(x)];
  }

  int root(int x) {
    if (parent_[x] < 0) {
      return x;
    }
    int r = root(parent_[x]);
    potential_[x] = op(potential_[x], potential_[parent_[x]]);
    return parent_[x] = r;
  }

  /**
   * @brief 頂点xから代表元へのポテンシャルを返す
   */
  T get_potential(int x) {
    root(x);
    return potential_[x];
  }

  /**
   * @brief x -> y へのポテンシャル
   * @pre is_same(x, y) == true
   */
  T diff(int x, int y) {
    return op(get_potential(x), inv(get_potential(y)));
  }

  bool is_same(int x, int y) {
    return root(x) == root(y);
  }

  /**
   * @brief 頂点xから頂点yに向かって重さwの辺を張る
   * @return true: マージ成功，false: 矛盾発生
   */
  bool merge(int x, int y, T w) {
    if (is_same(x, y)) {
      if (eq(diff(x, y), w)) {
        return true;
      } else {
        return false;
      }
    } else {
      T px = get_potential(x);
      T py = get_potential(y);
      w = op(inv(px), op(w, py));
      x = root(x), y = root(y);
      if (size(x) > size(y)) {
        std::swap(x, y);
        w = inv(w);
      }
      parent_[y] += parent_[x];
      parent_[x] = y;
      potential_[x] = w;
      group_num_--;
      return true;
    }
  }
};

ll op(ll a, ll b) {
  return a ^ b;
}
ll inv(ll a) {
  return a;
}
ll e() {
  return 0LL;
}
bool eq(ll a, ll b) {
  return a == b;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  PotentializedUnionFind<ll, op, inv, e, eq> puf(N);
  bool ok = true;
  rep(i, 0, M) {
    int x, y;
    ll z;
    cin >> x >> y >> z;
    x--;
    y--;
    ok &= puf.merge(x, y, z);
  }
  if (!ok) {
    cout << -1 << endl;
  } else {
    vector<ll> ans(N, 0);
    rrep(d, 0, 60) {
      vector<ll> cnt1(N, 0);
      rep(i, 0, N) {
        // rootから0を流す
        ll c = puf.diff(puf.root(i), i);
        cnt1[puf.root(i)] += ((c >> d) & 1);
      }
      rep(i, 0, N) {
        if (puf.root(i) == i) {
          // 1が多ければ，1を流した方が良かった
          // 0が多ければ，0を流して正解
          ans[i] += (cnt1[i] < puf.size(i) - cnt1[i] ? 0 : 1) * (1LL << d);
        }
      }
    }
    rep(i, 0, N) {
      cout << op(puf.diff(puf.root(i), i), ans[puf.root(i)]) << ' ';
    }
    cout << endl;
  }
}