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

// ワイルドカードありの全一致
// 種類数

const int null_val = 1e9;

struct S {
  bool is_mono;
  int val;
};

S op(const S &a, const S &b) {
  S res;
  if (!a.is_mono || !b.is_mono) {
    res.is_mono = false;
    res.val = null_val;
  } else {
    if (a.val == b.val) {
      res.is_mono = true;
      res.val = a.val;
    } else if (a.val == -1) {
      res.is_mono = true;
      res.val = b.val;
    } else if (b.val == -1) {
      res.is_mono = true;
      res.val = a.val;
    } else {
      res.is_mono = false;
      res.val = null_val;
    }
  }
  return res;
}

S e() {
  return S{true, -1};
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
    seg.set(i, S{true, A[i]});
  }

  rep(q, 0, Q) {
    int t;
    cin >> t;
    if (t == 1) {
      int i, x;
      cin >> i >> x;
      i--;
      seg.set(i, S{true, x});
    } else {
      int l, r;
      cin >> l >> r;
      l--;
      S s = seg.prod(l, r);
      YN(s.is_mono);
    }
  }
}