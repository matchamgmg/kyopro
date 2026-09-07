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

void solve() {
  int N, K;
  cin >> N >> K;
  if (N % 2 == 0) {
    cout << "No" << endl;
    return;
  }
  ll max_K = (N * N - 1) / 2;
  ll min_K = N - 1;
  if (!(min_K <= K && K <= max_K)) {
    cout << "No" << endl;
    return;
  }
  if (K % 2 == 1) {
    cout << "No" << endl;
    return;
  }
  cout << "Yes" << endl;
  ll req = (max_K - K) / 2;
  vector<vector<char>> S(N, vector<char>(N, '.'));
  rep(i, 0, N) {
    rep(j, 0, N) {
      if ((i % 2 == 0 && j == N - 1) || (i % 2 == 1 && j == 0)) {
        S[i][j] = 'D';
      } else if (i % 2 == 0) {
        S[i][j] = 'R';
      } else {
        S[i][j] = 'L';
      }
    }
  }
  int x = 0;
  int y = 0;
  vector<vector<char>> T = {{'D', 'R', 'R'}, {'D', 'U', 'D'}, {'R', 'U', 'R'}};
  rep(c, 0, req) {
    rep(i, 0, 3) {
      rep(j, 0, 3) {
        S[i + x][j + y] = T[i][j];
      }
    }
    if (x != 0) {
      S[x][y + 1] = 'U';
    }
    if (y == N - 3) {
      S[x][y + 2] = 'D';
      S[x + 2][y + 2] = 'D';
    }
    x += 2;
    if (x + 2 >= N) {
      x = 0;
      y += 2;
    }
  }

  // vv_cout(S);

  x = 0, y = 0;
  rep(i, 0, N * N - 1) {
    cout << S[x][y];
    if (S[x][y] == 'L') {
      y--;
    } else if (S[x][y] == 'R') {
      y++;
    } else if (S[x][y] == 'U') {
      x--;
    } else if (S[x][y] == 'D') {
      x++;
    }
  }
  cout << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}