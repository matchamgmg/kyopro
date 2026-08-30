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

  int N;
  cin >> N;
  string S;
  cin >> S;
  if (S[0] == 'x' || S[N - 1] == 'x') {
    cout << 0 << endl;
    return 0;
  }

  vector<mint> kaijou(N + 1, 1);
  rep(i, 0, N) { kaijou[i + 1] = kaijou[i] * (i + 1); }
  vector<mint> A(N + 1, 0);
  A[1] = 1;
  A[2] = 2;
  rep(i, 3, N + 1) {
    mint SA_kaijou = 0;
    rep(j, 2, i) { SA_kaijou += A[j] * kaijou[i - j + 1]; }
    A[i] = kaijou[i] - SA_kaijou;
  }

  mint ans = 1;
  int last = 0;
  rep(i, 1, N) {
    if (S[i] == 'o') {
      ans *= A[i - last + 1];
      last = i;
    }
  }
  cout << ans.val() << endl;
}