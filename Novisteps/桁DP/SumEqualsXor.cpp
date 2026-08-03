#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
// using mint = modint998244353;
using mint = modint1000000007;
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

  string L;
  cin >> L;
  int N = L.size();
  vector<mint> dp(2, 0);
  dp[0] = 1;
  for (char c : L) {
    c -= '0';
    vector<mint> old(2, 0);
    swap(dp, old);
    rep(i, 0, 2) {
      if (old[i] == 0) {
        continue;
      }
      rep(d, 0, 2) {
        int ni = i;
        if (i == 0 && d > c) {
          continue;
        }
        if (i == 0 && d < c) {
          ni = 1;
        }
        if (d == 0) {
          dp[ni] += old[i];
        } else {
          dp[ni] += old[i] * 2;
        }
      }
    }
  }
  cout << (dp[0] + dp[1]).val() << endl;
}