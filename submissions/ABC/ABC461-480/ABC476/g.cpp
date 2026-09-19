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

vector<ll> f(ll N) {
  vector<vector<ll>> dp(2, vector<ll>(61, 0)); // dp[1][i]: N未満確定, dp[0][i]:N以上かも(明確にNより大は除く)
  dp[0][0] = 1;
  rrep(i, 0, 60) {
    vector<vector<ll>> next_dp(2, vector<ll>(61, 0));
    rep(j, 0, 2) {
      rep(k, 0, 60) {
        if (dp[j][k] == 0) {
          continue;
        }
        if (j == 0) {
          if ((N >> i) & 1) {
            next_dp[0][k + 1] += dp[0][k];
            next_dp[1][k] += dp[0][k];
          } else {
            next_dp[0][k] += dp[0][k];
          }
        } else {
          next_dp[1][k + 1] += dp[1][k];
          next_dp[1][k] += dp[1][k];
        }
      }
    }
    swap(dp, next_dp);
  }
  vector<ll> res(61, 0);
  rep(i, 0, 61) {
    res[i] = dp[0][i] + dp[1][i];
  }
  return res;
}

void solve() {
  ll L, R;
  cin >> L >> R;
  L--;
  vector<ll> fR, fL;
  fR = f(R);
  fL = f(L);
  ll ans = 0;
  ll cnt = 0;
  rep(i, 0, 61) {
    chmax(ans, fR[i] - fL[i]);
    cnt += fR[i] - fL[i];
  }
  cout << ans << endl;
  cout << cnt << ' ' << R - L << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--)
    solve();
}