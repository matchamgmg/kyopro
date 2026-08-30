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

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x) { cout << (x ? "Yes" : "No") << endl; }
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
bool grid_check(int x, int y, int X, int Y) { return (0 <= x && x < X && 0 <= y && y < Y); }
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

const ll INF = 1e18;

struct S {
  ll min_RR;
  ll max_RR;
  S() {}
  S(ll a, ll b) : min_RR(a), max_RR(b) {}
};

S op(S a, S b) { return S{min(a.min_RR, b.min_RR), max(a.max_RR, b.max_RR)}; }

S e() { return S{INF, -INF}; }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<ll> X(N), Y(N);
  vector<S> XX(N), YY(N);
  rep(i, 0, N) {
    cin >> X[i] >> Y[i];
    XX[i] = {X[i] + Y[i], X[i] + Y[i]};
    YY[i] = {X[i] - Y[i], X[i] - Y[i]};
  }

  segtree<S, op, e> seg_XX(XX), seg_YY(YY);

  int t, i, L, R;
  ll x, y;
  rep(q, 0, Q) {
    cin >> t;
    if (t == 1) {
      cin >> i >> x >> y;
      i--;
      ll xx = x + y;
      ll yy = x - y;
      seg_XX.set(i, {xx, xx});
      seg_YY.set(i, {yy, yy});
    } else {
      cin >> L >> R >> x >> y;
      ll xx = x + y;
      ll yy = x - y;
      L--;
      S min_max_XX = seg_XX.prod(L, R);
      S min_max_YY = seg_YY.prod(L, R);
      ll ans = -INF;
      chmax(ans, abs(xx - min_max_XX.max_RR));
      chmax(ans, abs(xx - min_max_XX.min_RR));
      chmax(ans, abs(yy - min_max_YY.max_RR));
      chmax(ans, abs(yy - min_max_YY.min_RR));
      cout << ans << endl;
    }
  }
}