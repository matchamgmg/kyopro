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

void solve() {

  int N, M;
  cin >> N >> M;
  vector<vector<int>> G(N);
  vector<int> color(N, -1);
  vector<pair<int, int>> rest_edge;
  UnionFind uf(N);
  rep(i, 0, M) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    if (uf.same(a, b)) {
      rest_edge.emplace_back(a, b);
    } else {
      uf.merge(a, b);
      G[a].emplace_back(b);
      G[b].emplace_back(a);
    }
  }
  auto dfs = [&](auto f, int u, int p, int c) -> void {
    color[u] = c;
    c ^= 1;
    for (int v : G[u]) {
      if (v == p) {
        continue;
      }
      f(f, v, u, c);
    }
  };

  dfs(dfs, 0, -1, 0);

  int s = -1, g = -1;
  for (auto &[a, b] : rest_edge) {
    if (color[a] == color[b]) {
      s = a;
      g = b;
    }
  }
  if (s == -1 && g == -1) {
    cout << -1 << endl;
    return;
  }

  stack<int> st;
  auto dfs2 = [&](auto f, int u, int p) -> bool {
    bool res = false;
    if (u == g) {
      res = true;
    }
    st.emplace(u);
    for (int v : G[u]) {
      if (v == p) {
        continue;
      }
      res |= f(f, v, u);
    }
    if (!res) {
      st.pop();
    }
    return res;
  };

  dfs2(dfs2, s, -1);

  cout << st.size() << endl;
  while (!st.empty()) {
    cout << st.top() + 1 << ' ';
    st.pop();
  }
  cout << endl;
}

int main() {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--)
    solve();
}