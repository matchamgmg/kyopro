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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<string> S(N);
  rep(i, 0, N) {
    cin >> S[i];
  }
  const int INF = 1e9;
  vector<vector<int>> ans(N, vector<int>(N, INF));
  queue<tuple<int, int>> que;
  rep(i, 0, N) {
    ans[i][i] = 0;
    que.emplace(i, i);
  }
  rep(i, 0, N) {
    rep(j, 0, N) {
      if (i == j) {
        continue;
      }
      if (S[i][j] != '-') {
        ans[i][j] = 1;
        que.emplace(i, j);
      }
    }
  }
  while (!que.empty()) {
    auto [i, j] = que.front();
    que.pop();
    rep(x, 0, N) {
      rep(y, 0, N) {
        if (S[x][i] != '-' && S[j][y] != '-' && S[x][i] == S[j][y]) {
          if (ans[x][y] == INF) {
            ans[x][y] = ans[i][j] + 2;
            que.emplace(x, y);
          }
        }
      }
    }
  }
  rep(i, 0, N) {
    rep(j, 0, N) {
      cout << (ans[i][j] == INF ? -1 : ans[i][j]) << ' ';
    }
    cout << endl;
  }
}