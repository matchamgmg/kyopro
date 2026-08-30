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
  vector<int> B(N);
  rep(i, 0, N) { cin >> B[i]; }
  vector<pq<pair<int, int>>> G(N);
  rep(i, 0, M) {
    int U, V;
    cin >> U >> V;
    U--;
    V--;
    G[U].emplace(B[V], V);
  }
  set<pair<int, int>> yet;
  rep(i, 0, N) { yet.emplace(B[i], i); }
  vector<int> ans;
  int s = 0;
  do {
    int u = s;
    ans.emplace_back(u);
    yet.erase({B[u], u});
    int v;
    while (!G[u].empty()) {
      while (!G[u].empty() && !yet.contains(G[u].top())) {
        G[u].pop();
      }
      if (!G[u].empty()) {
        v = G[u].top().second;
        ans.emplace_back(v);
        yet.erase({B[v], v});
        u = v;
      }
    }
    if (!yet.empty()) {
      s = (*yet.rbegin()).second;
    }
  } while (!yet.empty());
  rep(i, 0, N) { cout << ans[i] + 1 << " \n"[i == N - 1]; }
}