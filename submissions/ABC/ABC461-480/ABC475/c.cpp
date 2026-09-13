/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

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

  ll N, S, L;
  cin >> N >> S >> L;
  vector<ll> A(N - 1), SA(N, 0);
  rep(i, 0, N - 1) {
    cin >> A[i];
    SA[i + 1] = SA[i] + A[i];
  }
  int ans = 0;
  int r = S - 1;
  rep(i, 0, S) {
    ll dist = SA[S - 1] - SA[i];
    dist *= 2;
    if (dist > L) {
      continue;
    }
    while (r < N && dist + SA[r] - SA[S - 1] <= L) {
      ++r;
    }
    chmax(ans, r - i);
  }
  int l = S - 1;
  rrep(i, S - 1, N) {
    ll dist = SA[i] - SA[S - 1];
    dist *= 2;
    if (dist > L) {
      continue;
    }
    while (0 <= l && dist + SA[S - 1] - SA[l] <= L) {
      --l;
    }
    chmax(ans, i - l);
  }
  cout << ans << endl;
}