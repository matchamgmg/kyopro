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

  int N, M, S, T;
  cin >> N >> M >> S >> T;
  S--, T--;
  struct Edge {
    int to;
    int w;
    bool operator<(const Edge &rhs) const {
      return to < rhs.to;
    }
  };
  vector<vector<Edge>> G(N);
  rep(i, 0, M) {
    int a, b;
    cin >> a >> b;
    a--, b--;
    G[a].emplace_back(b, 1);
  }
  rep(i, 0, N) {
    sort(all(G[i]));
    if (G[i].size() == 0) {
      continue;
    }
    G[i][0].w = 0;
  }

  const int INF = 1e9;
  vector<int> dist(N, INF);
  deque<int> que;
  dist[S] = 0;
  que.emplace_back(S);
  while (!que.empty()) {
    int u = que.front();
    int d = dist[u];
    que.pop_front();
    if (dist[u] < d) {
      continue;
    }
    for (auto &e : G[u]) {
      int nd = d + e.w;
      if (dist[e.to] <= nd) {
        continue;
      }
      dist[e.to] = nd;
      if (e.w == 0) {
        que.emplace_front(e.to);
      } else {
        que.emplace_back(e.to);
      }
    }
  }
  cout << (dist[T] == INF ? -1 : dist[T]) << endl;
}