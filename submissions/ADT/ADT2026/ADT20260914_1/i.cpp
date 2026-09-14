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

  ll N;
  cin >> N;
  string S, T;
  cin >> S >> T;
  int s = S.size();
  int t = T.size();

  vector<vector<ll>> cnt(26, vector<ll>(2 * s + 1, 0));
  rep(i, 0, s) {
    rep(c, 0, 26) {
      cnt[c][i + 1] = cnt[c][i] + (S[i] == c + 'a');
    }
  }
  rep(i, s, 2 * s) {
    rep(c, 0, 26) {
      cnt[c][i + 1] = cnt[c][i] + (S[i - s] == c + 'a');
    }
  }

  auto f = [&](ll m) -> bool {
    int offset = 0;
    ll res = 0;
    rep(i, 0, t) {
      if (res > N) {
        return false;
      }
      ll n = m;
      int c = T[i] - 'a';
      if (cnt[c][s + offset] - cnt[c][offset] == 0) {
        return false;
      }
      if (m - 1 >= (cnt[c][s + offset] - cnt[c][offset])) {
        ll k = (m - 1) / (cnt[c][s + offset] - cnt[c][offset]);
        res += k;
        n -= k * (cnt[c][s + offset] - cnt[c][offset]);
      }
      if (n > 0) {
        ll r = lower_bound(all(cnt[c]), n + cnt[c][offset]) - cnt[c].begin();
        offset += r - offset;
      }
      if (offset >= s) {
        res += offset / s;
        offset %= s;
      }
    }
    return res < N || res == N && offset == 0;
  };

  ll ok = 0, ng = 1e18;
  while (ng - ok > 1) {
    ll mid = (ok + ng) / 2;
    (f(mid) ? ok : ng) = mid;
  }
  cout << ok << endl;
}