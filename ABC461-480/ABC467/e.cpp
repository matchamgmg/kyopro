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

  ll N, M;
  cin >> N >> M;
  vector<ll> A(N), B(N - 1);
  rep(i, 0, N) {
    cin >> A[i];
  }
  rep(i, 0, N - 1) {
    cin >> B[i];
  }
  vector<ll> SB(N, 0); // B[k] = B[k] - B[k-1] + B[k-2] - ...
  rep(i, 0, N - 1) {
    SB[i + 1] += (B[i] - SB[i] + M) % M;
  }

  // C0 = 0 としたときのCiの値
  vector<ll> C(N, 0);
  rep(i, 1, N) {
    C[i] = SB[i];
  }
  ll cost = 0;
  rep(i, 0, N) {
    cost += (C[i] - A[i] + M) % M;
  }
  // Ci == Ai となるxと，それによってcost -= M されるiの個数
  set<ll> zero;
  map<ll, ll> mp_minus;
  ll even = 0;
  for (int i = 0; i < N; i += 2) {
    if (C[i] != A[i]) {
      mp_minus[(A[i] - C[i] + M) % M]++;
      zero.emplace((A[i] - C[i] + M) % M);
    }
    even++;
  }
  // Ci == Ai となるxと，それによってcost += M されるiの個数
  map<ll, ll> mp_plus;
  ll odd = 0;
  for (int i = 1; i < N; i += 2) {
    mp_plus[(C[i] - A[i] + M) % M]++;
    zero.emplace((C[i] - A[i] + M) % M);
    odd++;
  }

  ll ans = cost;
  ll recent = 0;

  for (auto x : zero) {
    cost += (x - recent) * (even - odd) - M * mp_minus[x];
    chmin(ans, cost);
    cost += M * mp_plus[x];
    recent = x;
  }
  cout << ans << endl;
}