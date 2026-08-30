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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }

  auto ave = [&]() -> double {
    auto f = [&](double m) -> bool {
      double dp_0 = 0, dp_1 = -1e18;
      vector<double> B(N);
      rep(i, 0, N) {
        B[i] = A[i] - m;
      }
      rep(i, 0, N) {
        double old_0 = -1e18, old_1 = -1e18;
        swap(dp_0, old_0);
        swap(dp_1, old_1);
        chmax(dp_0, old_0 + B[i]);
        chmax(dp_0, old_1 + B[i]);
        chmax(dp_1, old_0);
      }
      return max(dp_0, dp_1) > -1e-10;
    };
    double ok = 0, ng = 1e10;
    while (ng - ok > 1e-5) {
      double mid = (ok + ng) / 2;
      (f(mid) ? ok : ng) = mid;
    }
    return ok;
  };

  auto medi = [&]() -> int {
    auto f = [&](int m) -> bool {
      vector<int> B(N);
      rep(i, 0, N) {
        B[i] = (A[i] >= m ? 1 : -1);
      }
      int dp_0 = 0, dp_1 = -1e9;
      rep(i, 0, N) {
        int old_0 = -1e9, old_1 = -1e9;
        swap(dp_0, old_0);
        swap(dp_1, old_1);
        chmax(dp_0, old_0 + B[i]);
        chmax(dp_0, old_1 + B[i]);
        chmax(dp_1, old_0);
      }
      return max(dp_0, dp_1) > 0;
    };
    int ok = 0, ng = 1e9 + 1;
    while (ng - ok > 1) {
      int mid = (ok + ng) / 2;
      (f(mid) ? ok : ng) = mid;
    }
    return ok;
  };

  cout << ave() << endl;
  cout << medi() << endl;
}