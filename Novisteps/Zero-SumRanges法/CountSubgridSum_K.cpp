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

  vector<vector<int>> SA(H + 1, vector<int>(W + 1, 0));
  rep(i, 0, H) {
    rep(j, 0, W) {
      SA[i + 1][j + 1] = SA[i + 1][j] + S[i][j] - '0';
    }
  }
  rep(i, 0, H) {
    rep(j, 0, W) {
      SA[i + 1][j + 1] += SA[i][j + 1];
    }
  }

  vector<int> cnt(4 * H * W + 1, 0);
  int offset = 2 * H * W;
  ll ans = 0;
  rep(lx, 0, H) {
    rep(rx, lx + 1, H + 1) {
      cnt[SA[rx][0] - SA[lx][0] + offset]++;
      rep(y, 1, W + 1) {
        ans += cnt[SA[rx][y] - SA[lx][y] - K + offset];
        cnt[SA[rx][y] - SA[lx][y] + offset]++;
      }
      rep(y, 0, W + 1) {
        cnt[SA[rx][y] - SA[lx][y] + offset]--;
      }
    }
  }
  cout << ans << endl;
}