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
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x) { cout << (x ? "Yes" : "No") << endl; }
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
bool grid_check(int x, int y, int X, int Y) { return (0 <= x && x < X && 0 <= y && y < Y); }
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
  vector<mint> kaijou(N + 1, 1);
  rep(i, 0, N) { kaijou[i + 1] = kaijou[i] * mint(i + 1); }
  vector<pair<char, int>> RL;
  char last = '.';
  int len = 0;
  rep(i, 0, N) {
    if (S[i] == last) {
      len++;
    } else {
      if (len > 0) {
        RL.emplace_back(last, len);
      }
      last = S[i];
      len = 1;
    }
  }
  if (len > 0) {
    RL.emplace_back(last, len);
  }
  int M = RL.size();
  vector<mint> dp(M + 1, 1);
  last = 'o';
  len = 0;
  rep(i, 0, M) {
    if (RL[i].first == 'o') {
      if (last == 'o') {
        dp[i + 1] = dp[i];
      } else if (last == 'x') {
        mint a = kaijou[RL[i - 1].second + 1];
        rep(j, 1, RL[i - 1].second + 1) { a -= kaijou[j]; }
        dp[i + 1] = dp[i] * a;
      }
    } else {
      dp[i + 1] = dp[i];
    }
  }
  mint ans = dp[M];
  if (RL[M - 1].first == 'x') {
    ans = 0;
  }
  cout << ans.val() << endl;
}