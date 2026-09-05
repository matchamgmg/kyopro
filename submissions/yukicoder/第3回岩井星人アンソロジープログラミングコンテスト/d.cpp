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

  ll N, K;
  cin >> N >> K;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }

  const ll INF = 1e18;
  vector<vector<ll>> dp(2, vector<ll>(K + 1, -INF));
  dp[0][0] = 0;
  rep(i, 0, N) {
    vector<vector<ll>> old_dp(2, vector<ll>(K + 1, -INF));
    swap(dp, old_dp);
    rep(k, 0, K + 1) {
      // 選ばなかった
      chmax(dp[0][k], old_dp[0][k]);
      chmax(dp[0][k], old_dp[1][k]);
      // 選んだ
      if (k < K && old_dp[0][k] != -INF) {
        chmax(dp[1][k + 1], old_dp[0][k] + A[i]);
      }
    }
  }

  if (max(dp[0][K], dp[1][K]) == -INF) {
    cout << "Impossible" << endl;
  } else {
    cout << max(dp[0][K], dp[1][K]) << endl;
  }
}