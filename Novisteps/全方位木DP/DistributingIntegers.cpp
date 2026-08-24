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
 * @brief 全方位木DPに必要な演算の制約
 * @note (T, op, e) はモノイドをなす必要がある
 *
 * @tparam T 乗せるデータの型
 * @tparam V 頂点重みの型
 * @tparam E 辺重みの型
 * @tparam op 結合律を満たす二項演算
 * @tparam e 二項演算の単位元を返す関数
 * @tparam apply_vertex 頂点重みを加える演算
 * @tparam apply_edge 辺重みを加える演算
 */
template <class T, class V, class E, auto op, auto e, auto apply_vertex, auto apply_edge>
concept rerooting_concept = requires(const T &a, const T &b, const V &vertex_weight, const E &edge_weight) {
  { op(a, b) } -> std::same_as<T>;
  { e() } -> std::same_as<T>;
  { apply_vertex(a, vertex_weight) } -> std::same_as<T>;
  { apply_edge(a, edge_weight) } -> std::same_as<T>;
};

template <class T, class V, class E, auto op, auto e, auto apply_vertex, auto apply_edge>
  requires rerooting_concept<T, V, E, op, e, apply_vertex, apply_edge>
class Rerooting {
  struct Edge {
    int to;
    E edge_weight;
  };

  T dfs1(int u = 0, int par = -1) {
    int s = G[u].size();
    dp[u].assign(s, e());
    T res = e();
    for (int i = 0; i < s; ++i) {
      const auto &[v, vu] = G[u][i];
      if (v == par) {
        continue;
      }
      dp[u][i] = apply_edge(dfs1(v, u), vu);
      res = op(res, dp[u][i]);
    }
    return apply_vertex(res, vertex_weight[u]);
  }

  void dfs2(int u = 0, int par = -1, const T &dp_par = e()) {
    int s = G[u].size();
    for (int i = 0; i < s; ++i) {
      const auto &[v, vu] = G[u][i];
      if (v == par) {
        dp[u][i] = apply_edge(dp_par, vu);
      }
    }

    std::vector<T> dp_left(s + 1, e()), dp_right(s + 1, e());
    for (int i = 0; i < s; ++i) {
      dp_left[i + 1] = op(dp_left[i], dp[u][i]);
    }
    for (int i = s - 1; i >= 0; --i) {
      dp_right[i] = op(dp[u][i], dp_right[i + 1]);
    }
    ans[u] = apply_vertex(dp_left[s], vertex_weight[u]);

    for (int i = 0; i < s; ++i) {
      const auto &[v, _] = G[u][i];
      if (v == par) {
        continue;
      }
      dfs2(v, u, apply_vertex(op(dp_left[i], dp_right[i + 1]), vertex_weight[u]));
    }
  }

public:
  int n;
  std::vector<V> vertex_weight;
  std::vector<std::vector<Edge>> G;
  std::vector<std::vector<T>> dp;
  std::vector<T> ans;

  Rerooting(int n_)
      : n(n_), vertex_weight(std::vector<V>(n_)), G(std::vector<std::vector<Edge>>(n_)),
        dp(std::vector<std::vector<T>>(n_)), ans(std::vector<T>(n_)) {}
  Rerooting(const std::vector<V> &vec_)
      : n(vec_.size()), vertex_weight(vec_), G(std::vector<std::vector<Edge>>(vec_.size())),
        dp(std::vector<std::vector<T>>(vec_.size())), ans(std::vector<T>(vec_.size())) {}

  void set_vertex_weight(const std::vector<V> &vec_) {
    assert(n == vec_.size());
    vertex_weight = vec_;
  }
  void set_vertex_weight(int pos, const V &v) {
    assert(0 <= pos && pos < n);
    vertex_weight[pos] = v;
  }
  // 内部的には逆辺の重みをもつ
  void add_edge(int u, int v, const E &uv, const E &vu) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    G[u].emplace_back(v, vu);
    G[v].emplace_back(u, uv);
  }

  void build() {
    dfs1();
    dfs2();
  }

  const std::vector<T> &get_answer() const {
    return ans;
  }

  const T &operator[](int pos) const {
    assert(0 <= pos && pos < n);
    return ans[pos];
  }
};

