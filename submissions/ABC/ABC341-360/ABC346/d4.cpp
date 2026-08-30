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

  int N;
  cin >> N;
  string S;
  cin >> S;
  vector<ll> C(N);
  rep(i, 0, N) { cin >> C[i]; }
  vector<ll> cost_01(N + 1, 0), cost_10(N + 1, 0);
  rep(i, 0, N) {
    if (i % 2 == 0 && S[i] == '0' || i % 2 == 1 && S[i] == '1') {
      cost_01[i + 1] = cost_01[i];
      cost_10[i + 1] = cost_10[i] + C[i];
    } else {
      cost_01[i + 1] = cost_01[i] + C[i];
      cost_10[i + 1] = cost_10[i];
    }
  }

  const ll INF = 1e18;
  ll ans = INF;
  rep(i, 1, N) {
    chmin(ans, cost_01[i] + cost_10[N] - cost_10[i]);
    chmin(ans, cost_10[i] + cost_01[N] - cost_01[i]);
  }
  cout << ans << endl;
}