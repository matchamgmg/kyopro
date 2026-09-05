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
  string R, S;
  cin >> R >> S;
  int K;
  cin >> K;
  int N = R.size();
  if (S == "Warong") {
    rep(i, 0, K) {
      R[i] = 'A';
    }
    int W_cnt = 0;
    int X_cnt = 0;
    rep(i, K, N) {
      W_cnt += (R[i] == 'W');
      X_cnt += (R[i] == '?');
    }
    if (W_cnt >= 1) {
      cout << R << endl;
    } else if (X_cnt != 1) {
      cout << R << endl;
    } else {
      rep(i, K, N) {
        if (R[i] == '?') {
          R[i] = 'W';
        }
      }
      cout << R << endl;
    }
  } else {
    int AK_cnt = 0;
    int WK_cnt = 0;
    int XK_cnt = 0;
    int WA_cnt = 0;
    rep(i, 0, K) {
      AK_cnt += (R[i] == 'A');
      WK_cnt += (R[i] == 'W');
      XK_cnt += (R[i] == '?');
    }
    rep(i, 0, N) {
      WA_cnt += (R[i] == 'W');
    }
    if (AK_cnt == K) {
      rep(i, K, N) {
        R[i] = 'A';
      }
      cout << R << endl;
    } else if (XK_cnt != 1) {
      cout << R << endl;
    } else if (WK_cnt == 0 && WA_cnt != 0) {
      rep(i, 0, K) {
        if (R[i] == '?') {
          R[i] = 'W';
        }
      }
      cout << R << endl;
    } else {
      cout << R << endl;
    }
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