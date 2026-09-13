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

  string S;
  cin >> S;
  ll N = 1e7;
  vector<ll> primes;
  vector<bool> is_prime(N + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (ll i = 2; i <= N; ++i) {
    if (!is_prime[i]) {
      continue;
    }
    primes.push_back(i);
    for (ll j = i * i; j <= N; j += i) {
      is_prime[j] = false;
    }
  }
  bool ans = false;
  for (ll p : primes) {
    string T = to_string(p);
    if (S.size() != T.size()) {
      continue;
    }
    bool ok = true;
    rep(i, 0, S.size() - 1) {
      rep(j, i + 1, S.size()) {
        ok &= ((S[i] == S[j] && T[i] == T[j]) || (S[i] != S[j] && T[i] != T[j]));
      }
    }
    ans |= ok;
    if (ans) {
      cout << T << endl;
      return 0;
    }
  }
  cout << -1 << endl;
}