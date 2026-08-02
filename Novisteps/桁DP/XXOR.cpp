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
  rep(i, 0, N) { cin >> A[i]; }
  ll ans = 0;
  const ll INF = 1e18;
  vector<ll> dp(2, -INF);
  dp[0] = 0;
  rrep(i, 0, 60) {
    ll c = (K >> i & 1);
    vector<ll> old(2, -INF);
    swap(dp, old);
    ll cnt = 0;
    rep(j, 0, N) { cnt += (A[j] >> i & 1); }
    rep(k, 0, 2) {
      if (old[k] == -INF) {
        continue;
      }
      if (k == 0) {
        if (c == 0) {
          chmax(dp[0], old[k] + (1LL << i) * cnt);
        } else {
          chmax(dp[0], old[k] + (1LL << i) * (N - cnt));
          chmax(dp[1], old[k] + (1LL << i) * cnt);
        }
      } else {
        chmax(dp[1], old[k] + (1LL << i) * max(cnt, (N - cnt)));
      }
    }
  }
  cout << max(dp[0], dp[1]) << endl;
}