/*
// a <= b :
// a % b = a ... x
// a % b = 0 ... x

// N >= a > b > c >= 1 && a % b != 0

// b <= sqrt(N):
// count(a) = N - b - floor(N/b) + 1

// b > sqrt(N):
// count(a | a % b = 0, a > b) <= sqrt(N) ... m
// floor(N/2) < b <= N : m = 0, count(a) = sum_{b = 1 -> N}(N - b - m) - sum_{b = 1 -> floor(N/2)}(N - b - m)
// floor(N/3) < b <= floor(N/2) : m = 1
// max(floor(N/k+1), sqrt(N)) < b <= floor(N/k) : m = k - 1

*/

#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using mint = atcoder::modint998244353;

int main() {
  ll N;
  cin >> N;
  const ll MOD = 998244353;
  const ll inv2 = mint(2).inv().val();
  ll sqrtN = sqrt(N);

  ll ans(0);
  for (ll b = 1; b <= sqrtN; ++b) {
    ans += N - b - N / b + 1;
    ans %= MOD;
  }

  // sum_{b = 1 -> B} (N - b - m)
  auto f = [&](ll B, ll m) -> ll {
    ll res = (B % MOD) * ((N - m) % MOD) % MOD;
    res -= (B % MOD) * ((B + 1) % MOD) % MOD * inv2 % MOD;
    return res;
  };

  ll k = 1;
  while (N / k > sqrtN) {
    ans += f(N / k, k - 1) - f(max(N / (k + 1), sqrtN), k - 1) + MOD;
    ans %= MOD;
    ++k;
  }
  cout << ans << endl;
}
