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

void solve(const vector<vector<ll>> &LCM) {
  int N;
  cin >> N;
  rep(i, 0, N) {
    rep(j, 0, N) {
      cout << LCM[i][j] << ' ';
    }
    cout << '\n';
  }
  cout << flush;
}

ll my_pow(ll a, ll b) {
  ll res = 1;
  while (b > 0) {
    if (b & 1) {
      res *= a;
    }
    a *= a;
    b >>= 1;
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll M = 1000;
  vector<ll> lpf(M + 1);
  vector<ll> primes;
  iota(all(lpf), 0);
  for (ll x = 2; x <= M; ++x) {
    if (lpf[x] == x)
      primes.emplace_back(x);
    for (ll p : primes) {
      if (p * x > M || p > lpf[x])
        break;
      lpf[p * x] = p;
    }
  }

  vector<vector<map<ll, ll>>> factors(M, vector<map<ll, ll>>(M));
  vector<vector<ll>> LCM(M, vector<ll>(M, 1));
  for (ll m = 2; m <= M; ++m) {
    ll x = m;
    map<ll, ll> fac;
    while (x > 1) {
      ll p = lpf[x];
      fac[p]++;
      x /= p;
    }
    for (int i = m - 1; i < M; i += m) {
      for (int j = m - 1; j < M; j += m) {
        for (auto &[p, k] : fac) {
          if (factors[i][j][p] < k) {
            LCM[i][j] *= my_pow(p, k - factors[i][j][p]);
            factors[i][j][p] = k;
          }
        }
      }
    }
  }

  int T;
  cin >> T;
  while (T--)
    solve(LCM);
}