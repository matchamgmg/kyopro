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

ll gcd(ll a, ll b) {
  if (b == 0) {
    return a;
  }
  return gcd(b, a % b);
}

void solve() {
  struct Point {
    ll x, y;
  };
  vector<Point> P(4);
  rep(i, 0, 4) {
    cin >> P[i].x >> P[i].y;
  }
  ll kx1 = (P[0].y - P[1].y);
  ll ky1 = (P[1].x - P[0].x);
  ll kx2 = (P[2].y - P[3].y);
  ll ky2 = (P[3].x - P[2].x);
  ll g1 = gcd(kx1, ky1);
  ll g2 = gcd(kx2, ky2);
  kx1 /= g1;
  ky1 /= g1;
  kx2 /= g2;
  ky2 /= g2;
  if (!((kx1 == kx2 && ky1 == ky2) || (kx1 == -kx1 && ky1 == -ky2))) {
    cout << "Yes" << endl;
  } else {
    ll kx3 = (P[0].x + P[1].x) - (P[2].x + P[3].x);
    ll ky3 = (P[0].y + P[1].y) - (P[2].y + P[3].y);
    ll g3 = gcd(kx3, ky3);
    if (g3 == 0) {
      cout << "Yes" << endl;
    } else {
      kx3 /= g3;
      ky3 /= g3;
      if ((kx1 == kx3 && ky1 == ky3) || (kx1 == -kx3 && ky1 == -ky3)) {
        cout << "Yes" << endl;
      } else {
        cout << "No" << endl;
      }
    }
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