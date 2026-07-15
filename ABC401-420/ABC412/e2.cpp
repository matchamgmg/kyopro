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

// 素因数が一つのものを数える
//

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll L, R;
  cin >> L >> R;

  ll sqrtR = sqrt(R);
  vector<ll> primes;
  vector<bool> is_prime(sqrtR + 2, true);
  is_prime[0] = is_prime[1] = false;
  for (ll p = 2; p < sqrtR + 2; ++p) {
    if (is_prime[p] == false) {
      continue;
    }
    for (ll x = p * p; x < sqrtR + 2; x += p) {
      is_prime[x] = false;
    }
    primes.emplace_back(p);
  }

  vector<bool> one_prime(R - L + 1, false);
  vector<bool> LR_is_prime(R - L + 1, true);

  for (ll p : primes) {
    for (ll x = (L + p - 1) / p * p; x <= R; x += p) {
      if (one_prime[x - L] == true) {
        continue;
      }
      ll y = x;
      while (y % p == 0) {
        y /= p;
      }
      if (y == 1) {
        one_prime[x - L] = true;
      }
      LR_is_prime[x - L] = (LR_is_prime[x - L] && (y == x));
    }
  }

  ll ans = 1;
  rep(i, 1, R - L + 1) { ans += one_prime[i]; }
  rep(i, 1, R - L + 1) { ans += LR_is_prime[i]; }
  cout << ans << endl;
}