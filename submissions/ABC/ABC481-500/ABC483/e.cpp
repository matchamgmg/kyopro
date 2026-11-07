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
  ll N, M;
  cin >> N >> M;
  vector<string> S(N);
  rep(i, 0, N) {
    cin >> S[i];
  }
  ll ok6 = 0;
  vector<ll> ok5(M, 0);
  vector<vector<ll>> ok4(M, vector<ll>(M, 0));
  unordered_map<string, ll> mp6;
  for (string &s : S) {
    mp6[s]++;
  }
  for (auto &[k, v] : mp6) {
    ok6 += v * (v - 1) / 2;
  }
  rep(i, 0, M) {
    unordered_map<string, ll> mp5;
    for (string &s : S) {
      string t = s.substr(0, i) + s.substr(i + 1, M - i - 1);
      mp5[t]++;
    }
    for (auto &[k, v] : mp5) {
      ok5[i] += v * (v - 1) / 2;
    }
    ok5[i] -= ok6;
  }
  rep(i, 0, M - 1) {
    rep(j, i + 1, M) {
      unordered_map<string, ll> mp4;
      for (string &s : S) {
        string t = s.substr(0, i) + s.substr(i + 1, j - i - 1) + s.substr(j + 1, M - j - 1);
        mp4[t]++;
      }
      for (auto &[k, v] : mp4) {
        ok4[i][j] += v * (v - 1) / 2;
      }
      ok4[i][j] -= ok5[i] + ok5[j] + ok6;
    }
  }
  ll ans = 0;
  ans += ok6;
  rep(i, 0, M) {
    ans += ok5[i];
  }
  rep(i, 0, M - 1) {
    rep(j, i + 1, M) {
      ans += ok4[i][j];
    }
  }
  cout << ans << endl;
}