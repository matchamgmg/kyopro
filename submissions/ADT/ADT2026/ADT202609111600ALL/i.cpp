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

  int H, W, K;
  cin >> H >> W >> K;
  const int INF = 1e9;
  queue<pair<int, int>> que;
  vector<vector<int>> dist(H, vector<int>(W, INF));
  vector<vector<int>> det(H, vector<int>(W, 0));
  rep(i, 0, H) {
    rep(j, 0, W) {
      det[i][j] = 0;
    }
  }
  vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};
  rep(i, 0, K) {
    int r, c;
    cin >> r >> c;
    r--, c--;
    dist[r][c] = 0;
    det[r][c] = 4;
    que.emplace(r, c);
  }
  while (!que.empty()) {
    auto [x, y] = que.front();
    que.pop();
    rep(d, 0, 4) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (!grid_check(nx, ny, H, W)) {
        continue;
      }
      det[nx][ny]++;
      if (dist[nx][ny] == INF && det[nx][ny] == 2) {
        dist[nx][ny] = dist[x][y] + 1;
        que.emplace(nx, ny);
      }
    }
  }
  ll ans = 0;
  rep(i, 0, H) {
    rep(j, 0, W) {
      ans += (dist[i][j] == INF ? 0LL : dist[i][j]);
    }
  }
  cout << ans << endl;
}