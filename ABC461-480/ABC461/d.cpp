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

  int H, W, K;
  cin >> H >> W >> K;
  vector<string> S(H);
  rep(i, 0, H) { cin >> S[i]; }

  vector<vector<int>> ruiseki(H + 1, vector<int>(W + 1, 0));
  rep(i, 0, H) {
    rep(j, 0, W) { ruiseki[i + 1][j + 1] = (S[i][j] == '1'); }
  }
  rep(i, 0, H + 1) {
    rep(j, 0, W) { ruiseki[i][j + 1] += ruiseki[i][j]; }
  }
  rep(i, 0, H) {
    rep(j, 0, W + 1) { ruiseki[i + 1][j] += ruiseki[i][j]; }
  }

  ll ans = 0;

  auto g = [&](int i, int j, int x, int A) -> ll {
    int ok = j, ng = W + 1;
    auto h = [&](int m) -> bool { return (ruiseki[x][m] - ruiseki[i][m] + A < K); };
    while (ng - ok > 1) {
      int mid = (ok + ng) / 2;
      (h(mid) ? ok : ng) = mid;
    }
    return ng - j;
  };

  auto f = [&](int i, int j) -> ll {
    ll res = 0;
    rep(x, i + 1, H + 1) {
      int a = -ruiseki[x][j] + ruiseki[i][j];
      res += g(i, j, x, a - 1) - g(i, j, x, a);
    }
    return res;
  };

  rep(i, 0, H) {
    rep(j, 0, W) { ans += f(i, j); }
  }
  cout << ans << endl;
}