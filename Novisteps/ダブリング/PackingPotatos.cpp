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

// packing_start -> next_packing_start

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, Q, X;
  cin >> N >> Q >> X;
  vector<ll> W(N);
  rep(i, 0, N) {
    cin >> W[i];
  }
  vector<ll> SW(2 * N + 1, 0);
  rep(i, 0, 2 * N) {
    SW[i + 1] = SW[i] + W[i % N];
  }
  ll t = 0;
  if (SW[N] <= X) {
    t = X / SW[N];
    X -= t * SW[N];
  }
  vector<int> next_start(N);
  // X < SW[N]
  rep(i, 0, N) {
    int j = lower_bound(all(SW), SW[i] + X) - SW.begin();
    next_start[i] = j;
  }
  vector<vector<int>> doubling;
  doubling.emplace_back(next_start);
  ll nibeki = 1;
  ll d = 0;
  ll M = 1e12;
  vector<int> next_doubling(N);
  while (nibeki < M) {
    rep(i, 0, N) {
      next_doubling[i] = doubling[d][doubling[d][i] % N];
    }
    doubling.emplace_back(next_doubling);
    nibeki <<= 1;
    d++;
  }

  rep(q, 0, Q) {
    ll K;
    cin >> K;
    K--;
    d = 0;
    int u = 0;
    while (K) {
      if (K & 1) {
        u = doubling[d][u % N];
      }
      K >>= 1;
      d++;
    }
    cout << (next_start[u % N] - u % N) + N * t << endl;
  }
}