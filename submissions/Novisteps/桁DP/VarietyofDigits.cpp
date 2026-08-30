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

  string N;
  ll M;
  cin >> N >> M;
  vector<int> C(M);
  rep(i, 0, M) { cin >> C[i]; }
  int mask = 0;
  rep(i, 0, M) { mask = mask | 1 << C[i]; }

  vector<mint> dp_sum(2 * (1 << 10), 0), dp_num(2 * (1 << 10), 0);
  dp_num[0] = 1;
  for (char c : N) {
    c -= '0';
    vector<mint> old_sum(2 * (1 << 10), 0), old_num(2 * (1 << 10), 0);
    swap(dp_sum, old_sum);
    swap(dp_num, old_num);
    rep(i, 0, 2 * (1 << 10)) {
      if (old_num[i] == 0) {
        continue;
      }
      int u = i / (1 << 10);
      int v = i % (1 << 10);
      rep(d, 0, 10) {
        int nu = u;
        int nv = v | 1 << d;
        if (u == 0 && d > c) {
          continue;
        }
        if (u == 0 && d < c) {
          nu = 1;
        }
        if (v == 0 && d == 0) {
          nv = v;
        }
        dp_sum[nu * (1 << 10) + nv] += old_sum[i] * 10 + old_num[i] * d;
        dp_num[nu * (1 << 10) + nv] += old_num[i];
      }
    }
  }
  mint ans = 0;
  rep(i, 0, 2 * (1 << 10)) {
    int u = i / (1 << 10);
    int v = i % (1 << 10);
    if ((v & mask) == mask) {
      ans += dp_sum[i];
    }
  }
  cout << ans.val() << endl;
}