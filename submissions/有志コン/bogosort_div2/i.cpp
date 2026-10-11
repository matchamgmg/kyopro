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

// Bob : 1マス埋める
// Alice : 1マス埋める or 半径Kの正方形領域内にジャンプ(遅延行為)
// ジャンプできなくなったら，それ以降もジャンプできない(実質終了状態)
// Bobの手番の終了直後，ジャンプできなくなったとき，残りの空きマスの個数について
//  奇数 : Alice win
//  偶数 : Bob win

void solve() {
  int H, W, K;
  cin >> H >> W >> K;
  vector<string> S(H);
  vector<vector<int>> A(H, vector<int>(W, 0));
  rep(i, 0, H) {
    cin >> S[i];
  }
  int cnt = 0;
  rep(i, 0, H) {
    rep(j, 0, W) {
      if (S[i][j] == '.') {
        A[i][j] = 1;
        cnt++;
      }
    }
  }
  if (cnt % 2 == 1) {
    cout << cnt << endl;
  } else {
    vector<vector<int>> SA(H + 1, vector<int>(W + 1, 0));
    rep(i, 0, H) {
      rep(j, 0, W) {
        SA[i + 1][j + 1] = SA[i + 1][j] + A[i][j];
      }
    }
    rep(j, 0, W) {
      rep(i, 0, H) {
        SA[i + 1][j + 1] += SA[i][j + 1];
      }
    }

    auto f = [&](int x, int y) -> bool {
      int lx, ly, rx, ry;
      lx = max(0, x - K);
      ly = max(0, y - K);
      rx = min(H, x + K + 1);
      ry = min(W, y + K + 1);
      int res = SA[rx][ry] - SA[rx][ly] - SA[lx][ry] + SA[lx][ly];
      return res >= 3;
    };

    int ans = 0;
    rep(i, 0, H) {
      rep(j, 0, W) {
        if (S[i][j] == '#') {
          continue;
        }
        ans += f(i, j);
      }
    }
    cout << ans << endl;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}