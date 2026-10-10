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

void solve() {
  ll N;
  cin >> N;
  vector<ll> X(N), Y(N), Z(N);
  rep(i, 0, N) {
    cin >> X[i] >> Y[i] >> Z[i];
  }
  const ll INF = 1e18;
  vector<ll> dp(2, INF);
  dp[0] = 0, dp[1] = 0;
  set<pair<ll, ll>> st(2);
  st.insert({0, 0});
  auto calc_dist2 = [](ll x1, ll y1, ll x2, ll y2) -> ll {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
  };
  rep(i, 0, N) {
    vector<ll> next_dp(2, INF);
    // 剣を使う
    if (i >= 1) {
      ll d = calc_dist2(X[i], Y[i], X[i - 1], Y[i - 1]);
      chmin(next_dp[0], dp[0] + d);
    } else {
      ll d = calc_dist2(X[i], Y[i], 0, 0);
      chmin(next_dp[0], dp[0] + d);
    }
    for (auto p : st) {
      ll d = calc_dist2(X[i], Y[i], p.first, p.second);
      chmin(next_dp[0], dp[1] + d);
    }
    // 魔法を使う
    Z
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}