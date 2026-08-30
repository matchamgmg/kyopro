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

  string N;
  cin >> N;
  int D = 10;
  int U = 2;
  int M = 3;
  auto state2idx = [&](int u, int m, int d) -> int {
    return d + (1 << D) * (u * M + m);
  };
  auto idx2state = [&](int idx) -> tuple<int, int, int> {
    int d = idx % (1 << D);
    idx /= (1 << D);
    return {idx / M, idx % M, d};
  };
  vector<mint> dp(U * M * (1 << D), 0);
  dp[state2idx(0, 0, 0)] = 1;
  for (char c : N) {
    c -= '0';
    vector<mint> old(U * M * (1 << D), 0);
    swap(dp, old);
    rep(i, 0, U * M * (1 << D)) {
      if (old[i] == 0) {
        continue;
      }
      auto [u, m, d] = idx2state(i);
      rep(j, 0, 10) {
        int nu = u;
        int nm = (m + j) % 3;
        int nd = d | 1 << j;
        if (u == 0 && j > c) {
          continue;
        }
        if (u == 0 && j < c) {
          nu = 1;
        }
        if (d == 0 && j == 0) {
          nd = d;
        }
        dp[state2idx(nu, nm, nd)] += old[i];
      }
    }
  }
  mint ans = 0;
  rep(i, 0, U * M * (1 << D)) {
    auto [u, m, d] = idx2state(i);
    int cnt = 0;
    if (m == 0) {
      cnt++;
    }
    if (d >> 3 & 1) {
      cnt++;
    }
    if (popcount((unsigned int)d) == 3) {
      cnt++;
    }
    if (cnt == 1) {
      ans += dp[i];
    }
  }
  cout << ans.val() - 1 << endl;
}