#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

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

const bool keta_less(const string &a, const string &b) {
  if (a.size() != b.size()) {
    return a.size() < b.size();
  }
  int i = 0;
  while (i < a.size() && a[i] == b[i]) {
    ++i;
  }
  if (i != a.size()) {
    return a[i] < b[i];
  }
  return false;
}

const bool string_less(const string &a, const string &b) {
  int s = min(a.size(), b.size());
  int i = 0;
  while (i < s && a[i] == b[i]) {
    ++i;
  }
  if (i < s) {
    return a[i] < b[i];
  }
  if (a.size() == b.size()) {
    return false;
  } else {
    return a.size() < b.size();
  }
}

const bool value_less(const string &a, const string &b) {
  ll va = stoll(a);
  ll vb = stoll(b);
  return va < vb;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  cin >> N >> K;
  vector<string> S(N);
  rep(i, 0, N) {
    cin >> S[i];
  }
  priority_queue<string, vector<string>, keta_less> keta_que;
}