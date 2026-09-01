/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

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

template <class T, auto op, auto inv, auto e>
concept BIT_concept = requires(const T &a, const T &b) {
  { op(a, b) } -> std::same_as<T>;
  { inv(a) } -> std::same_as<T>;
  { e() } -> std::same_as<T>;
};

/**
 * @brief 可換群上の fenwick_tree
 */
template <class T, auto op, auto inv, auto e>
  requires BIT_concept<T, op, inv, e>
class BIT {
  int n;
  std::vector<T> data;

public:
  BIT(int n_) : n(n_), data(std::vector<T>(n_ + 1, e())) {}
  BIT(int n_, const T &x) : n(n_), data(std::vector<T>(n_ + 1, e())) {
    for (int i = 1; i <= n; ++i) {
      data[i] = op(data[i], x);
      int j = i + (i & -i);
      if (j <= n) {
        data[j] = op(data[j], data[i]);
      }
    }
  }
  BIT(const std::vector<T> &vec) : n(vec.size()), data(std::vector<T>(vec.size() + 1, e())) {
    for (int i = 1; i <= n; ++i) {
      data[i] = op(data[i], vec[i - 1]);
      int j = i + (i & -i);
      if (j <= n) {
        data[j] = op(data[j], data[i]);
      }
    }
  }

  T prefix_sum(int r) const {
    assert(0 <= r && r <= n);
    T res = e();
    for (; r > 0; r -= (r & -r)) {
      res = op(data[r], res);
    }
    return res;
  }

  T sum(int l, int r) const {
    assert(0 <= l && l <= r && r <= n);
    return op(inv(prefix_sum(l)), prefix_sum(r));
  }

  T get(int p) const {
    assert(0 <= p && p < n);
    return sum(p, p + 1);
  }

  void add(int p, const T &x) {
    assert(0 <= p && p < n);
    for (++p; p <= n; p += (p & -p)) {
      data[p] = op(data[p], x);
    }
  }

  void set(int p, const T &x) {
    assert(0 <= p && p < n);
    T prev = get(p);
    add(p, op(inv(prev), x));
  }

  /**
   * @brief `f(prefix_sum(r)) == true` となる最大の r (0 <= r <= n) を返す
   * @pre rに関してf(prefix_sum(r))は true -> false の単調性を持つ
   * @pre f(e()) == true
   */
  template <class F> int max_right(F f) const {
    assert(f(e()));
    int res = 0;
    T val = e();
    for (int k = std::bit_floor(static_cast<unsigned>(n)); k > 0; k >>= 1) {
      if (k + res <= n) {
        T nxt = op(val, data[k + res]);
        if (f(nxt)) {
          val = std::move(nxt);
          res += k;
        }
      }
    }
    return res;
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

using Mint = Modint<998244353>;

Mint op(const Mint &a, const Mint &b) {
  return a + b;
}
Mint inv(const Mint &a) {
  return -a;
}
Mint e() {
  return Mint{0};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<Mint> nibeki(N + 1, 1), nibeki_inv(N + 1, 1);
  rep(i, 0, N) {
    nibeki[i + 1] = nibeki[i] * 2;
  }
  nibeki_inv[N] = nibeki[N].inv();
  rrep(i, 0, N) {
    nibeki_inv[i] = nibeki_inv[i + 1] * 2;
  }

  vector<int> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  vector<int> temp = A;
  sort(all(temp));
  temp.erase(unique(all(temp)), temp.end());
  vector<int> dict(N);
  rep(i, 0, N) {
    dict[i] = lower_bound(all(temp), A[i]) - temp.begin();
  }
  int M = temp.size();

  BIT<Mint, op, inv, e> ft(M);
  Mint ans = 0;
  rep(i, 0, N) {
    ans += nibeki[i] * nibeki_inv[1] * ft.prefix_sum(dict[i] + 1);
    ft.add(dict[i], nibeki_inv[i]);
  }
  cout << ans.val() << endl;
}