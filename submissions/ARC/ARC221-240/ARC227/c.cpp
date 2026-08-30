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

// Sの周期を見る
// Sが長さLの周期TのK回の繰り返しで表現されるとする
// このとき，別の周期に属する人S_{i%L}は常に同じ距離移動するため，合流不可能
// Sを一回操作列として行ったとき
// S_{i%L == L-1}の人はちょうど一周して元に戻ってくる
// それ以外の人はN+1以上移動するが，S_{i%L == L-1}の人を追い越すことはない
// 多くともN回Sを繰り返せばすべての人がS_{i%L == L-1}に合流する

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  string S;
  cin >> S;
  string SS = S + S;
  int K = 0;
  rep(i, 1, N + 1) {
    string T = SS.substr(i, N);
    if (S == T) {
      K++;
    }
  }
  cout << K << endl;
  cout << N * N << endl;
  rep(i, 0, N) {
    cout << S;
  }
  cout << endl;
}