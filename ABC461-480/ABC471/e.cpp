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

  ll N, K;
  cin >> N >> K;
  vector<mint> A(N), AA(N), SA(N + 1, 0), SAA(N + 1, 0);
  rep(i, 0, N) {
    ll a;
    cin >> a;
    A[i] = a;
    AA[i] = A[i] * A[i];
    SA[i + 1] = SA[i] + A[i];
    SAA[i + 1] = SAA[i] + AA[i];
  }
  if (K == 1 || N == 1) {
    cout << SAA[N].val() << endl;
    return 0;
  }
  vector<mint> kaijou(N + 1, 1), inv(N + 1, 1);
  rep(i, 0, N) {
    kaijou[i + 1] = kaijou[i] * (i + 1);
    inv[i + 1] = inv[i] * mint(i + 1).inv();
  }
  mint NCK1 = kaijou[N - 1] * inv[K - 1] * inv[N - K];
  mint NCK2 = kaijou[N - 2] * inv[K - 2] * inv[N - K];
  mint ans1 = SAA[N] * NCK1;
  mint ans2 = 0;
  rep(i, 1, N) {
    ans2 += A[i - 1] * (SA[N] - SA[i]) * 2;
    // cout << (A[i - 1] * (SA[N] - SA[i])).val() << endl;
  }
  ans2 *= NCK2;
  cout << (ans1 + ans2).val() << endl;
}