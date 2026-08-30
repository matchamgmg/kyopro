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
  vector<vector<int>> G(N);
  rep(i, 0, M) {
    int U, V;
    cin >> U >> V;
    U--;
    V--;
    G[U].emplace_back(V);
    G[V].emplace_back(U);
  }
  string S;
  cin >> S;

  const int INF = 1e9;
  queue<tuple<int, int, int>> que;
  vector<set<int>> visited(N, set<int>());
  vector<vector<int>> dist(N, vector<int>(2, INF));

  rep(i, 0, N) {
    if (S[i] == 'S') {
      que.emplace(i, i, 0);
      dist[i][0] = 0;
    }
  }
  while (!que.empty()) {
    auto [u, f, d] = que.front();
    que.pop();
    for (int v : G[u]) {
      if (visited[v].size() >= 2) {
        continue;
      }
      if (visited[v].contains(f)) {
        continue;
      }
      if (dist[v][0] < INF) {
        dist[v][1] = d + 1;
      } else {
        dist[v][0] = d + 1;
      }
      que.emplace(v, f, d + 1);
      visited[v].emplace(f);
    }
  }
  rep(i, 0, N) {
    if (S[i] == 'D') {
      cout << dist[i][0] + dist[i][1] << endl;
    }
  }
}