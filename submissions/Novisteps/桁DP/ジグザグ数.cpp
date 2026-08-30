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

const int MOD = 10000;

int solve(string N, int M) {
  auto state2idx = [&](int u, int up, int la, int m) -> int {
    return u * (2 * M * 11) + up * (M * 11) + la * M + m;
  };
  auto idx2state = [&](int idx) -> tuple<int, int, int, int> {
    return {idx / M / 11 / 2, idx / M / 11 % 2, idx / M % 11, idx % M};
  };
  vector<int> dp(2 * 2 * 11 * M, 0);
  dp[state2idx(0, 1, 10, 0)] = dp[state2idx(0, 0, 10, 0)] = 1;
  for (char c : N) {
    c -= '0';
    vector<int> old(2 * 2 * 11 * M, 0);
    swap(dp, old);
    rep(i, 0, 2 * 2 * 11 * M) {
      if (old[i] == 0) {
        continue;
      }
      auto [u, up, la, m] = idx2state(i);
      rep(d, 0, 10) {
        int nu = u;
        int nup = 1 - up;
        int nla = d;
        int nm = (m * 10 + d) % M;
        if (u == 0 && d > c) {
          continue;
        }
        if (u == 0 && d < c) {
          nu = 1;
        }
        if (la == 10 && d == 0) {
          nla = la;
        }
        if (la == 10) {
          dp[state2idx(nu, nup, nla, nm)] += old[i];
        } else {
          if (up == 1 && la > d) {
            dp[state2idx(nu, nup, nla, nm)] += old[i];
          } else if (up == 0 && la < d) {
            dp[state2idx(nu, nup, nla, nm)] += old[i];
          }
        }
        dp[state2idx(nu, nup, nla, nm)] %= MOD;
      }
    }
  }
  int res = 0;
  rep(i, 0, 2 * 2 * 11 * M) {
    auto [u, up, la, m] = idx2state(i);
    if (m == 0 && la != 10) {
      res += dp[i];
      res %= MOD;
    }
  }
  int res1 = 0;
  int S = N.size();
  int n = (S > 2 ? 10 : N[0] - '0');
  for (int i = M; i < min(10, n); i += M) {
    res1++;
  }
  return (res - res1 + MOD) % MOD;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string A, B;
  cin >> A >> B;
  int N = A.size();
  int i = N - 1;
  while (i >= 0 && A[i] == '0') {
    i--;
  }
  A[i] -= 1;
  rep(j, i + 1, N) { A[j] = '9'; }
  int M;
  cin >> M;
  //   cout << solve(A, M) << endl;
  //   cout << solve(B, M) << endl;
  cout << (solve(B, M) - solve(A, M) + MOD) % MOD << endl;
}