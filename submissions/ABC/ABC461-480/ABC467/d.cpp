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

// 垂直二等分線が共有点を持つ

ll gcd(ll x, ll y) {
  if (y == 0) {
    return x;
  }
  return gcd(y, x % y);
}

void solve() {
  ll Px, Py, Qx, Qy, Rx, Ry, Sx, Sy;
  cin >> Px >> Py >> Qx >> Qy >> Rx >> Ry >> Sx >> Sy;

  ll a1 = -Px + Qx;
  ll b1 = -Py + Qy;
  ll c1 = Px * Px + Py * Py - Qx * Qx - Qy * Qy;
  ll a2 = -Rx + Sx;
  ll b2 = -Ry + Sy;
  ll c2 = Rx * Rx + Ry * Ry - Sx * Sx - Sy * Sy;
  if (a1 * b2 == a2 * b1) {
    ll Nx = Rx + Sx;
    ll Ny = Ry + Sy;
    ll x = a1 * Nx + b1 * Ny + c1;
    if (x == 0) {
      cout << "Yes" << endl;
    } else {
      cout << "No" << endl;
    }
  } else {
    cout << "Yes" << endl;
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