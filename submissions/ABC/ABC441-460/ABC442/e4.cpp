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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<ll> X(N), Y(N);
  vector<int> idx(N);
  rep(i, 0, N) {
    cin >> X[i] >> Y[i];
    idx[i] = i;
  }

  auto cross = [&](int i, int j) -> ll {
    return X[i] * Y[j] - Y[i] * X[j];
  };

  auto comp = [&](int i, int j) -> bool {
    int upi = (Y[i] > 0 || Y[i] == 0 && X[i] < 0);
    int upj = (Y[j] > 0 || Y[j] == 0 && X[j] < 0);
    if (upi != upj) {
      return upi > upj;
    }
    return cross(i, j) < 0;
  };

  sort(all(idx), comp);
  vector<int> dict(N);
  rep(i, 0, N) {
    dict[idx[i]] = i;
  }
  vector<int> L(2 * N, -1), R(2 * N, N);
  int l = -1;
  rep(i, 0, 2 * N - 1) {
    if (comp(idx[i % N], idx[(i + 1) % N]) || comp(idx[(i + 1) % N], idx[i % N])) {
      l = i;
    }
    L[i + 1] = l;
  }
  int r = N;
  rrep(i, 0, 2 * N - 1) {
    if (comp(idx[i % N], idx[(i + 1) % N]) || comp(idx[(i + 1) % N], idx[i % N])) {
      r = i + 1;
    }
    R[i] = r;
  }

  rep(q, 0, Q) {
    int a, b;
    cin >> a >> b;
    a--, b--;
    l = L[dict[a]];
    r = R[dict[b]];
    if (r <= l + 1) {
      r += N;
    }
    cout << r - l - 1 << endl;
  }
}