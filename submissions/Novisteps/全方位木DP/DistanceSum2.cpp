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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<vector<int>> G(N);
  rep(i, 0, N - 1) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    G[u].emplace_back(v);
    G[v].emplace_back(u);
  }

  struct T {
    ll val;
    ll siz;
  };
  vector<vector<T>> dp(N); ///< dp[i][j]...有向辺 j -> i を辿ってi側に届いた値
  vector<T> ans(N);        ///< 各頂点を根としたときの答え

  auto e = [&]() -> T { return T{0, 0}; };
  auto merge = [&](const T &a, const T &b) -> T { return {a.val + b.val, a.siz + b.siz}; };
  auto add_vertex = [&](const T &x) -> T { return {x.val, x.siz + 1}; };
  auto add_edge = [&](const T &x) -> T { return {x.val + x.siz, x.siz}; };

  auto dfs1 = [&](auto f, int u, int p) -> T {
    int s = G[u].size();
    dp[u].assign(s, e());
    T res = e();
    for (int i = 0; i < s; ++i) {
      int v = G[u][i];
      if (v == p) {
        continue;
      }
      dp[u][i] = f(f, v, u);
      res = merge(res, dp[u][i]);
    }
    return add_edge(add_vertex(res));
  };

  auto dfs2 = [&](auto f, int u, int p, T dp_p) -> void {
    int s = G[u].size();
    for (int i = 0; i < s; ++i) {
      int v = G[u][i];
      if (v == p) {
        dp[u][i] = dp_p;
      }
    }

    vector<T> dp_l(s + 1, e()), dp_r(s + 1, e());
    for (int i = 0; i < s; ++i) {
      dp_l[i + 1] = merge(dp_l[i], dp[u][i]);
    }
    for (int i = s - 1; i >= 0; --i) {
      dp_r[i] = merge(dp_r[i + 1], dp[u][i]);
    }
    ans[u] = add_vertex(dp_l[s]);

    for (int i = 0; i < s; ++i) {
      int v = G[u][i];
      if (v == p) {
        continue;
      }
      f(f, v, u, add_edge(add_vertex(merge(dp_l[i], dp_r[i + 1]))));
    }
  };

  dfs1(dfs1, 0, -1);
  dfs2(dfs2, 0, -1, e());

  rep(i, 0, N) {
    cout << ans[i].val << endl;
  }
}