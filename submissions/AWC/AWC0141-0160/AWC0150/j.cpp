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

  int N, M;
  cin >> N >> M;
  vector<int> S(M), D(M);
  rep(i, 0, M) {
    cin >> S[i] >> D[i];
    S[i]--;
  }
  vector<int> yatai(M, 0);
  vector<vector<bool>> dp(1 << M, vector<bool>(N, false));
  dp[0][0] = true;
  int ans = 0;
  while (true) {
    ans++;
    rep(i, 0, M) {
      yatai[i] = (S[i] + ans * D[i]) % N;
    }
    vector<vector<bool>> old_dp(1 << M, vector<bool>(N, false));
    swap(dp, old_dp);
    rep(bit, 0, 1 << M) {
      rep(i, 0, N) {
        if (!old_dp[bit][i]) {
          continue;
        }
        rep(j, -1, 2) {
          int ni = i + j;
          if (!(0 <= ni && ni < N)) {
            continue;
          }
          int nbit = bit;
          rep(k, 0, M) {
            if (ni == yatai[k]) {
              nbit = nbit | (1 << k);
            }
          }
          dp[nbit][ni] = true;
          if (nbit == (1 << M) - 1) {
            cout << ans << endl;
            return 0;
          }
        }
      }
    }
  }
}