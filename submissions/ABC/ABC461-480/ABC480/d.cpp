/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>;               // 大きい順
template <class T> using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = (s); i < (int)(n); i++)
#define rrep(i, s, n) for (int i = (n - 1); i >= (int)(s); i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
void pyes() {
  cout << "Yes" << endl;
}
void pno() {
  cout << "No" << endl;
}
void YN(bool x) {
  cout << (x ? "Yes" : "No") << endl;
}
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

  ll N, Q;
  cin >> N >> Q;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }

  vector<ll> SA(N + 1, 0);
  rep(i, 0, N) {
    SA[i + 1] = SA[i] + A[i];
  }
  vector<ll> SSA(N + 1, 0);
  rep(i, 0, N) {
    SSA[i + 1] = SSA[i] + SA[i + 1];
  }

  auto solve = [&](ll K) -> ll {
    ll ok = 0, ng = N + 1;
    auto f = [&](ll m, ll x) -> bool {
      return m * (m + 1) < x * 2;
    };
    while (ng - ok > 1) {
      ll mid = (ok + ng) / 2;
      (f(mid, K) ? ok : ng) = mid;
    }

    ll res = SSA[ok];
    ll rem = K - (ok * (ok + 1) / 2);
    res += SA[rem];
    return res;
  };

  rep(q, 0, Q) {
    ll L, R;
    cin >> L >> R;
    cout << solve(R) - solve(L - 1) << endl;
  }
}