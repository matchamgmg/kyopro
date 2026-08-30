#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

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

void solve() {
  int N;
  cin >> N;
  vector<int> A(N);
  rep(i, 0, N) { cin >> A[i]; }
  int LIS = 0;
  const int INF = 2e9;
  vector<int> L1(N + 1, INF);
  L1[0] = -INF;
  vector<int> left(N, 0);
  rep(i, 0, N) {
    int j = lower_bound(all(L1), A[i]) - L1.begin();
    chmax(LIS, j);
    L1[j] = A[i];
    left[i] = j;
  }
  vector<int> L2(N + 1, -INF);
  L2[0] = INF;
  vector<int> right(N, 0);
  int LDS = 0;
  rrep(i, 0, N) {
    int j =
        lower_bound(all(L2), A[i], [&](int e, int val) { return e > val; }) -
        L2.begin();
    chmax(LDS, j);
    L2[j] = A[i];
    right[i] = j;
  }

  vector<bool> ans(N, false);
  int cnt = 0;
  rep(i, 0, N) {
    if (left[i] + right[i] == LIS + 1) {
      cnt++;
      ans[i] = true;
    }
  }

  cout << cnt << endl;
  rep(i, 0, N) {
    if (ans[i]) {
      cout << i + 1 << ' ';
    }
  }
  cout << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}