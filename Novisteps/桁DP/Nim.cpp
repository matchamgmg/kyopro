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

  ll N, A1, A2, A3;
  cin >> N >> A1 >> A2 >> A3;

  auto state2idx = [&](int u1, int u2, int u3, int l1, int l2, int l3, int m1,
                       int m2, int m3) -> int {
    int idx = u1;
    idx = idx * 2 + u2;
    idx = idx * 2 + u3;
    idx = idx * 2 + l1;
    idx = idx * 2 + l2;
    idx = idx * 2 + l3;
    idx = idx * A1 + m1;
    idx = idx * A2 + m2;
    idx = idx * A3 + m3;
    return idx;
  };
  auto idx2state =
      [&](int idx) -> tuple<int, int, int, int, int, int, int, int, int> {
    int m3 = idx % A3;
    idx /= A3;
    int m2 = idx % A2;
    idx /= A2;
    int m1 = idx % A1;
    idx /= A1;
    int l3 = idx % 2;
    idx /= 2;
    int l2 = idx % 2;
    idx /= 2;
    int l1 = idx % 2;
    idx /= 2;
    int u3 = idx % 2;
    idx /= 2;
    int u2 = idx % 2;
    idx /= 2;
    int u1 = idx;
    return {u1, u2, u3, l1, l2, l3, m1, m2, m3};
  };
  vector<mint> dp(8 * 8 * A1 * A2 * A3, 0);
  dp[state2idx(0, 0, 0, 1, 1, 1, 0, 0, 0)] = 1;
  rrep(b, 0, 62) {
    int c = (N >> b & 1);
    vector<mint> old(8 * 8 * A1 * A2 * A3, 0);
    swap(dp, old);
    rep(i, 0, 8 * 8 * A1 * A2 * A3) {
      if (old[i] == 0) {
        continue;
      }
      auto [u1, u2, u3, l1, l2, l3, m1, m2, m3] = idx2state(i);
      rep(d1, 0, 2) {
        int nu1 = u1;
        int nl1 = l1;
        int nm1 = (m1 * 2 + d1) % A1;
        if (u1 == 0 && d1 > c) {
          continue;
        }
        if (u1 == 0 && d1 < c) {
          nu1 = 1;
        }
        if (l1 == 1 && d1 != 0) {
          nl1 = 0;
        }
        rep(d2, 0, 2) {
          int nu2 = u2;
          int nl2 = l2;
          int nm2 = (m2 * 2 + d2) % A2;
          if (u2 == 0 && d2 > c) {
            continue;
          }
          if (u2 == 0 && d2 < c) {
            nu2 = 1;
          }
          if (l2 == 1 && d2 != 0) {
            nl2 = 0;
          }
          rep(d3, 0, 2) {
            int nu3 = u3;
            int nl3 = l3;
            int nm3 = (m3 * 2 + d3) % A3;
            if (u3 == 0 && d3 > c) {
              continue;
            }
            if (u3 == 0 && d3 < c) {
              nu3 = 1;
            }
            if (l3 == 1 && d3 != 0) {
              nl3 = 0;
            }

            if ((d1 + d2 + d3) % 2 == 0) {
              dp[state2idx(nu1, nu2, nu3, nl1, nl2, nl3, nm1, nm2, nm3)] +=
                  old[i];
            }
          }
        }
      }
    }
  }
  mint ans = 0;
  rep(i, 0, 8 * 8 * A1 * A2 * A3) {
    auto [u1, u2, u3, l1, l2, l3, m1, m2, m3] = idx2state(i);
    if (l1 == 0 && l2 == 0 && l3 == 0 && m1 == 0 && m2 == 0 && m3 == 0) {
      ans += dp[i];
    }
  }
  cout << ans.val() << endl;
}