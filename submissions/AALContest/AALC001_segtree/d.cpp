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

// 転倒数
// prefixの2の個数
// prefixの1の個数を持つ
struct S {
  ll prefix0;
  ll prefix1;
  ll prefix2;
  ll tumbling;
};

S op(const S &a, const S &b) {
  S res;
  res.tumbling = a.tumbling + b.tumbling;
  res.tumbling += b.prefix0 * (a.prefix1 + a.prefix2);
  res.tumbling += b.prefix1 * a.prefix2;
  res.prefix0 = a.prefix0 + b.prefix0;
  res.prefix1 = a.prefix1 + b.prefix1;
  res.prefix2 = a.prefix2 + b.prefix2;
  return res;
}

S e() {
  return S{0, 0, 0, 0};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<int> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  segtree<S, op, e> seg(N);
  rep(i, 0, N) {
    if (A[i] == 0) {
      seg.set(i, S{1, 0, 0, 0});
    } else if (A[i] == 1) {
      seg.set(i, S{0, 1, 0, 0});
    } else {
      seg.set(i, S{0, 0, 1, 0});
    }
  }
  rep(q, 0, Q) {
    int t, l, r;
    cin >> t >> l >> r;
    if (t == 1) {
      l--;
      if (r == 0) {
        seg.set(l, S{1, 0, 0, 0});
      } else if (r == 1) {
        seg.set(l, S{0, 1, 0, 0});
      } else {
        seg.set(l, S{0, 0, 1, 0});
      }
    } else {
      l--;
      cout << seg.prod(l, r).tumbling << endl;
    }
  }
}