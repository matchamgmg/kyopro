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

ll solve(ll N) {
  string S = to_string(N);
  vector<ll> dp(2 * 10, 0);
  dp[0] = 1;
  for (char c : S) {
    c -= '0';
    vector<ll> old(2 * 10, 0);
    swap(dp, old);
    rep(i, 0, 2 * 10) {
      if (old[i] == 0) {
        continue;
      }
      int u = i / 10;
      int h = i % 10;
      rep(d, 0, 10) {
        int nu = u;
        int nh = h;
        if (u == 0 && d > c) {
          continue;
        }
        if (u == 0 && d < c) {
          nu = 1;
        }
        if (h == 0 && d != 0) {
          nh = d;
        }
        if (h != 0 && d >= h) {
          continue;
        }
        dp[nu * 10 + nh] += old[i];
      }
    }
  }
  ll res = 0;
  rep(i, 0, 2 * 10) { res += dp[i]; }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll L, R;
  cin >> L >> R;
  cout << solve(R) - solve(L - 1) << endl;
}