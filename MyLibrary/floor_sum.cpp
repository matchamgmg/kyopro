#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll floor_sum(ll N, ll M, ll A, ll B) {
  ll A1 = A / M, A2 = A % M;
  ll B1 = B / M, B2 = B % M;
  ll S = N * (N - 1) / 2 * A1;
  if (A2 == 0) {
    return S + B1 * N;
  } else {
    ll K = (A2 * (N - 1) + B2) / M;
    return S + B1 * N + K * N - floor_sum(K, A2, M, M - B2 + A2 - 1);
  }
}

void solve() {
  ll N, M, A, B;
  cin >> N >> M >> A >> B;
  cout << floor_sum(N, M, A, B) << endl;
}

int main() {
  int T;
  cin >> T;
  while (T--)
    solve();
}