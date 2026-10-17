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

ll g(ll N) {
  ll ok = 0, ng = 1e6 + 1;
  while (ng - ok > 1) {
    ll mid = (ok + ng) / 2;
    ((mid * mid * mid) <= N ? ok : ng) = mid;
  }
  return ok + 1;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  ll N;
  cin >> N;
  ll ans = 0;
  ll root3N = g(N);
  for (ll l = 3; l <= 20; ++l) {
    ll ok = 0, ng = N;
    auto f = [&](ll m, ll s) -> bool {
      ll res = 1;
      double lg = 0;
      rep(i, 0, s) {
        res *= (m + i);
        lg += log10((double)(m + i));
      }
      if (lg > 18.) {
        return false;
      }
      return res <= N;
    };
    while (ng - ok > 1) {
      ll mid = (ok + ng) / 2;
      (f(mid) ? ok : ng) = mid;
    }
    for (ll d = 1; d <= root3N; ++d) {
      ans += max(0LL, ok + l);
    }
  }
}