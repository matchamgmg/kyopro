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

void solve() {
  int N;
  cin >> N;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  sort(all(A));
  vector<ll> B(N);
  B[0] = A[N - 1];
  B[N - 1] = A[N - 2];
  vector<ll> O, E;
  rep(i, 0, N - 2) {
    (A[i] % 2 == 0 ? E : O).push_back(A[i]);
  }
  if (E.size() < O.size()) {
    ll ans = 1e18;
    ll res = 0;
    int s = 1;
    rep(i, 0, E.size()) {
      B[s++] = E[i];
      B[s++] = O[i];
    }
    rep(i, E.size(), O.size()) {
      B[s++] = O[i];
    }
    rep(i, 0, N - 1) {
      res += (B[i] + B[i + 1]) / 2;
    }
    chmin(ans, res);
    res = 0;
    s = 1;
    rep(i, 0, E.size()) {
      B[s++] = O[i];
      B[s++] = E[i];
    }
    rep(i, 0, N - 1) {
      res += (B[i] + B[i + 1]) / 2;
    }
    chmin(ans, res);
    cout << ans << endl;
  } else {
    ll ans = 1e18;
    ll res = 0;
    int s = 1;
    rep(i, 0, O.size()) {
      B[s++] = E[i];
      B[s++] = O[i];
    }
    rep(i, O.size(), E.size()) {
      B[s++] = E[i];
    }
    rep(i, 0, N - 1) {
      res += (B[i] + B[i + 1]) / 2;
    }
    chmin(ans, res);
    res = 0;
    s = 1;
    rep(i, 0, O.size()) {
      B[s++] = O[i];
      B[s++] = E[i];
    }
    rep(i, 0, N - 1) {
      res += (B[i] + B[i + 1]) / 2;
    }
    chmin(ans, res);
    cout << ans << endl;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}