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

// 辺をトポロジカルソート
// dp

void solve() {
  struct Edge {
    int from;
    int to;
    ll w;
  };
  int N, M;
  cin >> N >> M;
  vector<Edge> edges(M);
  rep(i, 0, M) {
    int u, v;
    ll c;
    cin >> u >> v >> c;
    u--, v--;
    edges[i] = Edge{u, v, c};
  }
  vector<int> idx(M);
  iota(all(idx), 0);
  sort(all(idx), [&](int i, int j) {
    return edges[i].w < edges[j].w;
  });
  vector<ll> ans(N, 0);
  int i = 0;
  while (i < M) {
    ll w = edges[idx[i]].w;
    map<int, ll> mp;
    while (i < M && edges[idx[i]].w == w) {
      int u = edges[idx[i]].from;
      int v = edges[idx[i]].to;
      i++;
      if (!mp.contains(u)) {
        mp[u] = ans[v] + w;
      } else {
        chmax(mp[u], ans[v] + w);
      }
      if (!mp.contains(v)) {
        mp[v] = ans[u] + w;
      } else {
        chmax(mp[v], ans[u] + w);
      }
    }
    for (auto [k, v] : mp) {
      chmax(ans[k], v);
    }
  }

  cout << *max_element(all(ans)) << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}