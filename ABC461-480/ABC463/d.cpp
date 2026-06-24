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

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
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

  int N, K;
  cin >> N >> K;
  vector<ll> L(N), R(N);
  rep(i, 0, N) { cin >> L[i] >> R[i]; }
  vector<int> idx(N);
  iota(all(idx), 0);
  sort(all(idx), [&](int i, int j) { return R[i] == R[j] ? L[i] < L[j] : R[i] < R[j]; });

  ll ok = -1, ng = 1e9;
  const ll INF = 2e9;
  auto f = [&](ll m) -> bool {
    int cnt = 0;
    ll r = -INF;
    for (int i : idx) {
      if (r + m <= L[i]) {
        r = R[i];
        cnt++;
      }
    }
    return cnt >= K;
  };

  while (ng - ok > 1) {
    ll mid = (ok + ng) / 2;
    (f(mid) ? ok : ng) = mid;
  }
  cout << (ok == 0 ? -1 : ok) << endl;
}