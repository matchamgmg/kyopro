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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;

  struct Edge {
    int to;
    int col;
  };
  vector<vector<Edge>> G(N);
  rep(i, 0, M) {
    int u, v, c;
    cin >> u >> v >> c;
    u--;
    v--;
    G[u].emplace_back(v, c);
  }

  const int INF = 1e9;
  vector<vector<pair<int, int>>> dist(N, vector<pair<int, int>>(2, {-1, INF}));
  queue<pair<int, int>> que; // v, col
  dist[0][0] = {0, 0};
  que.emplace(0, 0);
  while (!que.empty()) {
    auto [u, c] = que.front();
    que.pop();
    int d = INF;
    rep(i, 0, 2) {
      if (c == dist[u][i].first) {
        d = dist[u][i].second;
      }
    }
    if (d == INF) {
      continue;
    }
    for (auto e : G[u]) {
      if (c == e.col) {
        continue;
      }
      int v = e.to;
      int nd = d + 1;
      rep(i, 0, 2) {
        if (dist[v][i].first == e.col) {
          break;
        }
        if (dist[v][i].first != -1) {
          continue;
        }
        dist[v][i] = {e.col, nd};
        que.emplace(v, e.col);
        break;
      }
    }
  }
  cout << (dist[N - 1][0].second == INF ? -1 : dist[N - 1][0].second) << endl;
}