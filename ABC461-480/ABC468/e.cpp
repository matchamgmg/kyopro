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
  ll N;
  cin >> N;
  vector<int> A(N);
  rep(i, 0, N) { cin >> A[i]; }
  vector<mint> B(N);
  rep(i, 0, N) { B[i] = A[i]; }
  vector<mint> S(N + 1, 0);
  rep(i, 0, N) { S[i + 1] = S[i] + B[i]; }
  vector<mint> SS(N + 1);
  SS[1] = S[N];
  SS[N] = S[N];
  rep(i, 1, N + 1 / 2) {
    mint a = S[N - i] - S[i];
    SS[i + 1] = SS[i] + a;
    SS[N - i] = SS[N - i + 1] + a;
  }
  vector<mint> inv_K(N + 1, 0);
  rep(i, 1, N + 1) { inv_K[i] = mint(i).inv(); }

  mint ans = 0;
  rep(w, 1, N + 1) { ans += SS[w] * inv_K[w]; }
  cout << ans.val() << endl;
}