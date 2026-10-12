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

// [l, r) において最後に出現する0までの区間和 mod 998244353 + 0以降の区間和
// 最後に出現する0の位置を求める : 0の位置を管理する

ll op(ll a, ll b) {
  return a + b;
}
ll e() {
  return 0;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  segtree<ll, op, e> seg1(N), seg2(N);
  rep(i, 0, N) {
    seg1.set(i, A[i]);
    if (A[i] == 0) {
      seg2.set(i, 1);
    }
  }
  rep(q, 0, Q) {
    int t;
    cin >> t;
    if (t == 1) {
      ll i, x;
      cin >> i >> x;
      i--;
      seg1.set(i, x);
      if (x == 0) {
        seg2.set(i, 1);
      } else {
        seg2.set(i, 0);
      }
    } else {
      ll l, r, x;
      cin >> l >> r >> x;
      l--;
      ll zero = seg2.min_left(r, [&](int s) {
        return s < 1;
      });
      zero--;
      if (zero >= l) {
        x = (x + seg1.prod(l, zero)) % 998244353;
      }
      x += seg1.prod(max(l, zero), r);
      cout << x << endl;
    }
  }
}