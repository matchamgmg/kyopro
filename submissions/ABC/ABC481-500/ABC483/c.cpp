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

  int N, M;
  cin >> N >> M;
  vector<int> A(M), B(M);
  rep(i, 0, M) {
    cin >> A[i] >> B[i];
    A[i]--;
    B[i]--;
  }
  vector<vector<int>> X(N);
  vector<int> cnt(N, 0);
  rep(i, 0, M) {
    X[A[i]].push_back(i);
    cnt[A[i]]++;
  }
  pq<pair<int, int>> que;
  rep(i, 0, N) {
    que.emplace(cnt[i], -i);
  }
  rep(k, 0, N) {
    for (int i : X[k]) {
      cnt[B[i]]++;
      que.emplace(cnt[B[i]], -B[i]);
    }
    cnt[k] = 0;
    auto p = que.top();
    while (p.first != cnt[-p.second]) {
      que.pop();
      p = que.top();
    }
    cout << -p.second + 1 << endl;
    for (int i : X[k]) {
      cnt[B[i]]--;
    }
    cnt[k] = X[k].size();
    que.emplace(cnt[k], -k);
  }
}