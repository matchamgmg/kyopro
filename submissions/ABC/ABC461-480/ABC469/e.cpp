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

  int N, K;
  cin >> N >> K;
  string S;
  cin >> S;

  vector<int> A(N + 1, 0);
  rep(i, 0, N) { A[i + 1] = A[i] + (S[i] == 'o'); }

  auto f = [&](long double p) -> bool {
    bool res = false;
    int l = 0;
    long double el = N;
    rep(r, 1, N + 1) {
      if (A[r] - A[l] < K) {
        continue;
      }
      long double er = A[r] - r * p;
      while (A[r] - A[l] >= K) {
        chmin(el, A[l] - l * p);
        if (el <= er) {
          res = true;
        }
        l++;
      }
    }
    return res;
  };

  long double ok = 0, ng = 1;
  while (ng - ok > 1e-9) {
    long double mid = (ok + ng) / 2;
    (f(mid) ? ok : ng) = mid;
  }
  cout << fixed << setprecision(10) << ok << endl;
}