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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<int> A(N);
  rep(i, 0, N) { cin >> A[i]; }
  vector<int> R(Q), X(Q), qidx(Q);
  rep(i, 0, Q) {
    cin >> R[i] >> X[i];
    qidx[i] = i;
  }
  sort(all(qidx), [&](int i, int j) { return R[i] < R[j]; });
  int LIS = 0;
  const int INF = 2e9;
  vector<int> L(N + 1, INF);
  vector<int> ans(Q);
  L[0] = -INF;
  int i = 0;
  for (int q : qidx) {
    while (i < N && i < R[q]) {
      int j = lower_bound(all(L), A[i]) - L.begin();
      chmax(LIS, j);
      L[j] = A[i];
      ++i;
    }
    int k = upper_bound(all(L), X[q]) - L.begin();
    ans[q] = k - 1;
  }
  rep(i, 0, Q) { cout << ans[i] << endl; }
}