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

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x) { cout << (x ? "Yes" : "No") << endl; }
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
bool grid_check(int x, int y, int X, int Y) { return (0 <= x && x < X && 0 <= y && y < Y); }
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
  ll Y;
  cin >> Y;
  struct Edge {
    int to;
    ll time;
    Edge() : to(-1), time(0) {}
    Edge(int to, ll time) : to(to), time(time) {}
  };

  vector<vector<Edge>> G(N + 2);
  int U, V;
  ll T;
  rep(i, 0, M) {
    cin >> U >> V >> T;
    U--;
    V--;
    G[U].emplace_back(V, T);
    G[V].emplace_back(U, T);
  }
  vector<ll> X(N);
  rep(i, 0, N) { cin >> X[i]; }
  rep(i, 0, N) {
    G[i].emplace_back(N, X[i]);
    G[N + 1].emplace_back(i, X[i]);
  }
  G[N].emplace_back(N + 1, Y);
  pq_g<pair<ll, int>> que;
  const ll INF = 1e18;
  vector<ll> dist(N + 2, INF);
  dist[0] = 0;
  que.emplace(0, 0);
  while (!que.empty()) {
    auto p = que.top();
    que.pop();
    if (p.first > dist[p.second]) {
      continue;
    }
    for (auto [to, time] : G[p.second]) {
      if (dist[to] <= p.first + time) {
        continue;
      }
      chmin(dist[to], p.first + time);
      que.emplace(dist[to], to);
    }
  }
  rep(i, 1, N) { cout << dist[i] << " \n"[i == N - 1]; }
}