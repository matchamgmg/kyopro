/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

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

struct UnionFind {
  UnionFind(int n_) : n(n_), group_num(n_) {
    par.assign(n_, -1);
  }
  int root(int x) {
    if (par[x] < 0)
      return x;
    return par[x] = root(par[x]);
  }
  int size(int x) {
    return -par[root(x)];
  }
  bool same(int x, int y) {
    return root(x) == root(y);
  }
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
  int groups() {
    return group_num;
  }

private:
  int n;
  int group_num;
  vector<int> par;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<bool> color(N, false);
  vector<vector<int>> unions(N);
  rep(i, 0, N) {
    unions[i].push_back(i);
  }
  UnionFind uf(N);
  vector<set<int>> black(N), white(N);
  rep(i, 0, N) {
    white[i].insert(i);
  }
  rep(q, 0, Q) {
    int t;
    cin >> t;
    if (t == 1) {
      int u, v;
      cin >> u >> v;
      u--, v--;
      if (!uf.same(u, v)) {
        u = uf.root(u);
        v = uf.root(v);
        if (uf.size(u) > uf.size(v)) {
          swap(u, v);
        }
        for (int s : black[u]) {
          black[v].insert(s);
        }
        for (int s : white[u]) {
          white[v].insert(s);
        }
        for (int s : unions[u]) {
          unions[v].push_back(s);
        }
        uf.merge(u, v);
      }
    } else if (t == 2) {
      int x;
      cin >> x;
      x--;
      if (color[x]) {
        black[uf.root(x)].erase(x);
        white[uf.root(x)].insert(x);
      } else {
        white[uf.root(x)].erase(x);
        black[uf.root(x)].insert(x);
      }
      color[x] = color[x] ^ 1;
    } else {
      int x;
      cin >> x;
      x--;
      YN(!black[uf.root(x)].empty());
    }
  }
}