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
  int M = 3;
  int S = (1 << 10);
  int U = 2;
  auto state2idx = [&](int u, int m, int s) -> int {
    return s + (u * M + m) * S;
  };
  auto idx2state = [&](int idx) -> tuple<int, int, int> {
    int head = idx / S;
    return {head / M, head % M, idx % S};
  };
  vector<mint> dp(U * M * S, 0);
  dp[state2idx(0, 0, 0)] = 1;
  for (char c : N) {
    c -= '0';
    vector<mint> old(U * M * S, 0);
    swap(dp, old);
    rep(i, 0, U * M * S) {
      if (old[i] == 0) {
        continue;
      }
      auto [u, m, s] = idx2state(i);
      rep(d, 0, 10) {
        int nu = u;
        int nm = (m + d) % M;
        int ns = s | 1 << d;
        if (u == 0 && d > c) {
          continue;
        }
        if (u == 0 && d < c) {
          nu = 1;
        }
        if (s == 0 && d == 0) {
          ns = s;
        }
        dp[state2idx(nu, nm, ns)] += old[i];
      }
    }
  }

  mint ans = 0;
  rep(i, 0, U * M * S) {
    auto [u, m, s] = idx2state(i);
    int cnt = 0;
    if (m == 0) {
      cnt++;
    }
    if (s >> 3 & 1) {
      cnt++;
    }
    if (popcount((unsigned int)s) == 3) {
      cnt++;
    }
    if (cnt == 1) {
      ans += dp[i];
    }
  }
  ans--;
  cout << ans.val() << endl;
}