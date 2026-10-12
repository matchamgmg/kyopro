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

const int INF = 1e9;

struct S {
  bool mono;
  int left_val;
  int left_len;
  int right_val;
  int right_len;
  int max_len;
};

S op(const S &a, const S &b) {
  S res;
  res.left_val = a.left_val;
  res.right_val = b.right_val;
  res.max_len = max(a.max_len, b.max_len);
  if (a.right_val == b.left_val) {
    res.max_len = max(res.max_len, a.right_len + b.left_len);
  }
  if (a.mono) {
    if (a.right_val == b.left_val) {
      res.left_len = a.left_len + b.left_len;
    } else {
      res.left_len = a.left_len;
    }
  } else {
    res.left_len = a.left_len;
  }
  if (b.mono) {
    if (a.right_val == b.left_val) {
      res.right_len = a.right_len + b.right_len;
    } else {
      res.right_len = b.right_len;
    }
  } else {
    res.right_len = b.right_len;
  }
  if (a.mono && b.mono && a.right_val == b.left_val) {
    res.mono = true;
  } else {
    res.mono = false;
  }
  return res;
}

S e() {
  return S{true, -1, 1, -1, 1, -INF};
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
    seg.set(i, S{true, A[i], 1, A[i], 1, 1});
  }
  rep(q, 0, Q) {
    int t, l, r;
    cin >> t >> l >> r;
    if (t == 1) {
      l--;
      seg.set(l, S{true, r, 1, r, 1, 1});
    } else {
      l--;
      cout << seg.prod(l, r).max_len << endl;
    }
  }
}