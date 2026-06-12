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

struct Rect {
  ll lx, ly, rx, ry;
  Rect() = default;
  Rect(ll lx, ll ly, ll rx, ll ry) : lx(lx), ly(ly), rx(rx), ry(ry) {}
  const bool operator<(const Rect &rhs) const {
    return lx == rhs.lx ? ly == rhs.ly ? rx == rhs.rx ? ry < rhs.ry : rx < rhs.rx : ly < rhs.ly : lx < rhs.lx;
  }
};

struct UnionFind {
  UnionFind(int n_) : n(n_), group_num(n_) { par.assign(n_, -1); }
  UnionFind(vector<Rect> &v) : n((int)v.size()), group_num((int)v.size()) {
    par.assign((int)v.size(), -1);
    space.assign((int)v.size(), 0);
    rep(i, 0, (int)v.size()) {
      Rect &r = v[i];
      space[i] = (r.rx - r.lx) * (r.ry - r.ly);
    }
  }
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
    space[y] += space[x];
    par[x] = y;
    group_num--;
    return true;
  }
  int groups() { return group_num; }
  ll get_space(int x) { return space[root(x)]; }

private:
  int n;
  int group_num;
  vector<int> par;
  vector<ll> space;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, X, Y;
  cin >> N >> X >> Y;

  set<Rect> rects, next_rects;
  rects.emplace(0, 0, X, Y);

  char C;
  ll A, B;
  rep(i, 0, N) {
    next_rects.clear();
    cin >> C >> A >> B;
    if (C == 'X') {
      for (auto r : rects) {
        if (r.lx < A) {
          next_rects.emplace(r.lx, r.ly - B, min(A, r.rx), r.ry - B);
        }
        if (A < r.rx) {
          next_rects.emplace(max(A, r.lx), r.ly + B, r.rx, r.ry + B);
        }
      }
    } else {
      for (auto r : rects) {
        if (r.ly < A) {
          next_rects.emplace(r.lx - B, r.ly, r.rx - B, min(A, r.ry));
        }
        if (A < r.ry) {
          next_rects.emplace(r.lx + B, max(A, r.ly), r.rx + B, r.ry);
        }
      }
    }
    swap(rects, next_rects);
  }

  vector<Rect> V;
  for (auto r : rects) {
    V.emplace_back(r);
  }

  int m = V.size();
  UnionFind uf(V);

  auto check = [](const Rect &a, const Rect &b) -> bool {
    bool res = false;
    if (min(a.rx, b.rx) - max(a.lx, b.lx) >= 1 && (a.ly == b.ry || a.ry == b.ly)) {
      res = true;
    }
    if (min(a.ry, b.ry) - max(a.ly, b.ly) >= 1 && (a.lx == b.rx || a.rx == b.lx)) {
      res = true;
    }
    return res;
  };

  rep(i, 0, m - 1) {
    rep(j, i + 1, m) {
      if (check(V[i], V[j])) {
        uf.merge(i, j);
      }
    }
  }
  vector<ll> ans;
  rep(i, 0, m) {
    if (uf.root(i) == i) {
      ans.emplace_back(uf.get_space(i));
    }
  }
  sort(all(ans));
  cout << ans.size() << endl;
  v_cout(ans);
}