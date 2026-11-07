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

  int N;
  cin >> N;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  const ll INF = 1e18;
  vector<ll> Smin(N + 1, INF), Smax(N + 1, -INF);
  vector<ll> SA(N + 1, 0);
  Smin[0] = 0, Smax[0] = 0;
  ll current = 0;
  rep(i, 0, N) {
    current += A[i];
    SA[i + 1] = current;
    Smin[i + 1] = min(Smin[i], current);
    Smax[i + 1] = max(Smax[i], current);
  }
  vector<ll> RSmin(N + 1, INF), RSmax(N + 1, -INF);
  RSmin[N] = 0, RSmax[N] = 0;
  vector<ll> emax(N + 1, -INF), emin(N + 1, INF);
  emax[N] = 0, emin[N] = 0;
  current = 0;
  rrep(i, 0, N) {
    current -= A[i];
    RSmin[i] = min(RSmin[i + 1], current);
    RSmax[i] = max(RSmax[i + 1], current);
    emin[i] = RSmin[i] - current;
    emax[i] = RSmax[i] - current;
  }

  ll ans = Smax[N] - Smin[N] + 1;
  rep(i, 0, N) {
    ll imin = SA[i] - A[i] + emin[i + 1];
    ll imax = SA[i] - A[i] + emax[i + 1];
    ll ians = max(imax, Smax[i]) - min(imin, Smin[i]) + 1;
    chmax(ans, ians);
  }
  cout << ans << endl;
}