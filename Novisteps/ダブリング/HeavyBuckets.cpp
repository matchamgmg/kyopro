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

  int N, Q;
  cin >> N >> Q;
  vector<ll> A(N), B(N);
  rep(i, 0, N) {
    cin >> A[i];
    A[i]--;
    B[i] = i + 1;
  }
  vector<vector<ll>> doublingA, doublingB;
  doublingA.emplace_back(A);
  doublingB.emplace_back(B);
  ll M = 1e9;
  ll nibeki = 1;
  ll d = 0;
  while (nibeki < M) {
    rep(i, 0, N) {
      A[i] = doublingA[d][doublingA[d][i]];
      B[i] = doublingB[d][i] + doublingB[d][doublingA[d][i]];
    }
    doublingA.emplace_back(A);
    doublingB.emplace_back(B);
    nibeki <<= 1;
    d++;
  }

  rep(q, 0, Q) {
    int t, b;
    cin >> t >> b;
    b--;
    d = 0;
    ll ans = 0;
    while (t) {
      if (t & 1) {
        ans += doublingB[d][b];
        b = doublingA[d][b];
      }
      d++;
      t >>= 1;
    }
    cout << ans << endl;
  }
}