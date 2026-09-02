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

int op(int a, int b) {
  return a + b;
}
int inv(int a) {
  return -a;
}
int e() {
  return 0;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  vector<int> A(N);
  rep(i, 0, N) {
    cin >> A[i];
    A[i]--;
  }
  int Q;
  cin >> Q;
  vector<ll> X(Q);
  vector<int> qidx(Q);
  vector<int> ans(Q);
  rep(q, 0, Q) {
    cin >> X[q];
    X[q]--;
    qidx[q] = q;
  }
  sort(all(qidx), [&](int i, int j) {
    return X[i] < X[j];
  });

  struct T {
    int val;
    int siz;
    bool operator>(const T &rhs) const {
      return siz == rhs.siz ? val > rhs.val : siz > rhs.siz;
    }
  };

  vector<int> cnt(M, 0);
  rep(i, 0, N) {
    cnt[A[i]]++;
  }
  pq_g<T> que;
  rep(i, 0, M) {
    que.emplace(i, cnt[i]);
  }
  ll siz = N;
  int q = 0;
  while (q < Q && X[qidx[q]] < N) {
    ans[qidx[q]] = A[X[qidx[q]]] + 1;
    ++q;
  }
  BIT<int, op, inv, e> ft(M);
  const ll INF = 2e18;
  ll width = 0;
  while (!que.empty()) {
    T p = que.top();
    while (!que.empty() && que.top().siz == p.siz) {
      T q = que.top();
      que.pop();
      ft.add(q.val, 1);
      ++width;
    }
    ll next_siz = siz;
    if (!que.empty()) {
      next_siz += (que.top().siz - p.siz) * width;
    } else {
      next_siz = INF;
    }
    while (q < Q && X[qidx[q]] < next_siz) {
      int k = (X[qidx[q]] - siz) % width;
      ans[qidx[q]] = ft.max_right([&](int s) {
        return s <= k;
      }) + 1;
      ++q;
    }
    siz = next_siz;
  }
  rep(q, 0, Q) {
    cout << ans[q] << endl;
  }
}