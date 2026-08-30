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

struct S {
  ll sum;
  ll max_sum;
};
const ll INF = 8e18;
S e() {
  return S{0, -INF};
}
S op(S a, S b) {
  return S{a.sum + b.sum, max(a.max_sum, a.sum + b.max_sum)};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, B, Q;
  cin >> N >> B >> Q;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
    A[i] -= B;
  }

  segtree<S, op, e> seg(N);
  ll sum = 0;
  ll max_sum = -INF;
  rep(i, 0, N) {
    sum += A[i];
    chmax(max_sum, sum);
    seg.set(i, {A[i], A[i]});
  }

  rep(q, 0, Q) {
    ll c, x;
    cin >> c >> x;
    c--;
    x -= B;
    seg.set(c, {x, x});

    int r = seg.max_right(0, [&](S e) {
      return e.max_sum < 0;
    });
    cout << fixed << setprecision(11);
    if (r == N) {
      cout << (double)(seg.prod(0, N).sum + N * B) / N << endl;
    } else {
      cout << (double)(seg.prod(0, r + 1).sum + (r + 1) * B) / (r + 1) << endl;
    }
  }
}