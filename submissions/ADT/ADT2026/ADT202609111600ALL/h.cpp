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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  struct Edge {
    int to;
    ll weight;
  };

  int N;
  cin >> N;
  vector<vector<Edge>> G(N);
  ll sum = 0;
  rep(i, 0, N - 1) {
    int a, b;
    ll c;
    cin >> a >> b >> c;
    a--, b--;
    G[a].emplace_back(b, c);
    G[b].emplace_back(a, c);
    sum += c;
  }
  const ll INF = 1e18;
  vector<ll> dist1(N, INF);
  auto bfs = [&](vector<ll> &dist, int s) -> void {
    queue<int> que;
    dist[s] = 0;
    que.emplace(s);
    while (!que.empty()) {
      auto u = que.front();
      que.pop();
      for (auto e : G[u]) {
        if (dist[e.to] < INF) {
          continue;
        }
        dist[e.to] = dist[u] + e.weight;
        que.emplace(e.to);
      }
    }
  };

  bfs(dist1, 0);
  int ss = -1;
  ll max_d = 0;
  rep(i, 0, N) {
    if (max_d <= dist1[i]) {
      max_d = dist1[i];
      ss = i;
    }
  }
  vector<ll> dist2(N, INF);
  bfs(dist2, ss);
  cout << 2 * sum - *max_element(all(dist2)) << endl;
}