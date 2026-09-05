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

  using P = pair<int, int>;
  int N;
  cin >> N;
  vector<string> S(N);
  rep(i, 0, N) {
    cin >> S[i];
  }
  vector<P> player(2);
  int cnt = 0;
  rep(i, 0, N) {
    rep(j, 0, N) {
      if (S[i][j] == 'P') {
        player[cnt++] = {i, j};
      }
    }
  }

  State s = {player[0], player[1]};
  unordered_map<tuple<int, int, int, int>, int> dist;
  queue<tuple<int, int, int, int>> que;
  dist[s] = 0;
  que.push(s);

  vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};
  while (!que.empty()) {
    auto [x1, y1, x2, y2] = que.front();
    que.pop();
    rep(d, 0, 4) {
      int nx1 = x1 + dx[d];
      int ny1 = y1 + dy[d];
      int nx2 = x2 + dx[d];
      int ny2 = y2 + dy[d];
      if (!grid_check(nx1, ny1, N, N)) {
        nx1 -= dx[d];
        ny1 -= dy[d];
      }
      if (!grid_check(nx2, ny2, N, N)) {
        nx2 -= dx[d];
        ny2 -= dy[d];
      }
      if (S[nx1][ny1] == '#') {
        nx1 -= dx[d];
        ny1 -= dy[d];
      }
      if (S[nx2][ny2] == '#') {
        nx2 -= dx[d];
        ny2 -= dy[d];
      }
      tuple<int, int, int, int> t = {nx1, ny1, nx2, ny2};
      a;
    }
  }
}