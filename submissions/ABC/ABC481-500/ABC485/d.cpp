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
  string S;
  cin >> S;
  vector<vector<int>> SABC(3, vector<int>(N + 1, 0));
  vector<vector<int>> RSABC(3, vector<int>(N + 1, 0));
  rep(i, 0, N) {
    rep(j, 0, 3) {
      if (S[i] - 'A' == j) {
        SABC[j][i + 1] = SABC[j][i] + 1;
      } else {
        SABC[j][i + 1] = SABC[j][i];
      }
    }
  }
  rrep(i, 0, N) {
    rep(j, 0, 3) {
      if (S[i] - 'A' == j) {
        RSABC[j][i] = RSABC[j][i + 1] + 1;
      } else {
        RSABC[j][i] = RSABC[j][i + 1];
      }
    }
  }

  int L = 0;
  while (L <= N && (SABC[0][L] <= 0 || SABC[1][L] <= 0 || SABC[2][L] <= 0)) {
    L++;
  }
  int R = N;
  while (R >= 0 && (RSABC[0][R] <= 0 || RSABC[1][R] <= 0 || RSABC[2][R] <= 0)) {
    R--;
  }

  ll ans = 0;
  vector<int> cnt(3, 0);
  int M = L;
  while (M <= R) {
    rep(j, 0, 3) {
      cnt[j] = SABC[j][M] - SABC[j][L];
    }
    if (cnt[0] > 0 && cnt[1] > 0 && cnt[2] > 0) {
      break;
    }
    M++;
  }
  rep(i, L, R + 1) {
    while (M <= R && (cnt[0] <= 0 || cnt[1] <= 0 || cnt[2] <= 0)) {
      rep(j, 0, 3) {
        cnt[j] += (S[M] - 'A' == j);
      }
      M++;
    }
    ans += (R - M + 1);
    rep(j, 0, 3) {
      cnt[j] -= (S[i] - 'A' == j);
    }
  }
  cout << ans << endl;
}