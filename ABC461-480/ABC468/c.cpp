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
  vector<int> P(N), Q(N);
  rep(i, 0, N) { cin >> P[i]; }
  rep(i, 0, N) { cin >> Q[i]; }

  auto f = [&](vector<int> &V) -> bool {
    bool res1 = false;
    rep(i, 0, N) {
      if (V[i] > P[i]) {
        res1 = true;
        break;
      } else if (V[i] < P[i]) {
        res1 = false;
        break;
      }
    }
    bool res2 = false;
    rep(i, 0, N) {
      if (V[i] > Q[i]) {
        res2 = false;
        break;
      } else if (V[i] < Q[i]) {
        res2 = true;
        break;
      }
    }
    return (res1 && res2);
  };

  vector<int> A(N);
  iota(all(A), 1);
  int ans = 0;
  do {
    ans += f(A);
  } while (next_permutation(all(A)));
  cout << ans << endl;
}