#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

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

  ll N, K;
  cin >> N >> K;
  vector<ll> P(N), C(N);
  rep(i, 0, N) {
    cin >> P[i];
    P[i]--;
  }
  rep(i, 0, N) {
    cin >> C[i];
  }

  const ll INF = 9e18;
  auto solve = [&](int s) -> ll {
    vector<bool> seen(N, false);
    ll cycle_score = 0;
    ll cycle_length = 0;
    ll res = -INF;
    {
      int u = s;
      while (!seen[u]) {
        seen[u] = true;
        int v = P[u];
        cycle_length++;
        cycle_score += C[u];
        u = v;
      }
    }
    ll k = K;
    ll score = 0;
    if (cycle_score >= 0) {
      ll d = k / cycle_length;
      d = max(d - 1, 0LL);
      k -= d * cycle_length;
      score += d * cycle_score;
      chmax(res, score);
      int u = s;
      while (k) {
        score += C[u];
        chmax(res, score);
        u = P[u];
        k--;
      }
    } else {
      k = min(k, cycle_length);
      int u = s;
      while (k) {
        score += C[u];
        chmax(res, score);
        u = P[u];
        k--;
      }
    }
    return res;
  };

  ll ans = -INF;
  rep(i, 0, N) {
    chmax(ans, solve(i));
  }
  cout << ans << endl;
}