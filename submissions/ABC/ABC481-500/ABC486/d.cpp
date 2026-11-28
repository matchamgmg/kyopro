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
  vector<ll> L(N), P(N);
  rep(i, 0, N) {
    cin >> L[i] >> P[i];
  }
  const ll INF = 1e18;
  vector<ll> dp(N + 2, INF);
  dp[1] = 0;
  bool ok = false;
  rep(i, 0, N) {
    vector<ll> next_dp(N + 2, INF);
    if (i == N - 1) {
      rep(j, 0, N + 1) {
        if (j >= L[i]) {
          chmin(next_dp[j + 1], dp[j] + P[i]);
        }
      }
    } else {
      rep(j, 0, N + 1) {
        if (j < L[i]) {
          chmin(next_dp[j], dp[j]);
        } else {
          chmin(next_dp[j], dp[j]);
          chmin(next_dp[j + 1], dp[j] + P[i]);
        }
      }
    }
    swap(dp, next_dp);
  }
  ll ans = INF;
  rep(i, 0, N + 2) {
    chmin(ans, dp[i]);
  }
  cout << (ans == INF ? -1 : ans) << endl;
}