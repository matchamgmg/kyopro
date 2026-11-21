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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<int> A(N), B(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  rep(i, 0, N) {
    cin >> B[i];
  }
  int l = 0;
  int r = N;
  while (l < N && A[l] == B[l]) {
    l++;
  }
  while (r > 0 && A[r - 1] == B[r - 1]) {
    r--;
  }
  if (l >= r) {
    cout << "Yes" << endl;
    return 0;
  }
  if (A[l] == B[r - 1] && A[r - 1] == B[l]) {
    bool ok1 = true;
    bool ok2 = true;
    rep(i, l, r - 1) {
      ok1 &= (A[i + 1] == B[i]);
    }
    rep(i, l, r - 1) {
      ok2 &= (A[i] == B[i + 1]);
    }
    YN(ok1 || ok2);
  } else if (A[l] == B[r - 1]) {
    bool ok = true;
    rep(i, l, r - 1) {
      ok &= (A[i + 1] == B[i]);
    }
    YN(ok);
  } else if (A[r - 1] == B[l]) {
    bool ok = true;
    rep(i, l, r - 1) {
      ok &= (A[i] == B[i + 1]);
    }
    YN(ok);
  } else {
    cout << "No" << endl;
  }
}