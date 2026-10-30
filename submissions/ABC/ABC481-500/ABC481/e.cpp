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

ll cross(const pair<ll, ll> &a, const pair<ll, ll> &b) {
  return a.first * b.second - a.second * b.first;
}

bool up(const pair<ll, ll> &a) {
  return a.second > 0 || (a.second == 0 && a.first >= 0);
}

// return argument(a) < argument(b)
bool comp(const pair<ll, ll> &a, const pair<ll, ll> &b) {
  int aup = up(a);
  int bup = up(b);
  if (aup != bup)
    return aup > bup;
  return cross(a, b) > 0;
}

void solve() {
  ll N;
  cin >> N;
  vector<pair<ll, ll>> P(N);
  rep(i, 0, N) {
    cin >> P[i].first >> P[i].second;
  }
  sort(all(P), comp);
  vector<pair<ll, ll>> L = P;
  vector<pair<ll, pair<ll, ll>>> R;

  pair<ll, ll> A = {-1, 0};
  int l = lower_bound(L.begin(), L.end(), A,
                      [&](const pair<ll, ll> &e, const pair<ll, ll> &v) {
                        return comp(e, v);
                      }) -
          L.begin();
  rep(i, l, N) {
    R.emplace_back(1, P[i]);
  }
  rep(i, 0, l) {
    R.emplace_back(-1, pair<ll, ll>{-P[i].first, -P[i].second});
  }
  sort(all(R), [&](auto &i, auto &j) {
    return comp(i.second, j.second);
  });
  int ans = 0;
  chmax(ans, max(l, (int)N - l));
  int M = R.size();
  vector<int> B(M + 1, 0);
  rep(i, 0, M) {
    B[i + 1] = B[i] + R[i].first;
  }
  rep(i, 0, N) {
    int nl = l + B[i + 1];
    chmax(ans, max(nl, (int)N - nl));
  }
  cout << ans << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}