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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<ll> H(N), L(N);
  rep(i, 0, N) { cin >> H[i] >> L[i]; }
  vector<int> idx(N);
  iota(all(idx), 0);
  sort(all(idx), [&](int i, int j) { return L[i] == L[j] ? H[i] < H[j] : L[i] < L[j]; });
  int Q;
  cin >> Q;
  vector<ll> query(Q);
  rep(q, 0, Q) { cin >> query[q]; }
  vector<int> qidx(Q);
  iota(all(qidx), 0);
  sort(all(qidx), [&](int i, int j) { return query[i] < query[j]; });
  reverse(all(qidx));
  vector<ll> ans(Q);
  ll a = -1;
  int i = N - 1;
  for (int q : qidx) {
    ll t = query[q];
    while (i >= 0 && L[idx[i]] > t) {
      chmax(a, H[idx[i]]);
      i--;
    }
    ans[q] = a;
  }
  rep(q, 0, Q) { cout << ans[q] << endl; }
}