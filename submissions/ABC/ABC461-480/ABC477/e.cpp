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

  int N, Q;
  cin >> N >> Q;
  vector<ll> A(N), B(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  rep(i, 0, N) {
    cin >> B[i];
  }
  struct Edge {
    int to;
    ll weight;
  };

  vector<vector<Edge>> G(N + 1);
  rep(i, 0, N) {
    G[i].emplace_back((i + 1) % N, A[i]);
    G[(i + 1) % N].emplace_back(i, A[i]);
    G[i].emplace_back(N, B[i]);
    G[N].emplace_back(i, B[i]);
  }

  const ll INF = 1e18;
  vector<ll> dist_from_N(N + 1, INF);
  dist_from_N[N] = 0;
  pq_g<pair<ll, int>> que;
  que.emplace(0, N);
  while (!que.empty()) {
    auto p = que.top();
    que.pop();
    int u = p.second;
    ll d = p.first;
    if (d > dist_from_N[u]) {
      continue;
    }
    for (auto &e : G[u]) {
      ll nd = d + e.weight;
      if (nd >= dist_from_N[e.to]) {
        continue;
      }
      dist_from_N[e.to] = nd;
      que.emplace(nd, e.to);
    }
  }
  vector<ll> direct(N + 1, 0);
  rep(i, 0, N) {
    direct[i + 1] = direct[i] + A[i];
  }
  ll sum = direct[N];
  rep(q, 0, Q) {
    int S, T;
    cin >> S >> T;
    S--;
    T--;
    if (T == N) {
      cout << dist_from_N[S] << endl;
    } else {
      cout << min(min(direct[T] - direct[S], sum - (direct[T] - direct[S])), dist_from_N[S] + dist_from_N[T]) << endl;
    }
  }
}