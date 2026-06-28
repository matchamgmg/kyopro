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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, X;
  cin >> N >> X;
  vector<ll> A(N / 2), B(N - N / 2);
  rep(i, 0, N / 2) { cin >> A[i]; }
  rep(i, N / 2, N) { cin >> B[i]; }
  int NA = N / 2, NB = N - N / 2;
  vector<pair<ll, ll>> AC(1 << NA, {0, -1}), BC(1 << NB, {0, -1});
  rep(bit, 0, (1 << NA)) {
    AC[bit].second = bit;
    rep(i, 0, NA) {
      if (bit >> i & 1) {
        AC[bit].first += A[i];
      }
    }
  }
  rep(bit, 0, (1 << NB)) {
    BC[bit].second = bit;
    rep(i, 0, NB) {
      if (bit >> i & 1) {
        BC[bit].first += B[i];
      }
    }
  }
  sort(all(AC));
  sort(all(BC));

  rep(i, 0, bit) {}
}