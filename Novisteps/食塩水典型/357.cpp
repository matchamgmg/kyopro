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
  vector<ll> X(3);
  rep(i, 0, 3) {
    cin >> X[i];
  }
  //   sort(all(X));
  //   vector<ll> R(3);
  //   rep(i, 0, 3) {
  //     R[i] = X[i] % 6;
  //   }
  //   if (!(R[0] % 2 == R[1] % 2 && R[1] % 2 == R[2] % 2)) {
  //     cout << -1 << endl;
  //     return;
  //   }
  //   if ((R[0] + R[1] + R[2]) % 3 != 0) {
  //     cout << -1 << endl;
  //     return;
  //   }
  //   R[1] -= R[0];
  //   if (R[1] < 0) {
  //     R[1] += 6;
  //   }
  //   R[2] -= R[0];
  //   if (R[2] < 0) {
  //     R[2] += 6;
  //   }
  //   R[0] = 0;
  //   ll ans = 0;
  //   if (R[1] == 2) {
  //     X[0] += 7;
  //     X[1] += 5;
  //     X[2] += 3;
  //     ans++;
  //   } else if (R[1] == 4) {
  //     X[0] += 14;
  //     X[1] += 10;
  //     X[2] += 6;
  //     ans += 2;
  //   }
  //   vector<ll> A(3);
  //   rep(i, 0, 3) {
  //     A[i] = X[i] / 6;
  //   }
  //   A[1] -= A[0];
  //   A[2] -= A[0];
  //   A[0] = 0;
  //   ll x = min(A[2] - A[1], A[1] - A[0]);
  //   A[1] -= x;
  //   A[2] -= 2 * x;
  //   ans += 3 * x;
  //   ll y = max(A[2] - A[1], A[1] - A[0]);
  //   ans += 2 * y;
  //   cout << ans << endl;

  if ((X[0] + X[1] + X[2]) % 3 != 0) {
    cout << -1 << endl;
    return;
  }
  if (!(X[0] % 2 == X[1] % 2 && X[1] % 2 == X[2] % 2)) {
    cout << -1 << endl;
    return;
  }
  ll a = (X[0] + X[1] + X[2]) / 3;
  ll d = abs(X[0] - a) + abs(X[1] - a) + abs(X[2] - a);
  cout << d / 4 << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}