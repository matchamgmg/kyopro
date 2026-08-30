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

struct UnionFind {
  UnionFind(int n_) : n(n_), group_num(n_) { par.assign(n_, -1); }
  int root(int x) {
    if (par[x] < 0)
      return x;
    return par[x] = root(par[x]);
  }
  int size(int x) { return -par[root(x)]; }
  bool same(int x, int y) { return root(x) == root(y); }
  bool merge(int x, int y) {
    x = root(x);
    y = root(y);
    if (x == y)
      return false;
    if (size(x) > size(y))
      swap(x, y);
    par[y] += par[x];
    par[x] = y;
    group_num--;
    return true;
  }
  int groups() { return group_num; }

private:
  int n;
  int group_num;
  vector<int> par;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<int> S(N), T(N);
  rep(i, 0, N) { cin >> S[i] >> T[i]; }
  vector<int> imos(2 * N + 1, 0);
  rep(i, 0, N) {
    imos[S[i]]++;
    imos[T[i]]--;
  }
  int max_imos = imos[0];
  rep(i, 0, 2 * N) {
    imos[i + 1] += imos[i];
    chmax(max_imos, imos[i + 1]);
  }
  if (max_imos >= 3) {
    cout << 0 << endl;
    return 0;
  }
  vector<int> idx(N);
  iota(all(idx), 0);
  sort(all(idx), [&](int i, int j) { return S[i] == S[j] ? T[i] < T[j] : S[i] < S[j]; });
  UnionFind uf(N);
  int max_T = 0;
  rep(i, 0, N - 1) {
    chmax(max_T, T[idx[i]]);
    if (max_T > S[idx[i + 1]]) {
      uf.merge(idx[i], idx[i + 1]);
    }
  }
  mint ans = 1;
  for (int i : idx) {
    if (i == uf.root(i)) {
      ans *= 2;
    }
  }
  cout << ans.val() << endl;
}