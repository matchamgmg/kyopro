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

  int N, H, W;
  cin >> N >> H >> W;
  vector<int> A(N), B(N);
  rep(i, 0, N) { cin >> A[i] >> B[i]; }

  vector<vector<bool>> board(H, vector<bool>(W, false));

  auto put_tile = [&](int x, int y, int h, int w) -> bool {
    bool res = true;
    if (!(0 <= x && x + h <= H && 0 <= y && y + w <= W)) {
      res = false;
    } else {
      rep(i, 0, h) {
        rep(j, 0, w) { res &= (board[x + i][y + j] == false); }
      }
    }
    if (res) {
      rep(i, 0, h) {
        rep(j, 0, w) { board[x + i][y + j] = true; }
      }
    }
    return res;
  };

  auto remove_tile = [&](int x, int y, int h, int w) -> void {
    rep(i, 0, h) {
      rep(j, 0, w) { board[x + i][y + j] = false; }
    }
  };

  auto f = [&](const vector<int> &v) -> bool {
    auto dfs = [&](auto g, int dep, int dir) -> bool {
      bool res = false;
      if (dep == v.size()) {
        return true;
      }
      int x = H, y = W;
      rep(i, 0, H * W) {
        if (board[i / W][i % W] == false) {
          x = i / W, y = i % W;
          break;
        }
      }
      if (dir == 0) {
        if (put_tile(x, y, A[v[dep]], B[v[dep]])) {
          res |= g(g, dep + 1, 0);
          res |= g(g, dep + 1, 1);
          remove_tile(x, y, A[v[dep]], B[v[dep]]);
        }
      } else {
        if (put_tile(x, y, B[v[dep]], A[v[dep]])) {
          res |= g(g, dep + 1, 0);
          res |= g(g, dep + 1, 1);
          remove_tile(x, y, B[v[dep]], A[v[dep]]);
        }
      }
      return res;
    };
    return dfs(dfs, 0, 0) || dfs(dfs, 0, 1);
  };

  bool ans = false;
  rep(bit, 0, (1 << N)) {
    vector<int> tile;
    int siz = 0;
    rep(b, 0, N) {
      if (bit >> b & 1) {
        tile.emplace_back(b);
        siz += A[b] * B[b];
      }
    }
    if (siz != H * W) {
      continue;
    }
    do {
      ans |= f(tile);
    } while (next_permutation(all(tile)));
  }
  YN(ans);
}