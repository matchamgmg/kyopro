/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

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

int op_min(int a, int b) {
  return min(a, b);
}
int op_max(int a, int b) {
  return max(a, b);
}
int e_min() {
  return 1e9;
}
int e_max() {
  return -1e9;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  cin >> N >> K;
  vector<int> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }

  segtree<int, op_min, e_min> seg_min(N);
  segtree<int, op_max, e_max> seg_max(N);
  rep(i, 0, N) {
    seg_min.set(i, A[i]);
    seg_max.set(i, A[i]);
  }
  vector<bool> left(N + 1, true), right(N + 1, true);
  int last = 0;
  rep(i, 0, N) {
    left[i + 1] = (left[i] && (A[i] >= last));
    last = A[i];
  }
  last = N + 1;
  rrep(i, 0, N) {
    right[i] = (right[i + 1] && (A[i] <= last));
    last = A[i];
  }
  bool ans = false;
  rep(i, 0, N - K + 1) {
    ans |= (left[i] && right[i + K] && (seg_max.prod(0, i) <= seg_min.prod(i, i + K)) &&
            (seg_max.prod(i, i + K) <= seg_min.prod(i + K, N)));
  }
  YN(ans);
}