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
  ll N;
  cin >> N;
  vector<ll> R(N), C(N), X(N);
  rep(i, 0, N) {
    cin >> R[i] >> C[i] >> X[i];
  }
  map<pair<ll, ll>, ll> A;
  rep(i, 0, N) {
    A[{R[i], C[i]}] = X[i];
  }
  map<ll, ll> tate, yoko;
  rep(i, 0, N) {
    tate[C[i]] += X[i];
    yoko[R[i]] += X[i];
  }
  vector<pair<ll, ll>> T, Y;
  for (auto &[t, x] : tate) {
    T.emplace_back(x, t);
  }
  for (auto &[y, x] : yoko) {
    Y.emplace_back(x, y);
  }
  sort(rall(T));
  sort(rall(Y));
  ll ans = 0;
  rep(i, 0, T.size()) {
    auto [x, c] = T[i];
    rep(j, 0, Y.size()) {
      auto [y, r] = Y[j];
      chmax(ans, x + y - A[{r, c}]);
      if (A[{r, c}] == 0) {
        break;
      }
    }
  }
  cout << ans << endl;
}