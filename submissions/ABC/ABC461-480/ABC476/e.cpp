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
  int x, y;
  int x_pos, y_pos;
};
S op(S a, S b) {
  S res;
  if (a.x < b.x) {
    res.x = a.x;
    res.x_pos = a.x_pos;
  } else {
    res.x = b.x;
    res.x_pos = b.x_pos;
  }

  if (a.y > b.y) {
    res.y = a.y;
    res.y_pos = a.y_pos;
  } else {
    res.y = b.y;
    res.y_pos = b.y_pos;
  }
  return res;
}
S e() {
  return S{INF, -INF, -1, -1};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  segtree<S, op, e> seg(N);
  rep(i, 0, N) {
    int p;
    cin >> p;
    seg.set(i, S{p, p, i, i});
  }
  rep(q, 0, M) {
    int l, r;
    cin >> l >> r;
    l--;
    S s = seg.prod(l, r);
    seg.set(s.x_pos, S{s.y, s.y, s.x_pos, s.x_pos});
    seg.set(s.y_pos, S{s.x, s.x, s.y_pos, s.y_pos});
  }
  rep(i, 0, N) {
    cout << seg.get(i).x << ' ';
  }
  cout << endl;
}