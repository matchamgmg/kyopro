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

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
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

void solve() {
  string S;
  cin >> S;
  int N = S.size();
  vector<int> cnt(26);
  rep(i, 0, N) { ++cnt[S[i] - 'a']; }
  vector<int> idx(26);
  iota(all(idx), 0);
  sort(all(idx), [&](int i, int j) { return cnt[i] > cnt[j]; });
  int M = cnt[idx[0]];
  vector<string> T(M, "");
  int p = 0;
  for (int i : idx) {
    rep(j, 0, cnt[i]) {
      T[p] += i + 'a';
      p = (p + 1) % M;
    }
  }
  bool ans = true;
  if (cnt[idx[1]] == 0) {
    if (M == 1) {
      cout << "Yes" << endl;
      cout << S << endl;
    } else {
      cout << "No" << endl;
    }
    return;
  } else if (T[M - 1].back() == (idx[0] + 'a') && T[M - 2].back() == (idx[0] + 'a')) {
    ans = false;
  }
  YN(ans);
  if (ans) {
    rep(i, 0, M) { cout << T[i]; }
    cout << endl;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (; T--;)
    solve();
}