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

  int H, W;
  cin >> H >> W;
  vector<string> S(H);
  rep(i, 0, H) { cin >> S[i]; }
  vector<int> dx = {1, 1, 0, -1, -1, -1, 0, 1}, dy = {0, 1, 1, 1, 0, -1, -1, -1};
  vector<char> color = {'#', '.'};

  queue<int> frontier;
  queue<int> task;
  vector<int> time(H * W, -1);
  vector<bool> init_color(H * W, false);
  auto check2 = [&](int x, int y) -> bool {
    bool res = S[x][y] == '#';
    rep(d, 0, 8) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (!grid_check(nx, ny, H, W)) {
        continue;
      }
      if (S[nx][ny] == '.') {
        res = false;
      }
    }
    return res;
  };
  rep(i, 0, H) {
    rep(j, 0, W) { init_color[i * W + j] = check2(i, j); }
  }
  rep(i, 0, H) {
    rep(j, 0, W) {
      if (init_color[i * W + j]) {
        S[i][j] = '.';
      }
    }
  }
  rep(i, 0, H) {
    rep(j, 0, W) {
      if (S[i][j] == '#') {
        frontier.push(i * W + j);
        time[i * W + j] = 0;
      }
    }
  }
  int t = 0;
  auto check = [&](int x, int y, int tt) -> bool {
    bool res = false;
    rep(d, 0, 8) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (!grid_check(nx, ny, H, W)) {
        continue;
      }
      if (time[nx * W + ny] >= 0) {
        if (tt % 2 == time[nx * W + ny] % 2) {
          res = true;
        }
      }
    }
    return res;
  };
  // # : 偶数時刻で黒のマス
  while (!frontier.empty()) {
    while (!frontier.empty() && time[frontier.front()] == t) {
      int p = frontier.front();
      frontier.pop();
      int x = p / W;
      int y = p % W;
      rep(d, 0, 8) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (!grid_check(nx, ny, H, W)) {
          continue;
        }
        if (time[nx * W + ny] >= 0) {
          continue;
        }
        bool ok = check(nx, ny, t);
        if (ok) {
          task.push(nx * W + ny);
        }
      }
    }
    while (!task.empty()) {
      int p = task.front();
      task.pop();
      if (time[p] >= 0) {
        continue;
      }
      time[p] = t + 1;
      frontier.push(p);
    }
    ++t;
  }
  rep(i, 0, H) {
    rep(j, 0, W) { cout << color[time[i * W + j] >= 0 ? (time[i * W + j] % 2) : 1]; }
    cout << endl;
  }
}