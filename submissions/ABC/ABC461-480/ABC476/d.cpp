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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, M, K;
  cin >> N >> M >> K;
  ll X, Y;
  cin >> X >> Y;
  ll sum_money = X + Y * K;
  vector<ll> A(N), B(M);
  rep(i, 0, N) {
    cin >> A[i];
  }
  rep(i, 0, M) {
    cin >> B[i];
  }
  // ドリンクの購入数を固定
  sort(all(A));
  vector<ll> SA(N + 1, 0);
  rep(i, 0, N) {
    SA[i + 1] = SA[i] + A[i];
  }
  sort(all(B));
  vector<ll> KA(M, 0), O(M);
  rep(i, 0, M) {
    KA[i] = (B[i] + K - 1) / K;
    O[i] = (K - B[i] % K) % K;
  }
  ll ans = 0;
  rep(i, 0, N) {
    ans = upper_bound(all(SA), sum_money) - SA.begin() - 1;
  }
  ll sum_K = 0, sum_O = 0;

  rep(i, 0, M) {
    sum_K += KA[i];
    sum_O += O[i];
    if (sum_K > Y) {
      break;
    }
    ll money = sum_money - sum_K * K + sum_O;
    ll r = upper_bound(all(SA), money) - SA.begin() - 1 + i + 1;
    chmax(ans, r);
  }
  cout << ans << endl;
}