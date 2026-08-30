/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <bits/stdc++.h>

#include <atcoder/all>
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

struct T {
  int left_B;
  int right_A;
};

T op(T a, T b) {
  if (a.right_A == b.left_B) {
    return T{a.left_B, b.right_A};
  }
  int d = min(a.right_A, b.left_B);
  if (d == a.right_A) {
    return T{a.left_B + b.left_B - d, b.right_A};
  } else {
    return T{a.left_B, a.right_A - d + b.right_A};
  }
}

T e() {
  return T{0, 0};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  segtree<T, op, e> seg(N);
  string S;
  cin >> S;
  rep(i, 0, N) {
    if (S[i] == 'A') {
      seg.set(i, {0, 1});
    } else {
      seg.set(i, {1, 0});
    }
  }
  int Q;
  cin >> Q;
  rep(q, 0, Q) {
    int t;
    cin >> t;
    if (t == 1) {
      int i;
      char c;
      cin >> i >> c;
      i--;
      if (c == 'A') {
        seg.set(i, {0, 1});
      } else {
        seg.set(i, {1, 0});
      }
    } else {
      int l, r;
      cin >> l >> r;
      l--;
      T s = seg.prod(l, r);
      YN(s.left_B == 0);
    }
  }
}