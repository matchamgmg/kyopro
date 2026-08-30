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

  int N;
  cin >> N;
  vector<int> P(N, -1);
  rep(i, 1, N) {
    cin >> P[i];
    P[i]--;
  }
  vector<vector<int>> G(N);
  rep(i, 1, N) {
    G[P[i]].emplace_back(i);
    G[i].emplace_back(P[i]);
  }

  vector<ll> C(N), D(N);
  rep(i, 0, N) { cin >> C[i]; }
  rep(i, 0, N) { cin >> D[i]; }

  vector<mint> mod_inv(2e6);
  rep(i, 1, 2e6) { mod_inv[i] = mint(i).inv(); }

  bool ok = true;
  vector<mint> ans(N, 0);
  auto DFS = [&](auto f, int u, int p) -> ll {
    ll rest = 0;
    for (int v : G[u]) {
      if (v == p) {
        continue;
      }
      rest += f(f, v, u);
    }
    rest += C[u];
    if (rest < D[u]) {
      ok = false;
    }
    if (ok) {
      ans[u] = 1;
      rep(i, 1, D[u] + 1) { ans[u] = ans[u] * (rest - i + 1) * mod_inv[i]; }
    } else {
      ans[u] = 0;
    }
    return rest - D[u];
  };

  DFS(DFS, 0, -1);
  mint anss = 1;
  rep(i, 0, N) { anss = anss * ans[i]; }
  cout << anss.val() << endl;
}