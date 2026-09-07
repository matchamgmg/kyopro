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

  ll N;
  cin >> N;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  // 各iについて選ぶ回数は，目標値をxをしたときに，xの一次式となる．
  // Bi : 一次式の定数項
  vector<ll> B(N, 0);
  // xの係数
  vector<ll> C(N, 0);
  rrep(i, 1, N + 1) {
    B[i - 1] -= A[i - 1];
    C[i - 1] = 1;
    for (ll j = 2 * i; j <= N; j += i) {
      B[i - 1] -= B[j - 1];
      C[i - 1] -= C[j - 1];
    }
  }
  const ll INF = 1e18;
  ll min_X = 0, max_X = INF;
  bool ok = true;
  rep(i, 0, N) {
    if (C[i] > 0) {
      chmax(min_X, (-B[i] + C[i] - 1) / C[i]);
    } else if (C[i] == 0) {
      ok &= (B[i] >= 0);
    } else {
      ok &= (B[i] >= 0);
      if (B[i] >= 0) {
        chmin(max_X, B[i] / (-C[i]));
      }
    }
  }
  if (!ok || max_X < min_X) {
    cout << -1 << endl;
  } else {
    cout << min_X - A[0] << endl;
  }
}