template <long long MOD = 998244353> class Modint {
  static_assert(MOD > 0);
  long long value;

  /**
   * @brief value*x + MOD*y = 1 の解 (x, y) のうち，xを返す
   */
  long long inv_impl() const {
    long long x = 1, u = 0, s = value, t = MOD;
    long long k;
    while (t) {
      k = s / t;
      s -= k * t;
      std::swap(s, t);
      x -= k * u;
      std::swap(x, u);
    }
    assert(s == 1);
    x %= MOD;
    if (x < 0) {
      x += MOD;
    }
    return x;
  }

  long long pow_impl(long long x) const {
    long long res = 1;
    long long a;
    if (x >= 0) {
      a = value;
    } else {
      a = inv_impl();
      x *= -1;
    }
    while (x) {
      if (x & 1) {
        res *= a;
        res %= MOD;
      }
      a *= a;
      a %= MOD;
      x >>= 1;
    }
    return res;
  }

public:
  Modint() : value(0) {}

  Modint(long long x) {
    value = x % MOD;
    if (value < 0) {
      value += MOD;
    }
  }

  Modint &operator+=(const Modint &rhs) {
    this->value += rhs.value;
    if (this->value >= MOD) {
      this->value -= MOD;
    }
    return *this;
  }

  Modint operator+(const Modint &rhs) const {
    Modint res = *this;
    res += rhs;
    return res;
  }

  Modint &operator-=(const Modint &rhs) {
    this->value -= rhs.value;
    if (this->value < 0) {
      this->value += MOD;
    }
    return *this;
  }

  Modint operator-(const Modint &rhs) const {
    Modint res = *this;
    res -= rhs;
    return res;
  }

  Modint operator-() const {
    return Modint{MOD - this->value};
  }

  Modint &operator*=(const Modint &rhs) {
    this->value *= rhs.value;
    this->value %= MOD;
    return *this;
  }

  Modint operator*(const Modint &rhs) const {
    Modint res = *this;
    res *= rhs;
    return res;
  }

  Modint &operator/=(const Modint &rhs) {
    this->value *= rhs.inv_impl();
    this->value %= MOD;
    return *this;
  }

  Modint operator/(const Modint &rhs) const {
    Modint res = *this;
    res /= rhs;
    return res;
  }

  long long val() const {
    return value;
  }

  Modint inv() const {
    return Modint{inv_impl()};
  }

  Modint pow(long long x) const {
    return Modint{pow_impl(x)};
  }
};

/**
 * @brief modulo二項係数を計算するクラス（素数mod）
 * https://drken1215.hatenablog.com/entry/2018/06/08/210000
 * 制約 1 <= n <= 1e7，pは素数
 * 階乗の前計算 O(n)
 * 二項係数の計算 O(1)
 */
template <long long MOD = 998244353> class ModCombination {
  using Mint = Modint<MOD>;
  int n;
  std::vector<Mint> inv;
  std::vector<Mint> factorial;
  std::vector<Mint> factorial_inv;

public:
  ModCombination() {}

  ModCombination(int n_)
      : n(n_), inv(std::vector<Mint>(n_ + 1)), factorial(std::vector<Mint>(n_ + 1)),
        factorial_inv(std::vector<Mint>(n_ + 1)) {
    // a^-1 = -(p % a)^-1 * (p / a) を利用
    inv[1] = 1;
    for (int i = 2; i <= n; ++i) {
      inv[i] = -inv[MOD % i] * (MOD / i);
    }

    factorial[0] = 1;
    for (int i = 1; i <= n; ++i) {
      factorial[i] = factorial[i - 1] * i;
    }

    factorial_inv[0] = 1;
    for (int i = 1; i <= n; ++i) {
      factorial_inv[i] = factorial_inv[i - 1] * inv[i];
    }
  }

  Mint Com(int k, int r) {
    assert(0 <= k && k <= n && 0 <= r && r <= n);
    if (k < r) {
      return 0;
    }
    return factorial[k] * factorial_inv[r] * factorial_inv[k - r];
  }

  Mint Inv(int x) {
    assert(0 < x && x <= n);
    return inv[x];
  }
};

const ll MOD = 1e9 + 7;
ModCombination<MOD> comb;
using Mint = Modint<MOD>;

struct T {
  Mint val;
  Mint siz;
};

T op(const T &a, const T &b) {
  return T{comb.Com((a.siz + b.siz).val(), a.siz.val()) * a.val * b.val, a.siz + b.siz};
}

T e() {
  return T{1, 0};
}

T apply_vertex(const T &a, const Mint &x) {
  return T{a.val, a.siz + x};
}

T apply_edge(const T &a, const Mint &) {
  return a;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  comb = ModCombination<MOD>{N};
  Rerooting<T, Mint, Mint, op, e, apply_vertex, apply_edge> rerooting(N);
  rep(i, 0, N) {
    rerooting.set_vertex_weight(i, Mint{1});
  }
  rep(i, 0, N - 1) {
    int a, b;
    cin >> a >> b;
    a--, b--;
    rerooting.add_edge(a, b, Mint{0}, Mint{0});
  }
  rerooting.build();

  rep(i, 0, N) {
    cout << rerooting[i].val.val() << endl;
  }
}