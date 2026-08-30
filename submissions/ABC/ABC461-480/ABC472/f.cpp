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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, Q;
  cin >> N >> Q;
  vector<ll> X(2 * N), Y(2 * N);
  rep(i, 0, N) {
    cin >> X[i] >> Y[i];
    X[i + N] = X[i];
    Y[i + N] = Y[i];
  }

  // 三角形(O, k, k+1) の符号付面積
  vector<ll> cross(2 * N), Scross(2 * N + 1, 0);
  rep(i, 0, N) {
    cross[i] = X[i] * Y[i + 1] - X[i + 1] * Y[i];
    cross[i + N] = cross[i];
  }
  rep(i, 0, 2 * N) {
    Scross[i + 1] = Scross[i] + cross[i];
  }
  // 三角形(O, k, k+1) の重み付き重心
  vector<ll> GX(2 * N), GY(2 * N), SGX(2 * N + 1, 0), SGY(2 * N + 1);
  rep(i, 0, N) {
    GX[i] = (X[i] + X[i + 1]) * cross[i];
    GY[i] = (Y[i] + Y[i + 1]) * cross[i];
    GX[i + N] = GX[i];
    GY[i + N] = GY[i];
  }
  rep(i, 0, 2 * N) {
    SGX[i + 1] = SGX[i] + GX[i];
    SGY[i + 1] = SGY[i] + GY[i];
  }

  // 三角形(O, u, v) の符号付面積
  auto calc_space = [&](int u, int v) -> tuple<ll, ll, ll> {
    ll res_space = X[u] * Y[v] - X[v] * Y[u];
    ll res_x = res_space * (X[u] + X[v]);
    ll res_y = res_space * (Y[u] + Y[v]);
    return {res_space, res_x, res_y};
  };

  cout << fixed << setprecision(10);

  rep(q, 0, Q) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    if (u > v) {
      v += N;
    }
    auto [s, x, y] = calc_space(v, u);
    double space_sum = Scross[v] - Scross[u] + s;
    double ans_x = SGX[v] - SGX[u] + x;
    double ans_y = SGY[v] - SGY[u] + y;
    cout << ans_x / space_sum / 3 << ' ' << ans_y / space_sum / 3 << endl;
  }
}