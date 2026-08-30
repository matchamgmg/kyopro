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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, K;
  cin >> N >> K;
  vector<int> X(N), A(N);
  rep(i, 0, N) {
    cin >> X[i];
    X[i]--;
  }
  rep(i, 0, N) {
    cin >> A[i];
  }
  vector<vector<int>> doublingX;
  doublingX.emplace_back(X);
  ll nibeki = 1;
  ll d = 0;
  while (nibeki < K) {
    rep(i, 0, N) {
      X[i] = doublingX[d][doublingX[d][i]];
    }
    doublingX.emplace_back(X);
    nibeki <<= 1;
    d++;
  }

  vector<int> ans(N);
  rep(i, 0, N) {
    int u = i;
    ll k = K;
    d = 0;
    while (k) {
      if (k & 1) {
        u = doublingX[d][u];
      }
      k >>= 1;
      d++;
    }
    ans[i] = A[u];
  }
  rep(i, 0, N) {
    cout << ans[i] << ' ';
  }
  cout << endl;
}