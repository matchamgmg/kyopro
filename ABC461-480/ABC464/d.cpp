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

void solve() {
  int N;
  cin >> N;
  string S;
  cin >> S;
  vector<ll> X(N), Y(N - 1);
  rep(i, 0, N) { cin >> X[i]; }
  rep(i, 0, N - 1) { cin >> Y[i]; }

  vector<vector<ll>> dp(N + 1, vector<ll>(2, -INF)); // S, R
  dp[0][0] = dp[0][1] = 0;
  rep(i, 0, N) {
    if (i == 0) {
      if (S[i] == 'S') {
        chmax(dp[i + 1][0], 0LL);
        chmax(dp[i + 1][1], -X[i]);
      } else {
        chmax(dp[i + 1][0], -X[i]);
        chmax(dp[i + 1][1], 0LL);
      }
    } else {
      if (S[i] == 'S') {
        chmax(dp[i + 1][0], dp[i][0]);
        chmax(dp[i + 1][0], dp[i][1] + Y[i - 1]);
        chmax(dp[i + 1][1], max(dp[i][0], dp[i][1]) - X[i]);
      } else {
        chmax(dp[i + 1][0], dp[i][0] - X[i]);
        chmax(dp[i + 1][0], dp[i][1] + Y[i - 1] - X[i]);
        chmax(dp[i + 1][1], max(dp[i][0], dp[i][1]));
      }
    }
  }
  cout << max(dp[N][0], dp[N][1]) << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}