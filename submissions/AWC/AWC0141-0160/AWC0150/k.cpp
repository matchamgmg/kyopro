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

  int N, M, K;
  cin >> N >> M >> K;
  struct Edge {
    int from, to, weight;
  };
  struct Edge2 {
    int to, weight;
  };

  vector<Edge> edges(M);
  vector<vector<Edge2>> G(N);
  rep(i, 0, M) {
    int u, v, w;
    cin >> u >> v >> w;
    u--, v--;
    if (u > v) {
      swap(u, v);
    }
    edges[i] = {u, v, w};
    G[u].emplace_back(v, w);
    G[v].emplace_back(u, w);
  }
  if (K == 2) {
    int ans = 1e9;
    for (Edge &e : edges) {
      chmin(ans, e.weight);
    }
    cout << ans << endl;
  } else if (K == 3) {
    int ans = 1e9;
    rep(i, 0, N) {
      if (G[i].size() <= 1) {
        continue;
      }
      int min_1 = 1e9, min_2 = 1e9;
      for (Edge2 &e : G[i]) {
        if (min_1 >= e.weight) {
          min_2 = min_1;
          min_1 = e.weight;
        } else if (min_2 >= e.weight) {
          min_2 = e.weight;
        }
      }
      chmin(ans, min_1 + min_2);
    }
    cout << ans << endl;
  } else if (K == 4) {
    int ans = 1e9;
    rep(i, 0, N) {
      if (G[i].size() <= 2) {
        continue;
      }
      int min_1 = 1e9, min_2 = 1e9, min3 = 1e9;
      for (Edge2 &e : G[i]) {
        if (min_1 >= e.weight) {
          min_3 = min_2;
          min_2 = min_1;
          min_1 = e.weight;
        } else if (min_2 >= e.weight) {
          min_3 = min_2;
          min_2 = e.weight;
        } else if (min_3 >= e.weight) {
          min_3 = e.weight;
        }
      }
      chmin(ans, min_1 + min_2 + min_3);
    }

    rep(i, 0, N) {
      if (G[i].size() <= 1) {
        continue;
      }
      vector<int> depth1_cost(G[i].size(), 1e9);
      vector<int> depth2_cost(G[i].size(), 1e9);
      rep(j, 0, G[i].size()) {
        Edge2 &e = G[i][j];
            }
    }
  }
}