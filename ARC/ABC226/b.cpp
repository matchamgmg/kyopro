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
void solve() {
  int N, M;
  cin >> N >> M;
  vector<int> A(M);
  rep(i, 0, M) { cin >> A[i]; }

  ll max_height = 0;
  ll baseline = 0;
  ll space = N;
  rrep(i, 0, M) {
    if (max_height > baseline) {
      if (A[i] >= space) {
        A[i] -= space;
        space = N;
        baseline = max_height;
      } else {
        space -= A[i];
        space <<= 1;
        A[i] = 0;
      }
    }
    if (max_height == baseline) {
      if (A[i] >= space) {
        ll d = A[i] / space;
        A[i] -= d * space;
        max_height += (1LL << i) * d;
        baseline = max_height;
      }
      if (A[i] > 0) {
        max_height += (1LL << i);
        space -= A[i];
        space <<= 1;
        A[i] = 0;
      }
    }
  }
  cout << max_height << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}