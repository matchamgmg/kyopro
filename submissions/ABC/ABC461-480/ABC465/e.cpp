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

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
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

/**
 * 桁DP
 * * 3で割り切れるか
 * * 3を含むか
 * * 3種類か
 * * N以下か
 *
 * dp[上からi桁決めた][N未満が確定した or
 * 今までが一致][今までのmod3][使った数字の集合]
 *
 * 使った数字の集合 : 2^10 通り
 */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string S;
  cin >> S;
  int D = S.size();

  using T = tuple<int, int, int>;
  map<T, mint> dp;
  dp[T(0, 0, 0)] = 1;
  for (char c : S) {
    c -= '0';
    map<T, mint> old;
    swap(old, dp);
    for (auto [t, x] : old) {
      auto [under_N, mod_3, set_digit] = t;
      rep(d, 0, 10) {
        int n_under_N = under_N, n_mod_3 = (mod_3 + d) % 3,
            n_set_digit = set_digit | 1 << d;
        if (n_under_N == 0 && d > c) {
          continue;
        }
        if (d < c) {
          n_under_N = 1;
        }
        if (set_digit == 0 && d == 0) {
          n_set_digit = 0;
        }
        dp[T(n_under_N, n_mod_3, n_set_digit)] += x;
      }
    }
  }

  mint ans = 0;
  for (auto [t, x] : dp) {
    auto [i, j, k] = t;
    int cnt = 0;
    if (j == 0) {
      cnt++;
    }
    if (popcount((unsigned int)k) == 3) {
      cnt++;
    }
    if (k >> 3 & 1) {
      cnt++;
    }
    if (cnt == 1) {
      ans += x;
    }
  }
  ans -= 1;
  cout << ans.val() << endl;
}