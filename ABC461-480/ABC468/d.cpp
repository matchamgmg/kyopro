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
  string S;
  cin >> S;
  int N = S.size();
  // 奇数長さの良い文字列を取り出す
  ll ans = 0;
  rep(i, 0, N) {
    int cnt = 0;
    ans++;
    rep(j, 1, N) {
      if (i - j < 0 || i + j >= N) {
        continue;
      }
      if (S[i - j] == S[i + j]) {
        ans++;
      } else {
        cnt++;
        if (cnt <= 1) {
          ans++;
        } else {
          break;
        }
      }
    }
  }

  // 偶数長さの良い文字列を取り出す
  rep(i, 0, N - 1) {
    int cnt = 0;
    if (S[i] != S[i + 1]) {
      cnt++;
    }
    ans++;
    rep(j, 1, N) {
      if (i - j < 0 || i + 1 + j >= N) {
        continue;
      }
      if (S[i - j] == S[i + 1 + j]) {
        ans++;
      } else {
        cnt++;
        if (cnt <= 1) {
          ans++;
        } else {
          break;
        }
      }
    }
  }
  cout << ans << endl;
}