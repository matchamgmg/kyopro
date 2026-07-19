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

  int N, M;
  cin >> N >> M;
  vector<int> A(N), B(N - 1);
  rep(i, 0, N) { cin >> A[i]; }
  rep(i, 0, N - 1) { cin >> B[i]; }
  const int INF = 1e9;
  vector<vector<int>> dp(N, vector<int>(M, INF));
  dp[0][0] = 0;
  dp[0][1] = 1;
  rep(i, 0, N - 1) {
    int m = (B[i] - (A[i] + A[i + 1]) % M + M) % M;
    if (m == 0) {
      chmin(dp[i + 1][0], dp[i][0]);
      chmin(dp[i + 1][1], dp[i][1] + 1);
    } else if (m == 1) {
      chmin(dp[i + 1][0], dp[i][1]);
      chmin(dp[i + 1][1], dp[i][0] + 1);
    }
  }
  cout << min(dp[N - 1][0], dp[N - 1][1]) << endl;
}