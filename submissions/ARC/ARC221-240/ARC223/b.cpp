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

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
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

void solve(const vector<mint> &kaijou, const vector<mint> &kaijou_inv) {
  ll N, K;
  cin >> N >> K;
  vector<ll> A(N);
  rep(i, 0, N) { cin >> A[i]; }
  vector<ll> R;
  int i = 0;
  rep(i, 0, N - 1) {
    if ((A[i] + A[i + 1]) % K != 0 && (abs(A[i] - A[i + 1]) % K) != 0) {
      R.emplace_back(i + 1);
    }
  }
  R.emplace_back(N);
  ll l = 0;
  mint ans = 1;
  rep(s, 0, R.size()) {
    ll r = R[s];
    if (A[l] * 2 % K == 0) {
      map<ll, ll> mp;
      rep(i, l, r) { mp[A[i]]++; }
      mint add = kaijou[r - l];
      for (auto [_, x] : mp) {
        add *= kaijou_inv[x];
      }
      ans *= add;
    } else {
      map<ll, ll> mp;
      rep(i, l, r) { mp[A[i] % K]++; }
      mint add = kaijou[r - l];
      for (auto [_, x] : mp) {
        add *= kaijou_inv[x];
      }
      ans *= add;
    }
    l = r;
  }
  cout << ans.val() << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  vector<mint> kaijou(2e5 + 2, 0);
  vector<mint> kaijou_inv(2e5 + 2, 0);
  kaijou[0] = 1;
  kaijou_inv[0] = 1;
  rep(i, 0, 2e5 + 1) {
    kaijou[i + 1] = kaijou[i] * (i + 1);
    kaijou_inv[i + 1] = kaijou_inv[i] * mint(i + 1).inv();
  }
  while (T--)
    solve(kaijou, kaijou_inv);
}