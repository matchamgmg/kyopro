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

void solve(const vector<ll> &lpf) {
  int N;
  cin >> N;
  vector<int> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }

  struct Bests {
    int first = 0;
    int first_idx = -1;
    int second = 0;
    int second_idx = -1;
    void update(int a, int i) {
      if (first <= a) {
        second = first;
        second_idx = first_idx;
        first = a;
        first_idx = i;
      } else if (second <= a) {
        second = a;
        second_idx = i;
      }
    }
  };
  map<int, Bests> prime_factorials;
  vector<vector<pair<int, int>>> factors(N);
  rep(i, 0, N) {
    int a = A[i];
    vector<pair<int, int>> f;
    while (a > 1) {
      int p = lpf[a];
      if (f.empty() || !f.empty() && f.back().first != p) {
        f.emplace_back(p, 1);
      } else {
        f.back().second++;
      }
      a /= p;
    }
    factors[i] = f;
    for (auto [p, k] : f) {
      prime_factorials[p].update(k, i);
    }
  }
  mint before = 1;
  for (auto &[p, b] : prime_factorials) {
    before *= mint(p).pow(b.first);
  }
  rep(i, 0, N) {
    mint after = before;
    for (auto &[p, k] : factors[i]) {
      auto &b = prime_factorials[p];
      if (b.first_idx == i) {
        after *= mint(p).pow(b.first).inv() * mint(p).pow(b.second);
      }
    }
    cout << after.val() << ' ';
  }
  cout << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll M = 1e7;
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

  int T;
  cin >> T;
  while (T--)
    solve(lpf);
}