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

void solve() {
  ll N, K;
  cin >> N >> K;

  ll M = 62;
  const ll INF = 1e62;
  vector<mint> dp((K + 1) * 2, 0);
  vector<mint> dp2((K + 1) * 2, 0);
  dp2[0] = 1;
  rrep(m, 0, M) {
    vector<mint> old((K + 1) * 2, 0);
    vector<mint> old2((K + 1) * 2, 0);
    swap(dp, old);
    swap(dp2, old2);
    rep(i, 0, (K + 1) * 2) {
      if (old2[i] == 0) {
        continue;
      }
      int u = i / (K + 1);
      int k = i % (K + 1);
      rep(d, 0, 2) {
        int nu = u;
        int nk = k + d;
        if (u == 0) {
          if (!(N >> m & 1)) {
            if (d == 1) {
              continue;
            }
            dp[nu * (K + 1) + nk] += old[i] * 2;
            dp2[nu * (K + 1) + nk] += old2[i];
          } else {
            if (d == 0) {
              nu = 1;
            }
            if (nk < K + 1) {
              dp[nu * (K + 1) + nk] += old[i] * 2 + old2[i] * d;
              dp2[nu * (K + 1) + nk] += old2[i];
            }
          }
        } else {
          if (nk < K + 1) {
            dp[nu * (K + 1) + nk] += old[i] * 2 + old2[i] * d;
            dp2[nu * (K + 1) + nk] += old2[i];
          }
        }
      }
    }
  }
  mint ans = 0;
  rep(i, 0, (K + 1) * 2) {
    if (i % (K + 1) == K) {
      ans += dp[i];
    }
  }
  cout << ans.val() << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}