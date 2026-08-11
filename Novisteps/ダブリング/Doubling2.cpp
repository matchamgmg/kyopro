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

  int N, Q;
  cin >> N >> Q;
  vector<int> A(N); // 一手先
  for (int i = 0; i < N; i++) {
    cin >> A[i];
    A[i]--;
  }
  // ダブリングテーブルを作る
  vector<vector<int>> dp; // dp[d][i] : 今巣iにいるとき，2^d手あとの場所
  dp.emplace_back(A);
  // dp[0][i] : 1手後
  ll nibeki = 1;
  ll d = 0; // 2^d = nibeki
  ll Y_max = 1e9;
  while (nibeki < Y_max) {
    vector<int> next_dp(N);
    for (int i = 0; i < N; i++) {
      next_dp[i] = dp.back()[dp.back()[i]];
    }
    dp.emplace_back(next_dp); // dp[d+1][i]が作られる
    nibeki <<= 1;
    d++;
  }

  for (int q = 0; q < Q; ++q) {
    int X, Y;
    cin >> X >> Y;
    X--;
    d = 0;
    while (Y) {
      if (Y & 1) {
        X = dp[d][X];
      }
      d++;
      Y >>= 1;
    }
    cout << X + 1 << endl;
  }
}