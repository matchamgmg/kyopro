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

  int H, W;
  cin >> H >> W;
  int A, B;
  cin >> A >> B;
  A--, B--;
  int R1, C1, R2, C2;
  cin >> R1 >> C1 >> R2 >> C2;
  R1--, C1--;
  int P, Q;
  cin >> P >> Q;
  P--, Q--;

  bool inside = false;
  vector<pair<int, int>> RC;
  rep(i, R1, R2) {
    rep(j, C1, C2) {
      if (R1 <= A && A < R2 && C1 <= B && B < C2) {
        inside = true;
      }
      if (R1 <= P && P < R2 && C1 <= Q && Q < C2) {
        inside = true;
      }
      if (i == R1 || i == R2 - 1 || j == C1 || j == C2 - 1) {
        RC.emplace_back(i, j);
      }
    }
  }
  int M = RC.size();

  vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};
  const int INF = 1e9;
  int ans = INF;
  for (auto &[R, C] : RC) {
    vector<vector<int>> dist(H, vector<int>(W, INF));
    queue<pair<int, int>> que;
    dist[R][C] = 0;
    que.emplace(R, C);
    while (!que.empty()) {
      auto [x, y] = que.front();
      que.pop();
      rep(d, 0, 4) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (!grid_check(nx, ny, H, W)) {
          continue;
        }
        if (dist[nx][ny] < INF) {
          continue;
        }
        dist[nx][ny] = dist[x][y] + 1;
        que.emplace(nx, ny);
      }
    }
    chmin(ans, dist[A][B] + dist[P][Q]);
  }

  vector<vector<int>> dist(H, vector<int>(W, INF));
  queue<pair<int, int>> que;
  dist[A][B] = 0;
  que.emplace(A, B);
  while (!que.empty()) {
    auto [x, y] = que.front();
    que.pop();
    rep(d, 0, 4) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (!grid_check(nx, ny, H, W)) {
        continue;
      }
      if (dist[nx][ny] < INF) {
        continue;
      }
      dist[nx][ny] = dist[x][y] + 1;
      que.emplace(nx, ny);
    }
  }

  if (inside) {
    cout << 2 * dist[P][Q] << endl;
  } else {
    cout << ans + dist[P][Q] << endl;
  }
}