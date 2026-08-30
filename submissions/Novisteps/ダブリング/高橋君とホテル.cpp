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

  int N;
  cin >> N;
  vector<ll> X(N + 2);
  rep(i, 0, N) {
    cin >> X[i + 1];
  }
  ll L, Q;
  cin >> L >> Q;
  X[0] = X[1] - L;
  X[N + 1] = X[N] + L;
  // 右左にできるだけ進む貪欲
  vector<int> to_right(N + 2), to_left(N + 2);
  rep(i, 0, N + 2) {
    int r = upper_bound(all(X), X[i] + L) - X.begin();
    to_right[i] = r - 1;
  }
  rep(i, 0, N + 2) {
    int l =
        upper_bound(rall(X), X[i] - L, [&](ll e, ll val) { return e > val; }) -
        X.rbegin();
    to_left[i] = N + 2 - l;
  }
  //   v_cout(to_right);
  //   v_cout(to_left);

  vector<vector<int>> doublingR, doublingL;
  doublingR.emplace_back(to_right);
  doublingL.emplace_back(to_left);
  ll nibeki = 1;
  ll d = 0;
  vector<int> next_R(N + 2), next_L(N + 2);
  while (nibeki < N + 2) {
    rep(i, 0, N + 2) {
      next_R[i] = doublingR[d][doublingR[d][i]];
      next_L[i] = doublingL[d][doublingL[d][i]];
    }
    doublingR.emplace_back(next_R);
    doublingL.emplace_back(next_L);
    nibeki <<= 1;
    d++;
  }
  ll D = d;

  // X[a]から始めて，X[b]を超えない最遠のホテル
  rep(q, 0, Q) {
    int a, b;
    cin >> a >> b;
    ll ans = 0;
    if (a < b) {
      rrep(d, 0, D + 1) {
        int v = doublingR[d][a];
        if (X[v] > X[b]) {
          continue;
        } else {
          a = v;
          ans += (1LL << d);
        }
      }
      if (a == b) {
        cout << ans << endl;
      } else {
        cout << ans + 1 << endl;
      }
    } else {
      rrep(d, 0, D + 1) {
        int v = doublingL[d][a];
        if (X[v] < X[b]) {
          continue;
        } else {
          a = v;
          ans += (1LL << d);
        }
      }
      if (a == b) {
        cout << ans << endl;
      } else {
        cout << ans + 1 << endl;
      }
    }
  }
}