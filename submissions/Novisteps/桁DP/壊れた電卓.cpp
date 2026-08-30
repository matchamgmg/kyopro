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

  ll A, K;
  cin >> A >> K;
  ll D = 0, a = A;
  ll TEN = 1;
  while (a) {
    D++;
    a /= 10;
    TEN *= 10;
  }
  TEN /= 10;
  const ll INF = 1e18;
  vector<ll> dp((1 << 10) * 3, INF);
  dp[1 << 10] = 0;
  while (TEN > 0) {
    vector<ll> old((1 << 10) * 3, INF);
    swap(dp, old);
    rep(i, 0, (1 << 10) * 3) {
      if (old[i] == INF) {
        continue;
      }
      int u = i / (1 << 10);
      int s = i % (1 << 10);
      rep(d, 0, 10) {
        int nu = u;
        int ns = s | 1 << d;
        if (u == 1 && d < A / TEN) {
          nu = 0;
        } else if (u == 1 && d > A / TEN) {
          nu = 2;
        }
        if (s == 0 && d == 0) {
          ns = s;
        }

        if (nu == 0) {
          chmin(dp[nu * (1 << 10) + ns], old[i] + (A / TEN - d) * TEN);
        } else if (nu == 2) {
          chmin(dp[nu * (1 << 10) + ns], old[i] + (d - A / TEN) * TEN);
        } else {
          chmin(dp[nu * (1 << 10) + ns], old[i]);
        }
        // dp[nu * (1 << 10) + ns];
      }
    }
    A %= TEN;
    TEN /= 10;
  }
  ll ans = INF;
  rep(i, 0, 3 * (1 << 10)) {
    if (popcount((unsigned ll)(i % (1 << 10))) <= K) {
      chmin(ans, dp[i]);
    }
  }
  cout << ans << endl;
}