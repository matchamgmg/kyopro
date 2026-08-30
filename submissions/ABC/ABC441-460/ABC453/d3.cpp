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

  const int INF = 1e9;
  vector<pair<int, int>> dist(H * W * 4, {INF, -1});
  auto int2idx = [&](int n) -> tuple<int, int, int> {
    int d = n % 4;
    n /= 4;
    return {n / W, n % W, d};
  };
  auto idx2int = [&](int x, int y, int d) -> int { return (x * W + y) * 4 + d; };

  int sx, sy, gx, gy;

  rep(i, 0, H) {
    rep(j, 0, W) {
      if (S[i][j] == 'S') {
        sx = i;
        sy = j;
      } else if (S[i][j] == 'G') {
        gx = i;
        gy = j;
      }
    }
  }

  queue<tuple<int, int, int>> que; // x, y, d
  vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};
  rep(d, 0, 4) { dist[idx2int(sx, sy, d)].first = 0; }
  que.emplace(sx, sy, 0);

  while (!que.empty()) {
    auto [x, y, d] = que.front();
    que.pop();
    rep(nd, 0, 4) {
      if (S[x][y] == 'o' && nd != d) {
        continue;
      }
      if (S[x][y] == 'x' && nd == d) {
        continue;
      }
      int nx = x + dx[nd];
      int ny = y + dy[nd];
      if (!grid_check(nx, ny, H, W)) {
        continue;
      }
      if (S[nx][ny] == '#') {
        continue;
      }
      if (dist[idx2int(nx, ny, nd)].first > dist[idx2int(x, y, d)].first + 1) {
        que.emplace(nx, ny, nd);
        dist[idx2int(nx, ny, nd)].first = dist[idx2int(x, y, d)].first + 1;
        dist[idx2int(nx, ny, nd)].second = d;
      }
    }
  }

  int ans = INF;
  int ans_d;
  rep(d, 0, 4) {
    if (ans > dist[idx2int(gx, gy, d)].first) {
      ans = dist[idx2int(gx, gy, d)].first;
      ans_d = d;
    }
  }
  if (ans == INF) {
    cout << "No" << endl;
  } else {
    cout << "Yes" << endl;
    vector<char> RDLU = {'R', 'D', 'L', 'U'};
    string T;
    int px, py, pd;
    while (!(gx == sx && gy == sy)) {
      T += RDLU[ans_d];
      px = gx - dx[ans_d];
      py = gy - dy[ans_d];
      pd = dist[idx2int(gx, gy, ans_d)].second;
      gx = px;
      gy = py;
      ans_d = pd;
    }
    reverse(all(T));
    cout << T << endl;
  }
}