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

  int H, W, N, h, w;
  cin >> H >> W >> N >> h >> w;
  vector<vector<int>> A(H, vector<int>(W));
  vector<int> cnt(N, 0);
  rep(i, 0, H) {
    rep(j, 0, W) {
      cin >> A[i][j];
      A[i][j]--;
      cnt[A[i][j]]++;
    }
  }
  vector<vector<int>> ans(H - h + 1, vector<int>(W - w + 1, 0));
  rep(i, 0, h) {
    rep(j, 0, w) {
      cnt[A[i][j]]--;
    }
  }
  rep(i, 0, N) {
    ans[0][0] += (cnt[i] > 0);
  }
  rep(i, 0, H - h + 1) {
    rep(jj, 0, W - w + 1) {
      int j = (i % 2 == 0 ? jj : W - w - jj);
      if (i == 0 && j == 0) {
        continue;
      }
      if (j == 0 && i % 2 == 0 || j == W - w && i % 2 == 1) { // shita
        int after = ans[i - 1][j];
        rep(y, 0, w) {
          cnt[A[i - 1][j + y]]++;
          if (cnt[A[i - 1][j + y]] == 1) {
            after++;
          }
          cnt[A[i + h - 1][j + y]]--;
          if (cnt[A[i + h - 1][j + y]] == 0) {
            after--;
          }
        }
        ans[i][j] = after;
      } else if (i % 2 == 0) {
        int after = ans[i][j - 1];
        rep(x, 0, h) {
          cnt[A[i + x][j - 1]]++;
          if (cnt[A[i + x][j - 1]] == 1) {
            after++;
          }
          cnt[A[i + x][j + w - 1]]--;
          if (cnt[A[i + x][j + w - 1]] == 0) {
            after--;
          }
        }
        ans[i][j] = after;
      } else if (i % 2 == 1) {
        int after = ans[i][j + 1];
        rep(x, 0, h) {
          cnt[A[i + x][j]]--;
          if (cnt[A[i + x][j]] == 0) {
            after--;
          }
          cnt[A[i + x][j + w]]++;
          if (cnt[A[i + x][j + w]] == 1) {
            after++;
          }
        }
        ans[i][j] = after;
      }
    }
  }
  vv_cout(ans);
}