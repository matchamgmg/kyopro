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

struct S {
  mint A1, A2, A3, A4;
  mint siz;
};
struct F {
  mint x1, x2, x3, x4;
};
S op(const S &a, const S &b) {
  return S{a.A1 + b.A1, a.A2 + b.A2, a.A3 + b.A3, a.A4 + b.A4, a.siz + b.siz};
}
S e() {
  return S{0, 0, 0, 0, 0};
}
S mapping(const F &f, const S &x) {
  S res;
  res.A1 = x.A1 + f.x1 * x.siz;
  res.A2 = x.A2 + 2 * x.A1 * f.x1 + f.x2 * x.siz;
  res.A3 = x.A3 + 3 * x.A2 * f.x1 + 3 * x.A1 * f.x2 + f.x3 * x.siz;
  res.A4 = x.A4 + 4 * x.A3 * f.x1 + 6 * x.A2 * f.x2 + 4 * x.A1 * f.x3 + f.x4 * x.siz;
  res.siz = x.siz;
  return res;
}
F composition(const F &f, const F &g) {
  F res;
  res.x1 = g.x1 + f.x1;
  res.x2 = g.x2 + 2 * g.x1 * f.x1 + f.x2;
  res.x3 = g.x3 + 3 * g.x2 * f.x1 + 3 * g.x1 * f.x2 + f.x3;
  res.x4 = g.x4 + 4 * g.x3 * f.x1 + 6 * g.x2 * f.x2 + 4 * g.x1 * f.x3 + f.x4;
  return res;
}
F id() {
  return F{0, 0, 0, 0};
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
  lazy_segtree<S, op, e, F, mapping, composition, id> seg(N);
  rep(i, 0, N) {
    mint x = A[i];
    seg.set(i, S{x, x.pow(2), x.pow(3), x.pow(4), 1});
  }

  rep(q, 0, Q) {
    int t, l, r, x;
    cin >> t >> l >> r >> x;
    if (t == 1) {
      l--;
      mint a = x;
      seg.apply(l, r, F{a, a.pow(2), a.pow(3), a.pow(4)});
      // vector<S> vs(N);
      // rep(i, 0, N) {
      //   vs[i] = seg.get(i);
      // }
      // rep(i, 0, N) {
      //   cout << vs[i].A1.val() << ' ';
      // }
      // cout << endl;
    } else {
      l--;
      S s = seg.prod(l, r);
      if (x == 1) {
        cout << s.A1.val() << endl;
      } else if (x == 2) {
        cout << s.A2.val() << endl;
      } else if (x == 3) {
        cout << s.A3.val() << endl;
      } else {
        cout << s.A4.val() << endl;
      }
    }
  }
}