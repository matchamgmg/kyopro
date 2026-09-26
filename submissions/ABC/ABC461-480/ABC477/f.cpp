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

ll op(ll a, ll b) {
  return a + b;
}
ll inv(ll a) {
  return -a;
}
ll e() {
  return 0;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, Q;
  cin >> N >> M >> Q;
  BIT<ll, op, inv, e> ft(M + 1);
  vector<ll> L(N), R(N);
  rep(i, 0, N) {
    cin >> L[i] >> R[i];
    L[i]--;
  }
  struct Query {
    int a, b, c, d;
  };
  vector<Query> queries(Q);
  vector<int> qidx(Q);
  rep(q, 0, Q) {
    qidx[q] = q;
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    a--;
    c--;
    queries[q] = Query{a, b, c, d};
  }

  auto Sort_by_snake_order = [&](vector<Query> &que, vector<int> &idx) {
    int divide = std::max(1, int(N / std::sqrt(Q)));
    auto Comp = [&que, divide](int i, int j) -> bool {
      if (que[i].a / divide != que[j].a / divide) {
        return que[i].a < que[j].a;
      }
      if ((que[i].a / divide) & 1) {
        return que[i].b > que[j].b;
      }
      return que[i].b < que[j].b;
    };
    std::sort(idx.begin(), idx.end(), Comp);
  };

  Sort_by_snake_order(queries, qidx);

  vector<ll> ans(Q);

  {
    int current_l = 0, current_r = 0;
    for (int i : qidx) {
      while (current_l > queries[i].a) {
        --current_l;
        ft.add(L[current_l], 1);
        ft.add(R[current_l], -1);
      }
      while (current_r < queries[i].b) {
        ft.add(L[current_r], 1);
        ft.add(R[current_r], -1);
        ++current_r;
      }
      while (current_l < queries[i].a) {
        ft.add(L[current_l], -1);
        ft.add(R[current_l], 1);
        ++current_l;
      }
      while (current_r > queries[i].b) {
        --current_r;
        ft.add(L[current_r], -1);
        ft.add(R[current_r], 1);
      }
      ans[i] = ft.sum(queries[i].c, queries[i].d);
    }
  }
  rep(q, 0, Q) {
    cout << ans[qidx[q]] << endl;
  }
}