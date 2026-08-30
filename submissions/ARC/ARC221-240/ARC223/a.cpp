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

void solve() {
  int N;
  ll W;
  cin >> N >> W;
  vector<ll> w(N), v(N);
  rep(i, 0, N) { cin >> w[i] >> v[i]; }
  vector<ll> pre_w(N + 1, 0);
  rep(i, 0, N) { pre_w[i + 1] = pre_w[i] + w[i]; }
  ll ans = 0;
  ll sum_W = 0;
  ll sum_V = 0;
  int r = N;
  while (sum_W <= W) {
    int i = 0;

    ll weight = sum_W;
    ll val = sum_V;
    chmax(ans, val);
    while (i < r && weight + w[i] <= W) {
      val += v[i];
      weight += w[i];
      chmax(ans, val);
      ++i;
    }
    if (i < r && weight + w[i] > W && w[i] <= W) {
      chmax(ans, v[i]);
      sum_W += w[i];
      sum_V += v[i];
      r = i;
      continue;
    }
    if (i >= r) {
      chmax(ans, val);
      break;
    }
  }
  cout << ans << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}