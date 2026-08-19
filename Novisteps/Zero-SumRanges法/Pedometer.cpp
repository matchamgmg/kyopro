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

ll GCD(ll x, ll y) {
  if (y == 0) {
    return x;
  }
  if (x % y == 0) {
    return y;
  }
  return GCD(y, x % y);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  ll N, M;
  cin >> N >> M;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  ll L = 0;
  rep(i, 0, N) {
    L += A[i];
  }
  vector<ll> cnt(M, 0);
  ll sum = 0;
  cnt[sum]++;
  ll ans = 0;
  rep(i, 0, 2 * N - 1) {
    sum += A[i % N];
    sum %= M;
    if (i >= N - 1) {
      cnt[(sum + max(0LL, (L - sum + M - 1) / M) * M - L) % M]--;
      ans += cnt[sum];
    }
    cnt[sum]++;
  }
  cout << ans << endl;
}