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

  const ll INF = 1e18;
  int N;
  cin >> N;
  vector<int> P(N, -1);
  vector<ll> W(N, INF);
  rep(i, 1, N) {
    cin >> P[i] >> W[i];
    P[i]--;
  }
  vector<vector<int>> G(N);
  rep(i, 1, N) { G[P[i]].emplace_back(i); }
  vector<int> siz(N, 0);
  auto dfs1 = [&](auto f, int u, int p) -> int {
    int res = 1;
    for (int v : G[u]) {
      if (v == p) {
        continue;
      }
      res += f(f, v, u);
    }
    return siz[u] = res;
  };
  dfs1(dfs1, 0, -1);
  rep(i, 0, N) {
    sort(all(G[i]), [&](int j, int k) { return siz[j] > siz[k]; });
  }
  vector<int> vertex, id(N), head(N);
  int cnt = 0;
  auto dfs2 = [&](auto f, int u, int p, int h) -> void {
    vertex.emplace_back(u);
    id[u] = cnt;
    cnt++;
    head[u] = h;
    int cnt2 = 0;
    for (int v : G[u]) {
      if (v == p) {
        continue;
      }
      if (cnt2 == 0) {
        f(f, v, u, h);
      } else {
        f(f, v, u, v);
      }
      cnt2++;
    }
  };
  dfs2(dfs2, 0, -1, 0);
  v_cout(vertex);
  v_cout(id);
  v_cout(head);
}