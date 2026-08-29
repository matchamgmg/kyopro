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

struct UnionFind {
  UnionFind(int n_) : n(n_), group_num(n_) {
    par.assign(n_, -1);
  }
  int root(int x) {
    if (par[x] < 0)
      return x;
    return par[x] = root(par[x]);
  }
  int size(int x) {
    return -par[root(x)];
  }
  bool same(int x, int y) {
    return root(x) == root(y);
  }
  bool merge(int x, int y) {
    x = root(x);
    y = root(y);
    if (x == y)
      return false;
    if (size(x) > size(y))
      swap(x, y);
    par[y] += par[x];
    par[x] = y;
    group_num--;
    return true;
  }
  int groups() {
    return group_num;
  }

private:
  int n;
  int group_num;
  vector<int> par;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  struct Edge {
    double cost;
    int from;
    int to;
    bool operator>(const Edge &rhs) const {
      return cost == rhs.cost ? (from == rhs.from ? to > rhs.to : from > rhs.from) : cost > rhs.cost;
    }
  };
  vector<int> A(M), B(M);
  vector<ll> C(M), T(M);
  rep(i, 0, M) {
    cin >> A[i] >> B[i] >> C[i] >> T[i];
  }

  auto f = [&](double m) -> bool {
    UnionFind uf(N);
    vector<Edge> E(M);
    rep(i, 0, M) {
      E[i] = {C[i] - m * T[i], A[i], B[i]};
    }
    pq_g<Edge> que;
    rep(i, 0, M) {
      que.push(E[i]);
    }
    queue<Edge> rem;
    double res = 0;
    while (!que.empty()) {
      auto e = que.top();
      que.pop();
      if (uf.same(e.from, e.to)) {
        rem.push(e);
      } else {
        uf.merge(e.from, e.to);
        res += e.cost;
      }
    }
    while (!rem.empty()) {
      auto e = rem.front();
      rem.pop();
      if (e.cost < 0.) {
        res += e.cost;
      }
    }
    return res < 0.;
  };

  double ok = 1e10, ng = 0;
  while (ok - ng > 1e-3) {
    double mid = (ok + ng) / 2;
    (f(mid) ? ok : ng) = mid;
  }
  cout << fixed << setprecision(4) << ok << endl;
}