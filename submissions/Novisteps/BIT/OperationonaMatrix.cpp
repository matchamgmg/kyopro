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

// クエリ3はQ_3回
// クエリ1をBITで加算
// i行目について、クエリ2で、クエリ3に必要なマスの情報を保管

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
  struct Query1 {
    int time;
    int l;
    int r;
    ll x;
  };
  struct Query2 {
    int time;
    ll x;
  };
  struct Query3 {
    int time;
    int j;
  };
  vector<pair<int, int>> queries(Q);
  vector<Query1> query1;
  vector<vector<Query2>> query2(N);
  vector<vector<Query3>> query3(N);
  rep(q, 0, Q) {
    int t;
    cin >> t;
    if (t == 1) {
      int l, r;
      ll x;
      cin >> l >> r >> x;
      l--;
      query1.emplace_back(q, l, r, x);
      queries[q] = {1, -1};
    } else if (t == 2) {
      int i;
      ll x;
      cin >> i >> x;
      i--;
      query2[i].emplace_back(q, x);
      queries[q] = {2, i};
    } else {
      int i, j;
      cin >> i >> j;
      i--;
      query3[i].emplace_back(q, j);
      queries[q] = {3, i};
    }
  }

  BIT<ll, op, inv, e> ft(M + 1);
  int i1 = 0;
  vector<int> i2(N, 0), i3(N, 0), i33(N, 0);
  struct Answer {
    int time;
    ll val;
    bool operator<(const Answer &rhs) {
      return time < rhs.time;
    }
  };
  vector<queue<Answer>> que(N);
  vector<Answer> ans;
  const int INF = 1e9;
  rep(i, 0, N) {
    int next_q = INF;
    if (i2[i] < query2[i].size()) {
      next_q = query2[i][i2[i]].time;
    }
    while (i3[i] < query3[i].size() && query3[i][i3[i]].time < next_q) {
      Query3 &b = query3[i][i3[i]];
      ++i3[i];
      que[i].emplace(b.time, 0 - ft.prefix_sum(b.j));
    }
  }
  rep(q, 0, Q) {
    if (queries[q].first == 1) {
      Query1 &a = query1[i1];
      ++i1;
      ft.add(a.l, a.x);
      ft.add(a.r, -a.x);
    } else if (queries[q].first == 2) {
      int i = queries[q].second;
      Query2 &a = query2[i][i2[i]];
      ++i2[i];
      int next_q = INF;
      if (i2[i] < query2[i].size()) {
        next_q = query2[i][i2[i]].time;
      }
      while (i3[i] < query3[i].size() && query3[i][i3[i]].time < next_q) {
        Query3 &b = query3[i][i3[i]];
        ++i3[i];
        que[i].emplace(b.time, a.x - ft.prefix_sum(b.j));
      }
    } else {
      int i = queries[q].second;
      Query3 &a = query3[i][i33[i]];
      ++i33[i];
      que[i].front().val += ft.prefix_sum(a.j);
      ans.push_back(que[i].front());
      que[i].pop();
    }
  }
  sort(all(ans));
  for (Answer &a : ans) {
    cout << a.val << endl;
  }
}