#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

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

  ll N, X;
  cin >> N >> X;
  vector<ll> U(N), D(N);
  rep(i, 0, N) {
    cin >> U[i] >> D[i];
  }
  const ll INF = 3e9;
  ll ok = 0, ng = 3e9;
  auto f = [&](ll m) -> bool {
    ll l = -INF, r = INF;
    rep(i, 0, N) {
      if (m - U[i] > D[i]) {
        return false;
      }
      l = max(l - X, m - U[i]);
      r = min(r + X, D[i]);
      if (l > r) {
        return false;
      }
    }
    return true;
  };
  while (ng - ok > 1) {
    ll mid = (ok + ng) / 2;
    (f(mid) ? ok : ng) = mid;
  }
  auto calc = [&](ll h) -> ll {
    ll res = 0;
    rep(i, 0, N) {
      res += (U[i] + D[i]) - h;
    }
    return res;
  };
  cout << calc(ok) << endl;
}