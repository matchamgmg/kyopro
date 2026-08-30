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
  vector<string> S(H);
  rep(i, 0, H) {
    cin >> S[i];
  }
  vector<bool> tate_ok(W, true), yoko_ok(H, true);
  rep(i, 0, H) {
    rep(j, 0, W) {
      if (S[i][j] == '#') {
        tate_ok[j] = false;
        yoko_ok[i] = false;
      }
    }
  }
  int ans = 0;
  const int INF = 1e9;
  vector<vector<int>> dist(H, vector<int>(W, INF));
  queue<pair<int, int>> que;
  rep(i, 0, H) {
    rep(j, 0, W) {
      if (tate_ok[j] && yoko_ok[i]) {
        dist[i][j] = 0;
        ans++;
        que.emplace(i, j);
      }
    }
  }

  vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};

  while (!que.empty()) {
    auto p = que.front();
    int cur = dist[p.first][p.second];
    que.pop();
    rep(d, 0, 4) {
      int nx = p.first + dx[d];
      int ny = p.second + dy[d];
      if (!grid_check(nx, ny, H, W)) {
        continue;
      }
      if (dist[nx][ny] < INF) {
        continue;
      }
      if (S[nx][ny] == '#') {
        continue;
      }
      dist[nx][ny] = cur + 1;
      if (dist[nx][ny] <= K) {
        ans++;
      }
      que.emplace(nx, ny);
    }
  }
  cout << ans << endl;
}