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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, K;
  cin >> N >> K;
  vector<ll> A(N), B(N), C(N);
  ll pre_ans = 0;
  rep(i, 0, N) {
    cin >> A[i] >> B[i];
    C[i] = B[i] - A[i];
    pre_ans += A[i];
  }

  // 何番目のカードを見ているか
  // i番目のカードを選んだか (ひっくり返す)
  // 何個の区間を選んだか
  const ll INF = 1e18;
  vector<vector<vector<ll>>> dp(N + 1, vector<vector<ll>>(2, vector<ll>(K + 1, -INF)));
  dp[0][1][0] = 0;
  rep(i, 0, N) {
    // i番目のカードを選ばないとき
    rep(j, 0, K + 1) {
      chmax(dp[i + 1][1][j], dp[i][0][j]);
      chmax(dp[i + 1][1][j], dp[i][1][j]);
    }
    // i番目のカードを選ぶとき
    rep(j, 0, K) { chmax(dp[i + 1][0][j + 1], dp[i][1][j] + C[i]); }
    rep(j, 0, K + 1) { chmax(dp[i + 1][0][j], dp[i][0][j] + C[i]); }
  }
  ll ans = -INF;
  rep(j, 0, K + 1) {
    chmax(ans, dp[N][0][j]);
    chmax(ans, dp[N][1][j]);
  }
  cout << pre_ans + ans << endl;
}