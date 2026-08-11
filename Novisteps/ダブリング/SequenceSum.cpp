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

  ll N, X, M;
  cin >> N >> X >> M;
  vector<ll> A(M), DiffA(M);
  rep(i, 0, M) {
    A[i] = (ll)i * i % M;
    DiffA[i] = A[i];
  }
  vector<vector<ll>> doublingA, doublingDiffA;
  doublingA.emplace_back(A);
  doublingDiffA.emplace_back(DiffA);
  ll nibeki = 1;
  ll d = 0;
  N--;
  while (nibeki < N) {
    rep(i, 0, M) {
      A[i] = doublingA[d][doublingA[d][i]];
      DiffA[i] = doublingDiffA[d][i] + doublingDiffA[d][doublingA[d][i]];
    }
    doublingA.emplace_back(A);
    doublingDiffA.emplace_back(DiffA);
    nibeki <<= 1;
    d++;
  }
  ll ans = X;
  ll cur = X;
  d = 0;
  while (N) {
    if (N & 1) {
      ans += doublingDiffA[d][cur];
      cur = doublingA[d][cur];
    }
    d++;
    N >>= 1;
  }
  cout << ans << endl;
}