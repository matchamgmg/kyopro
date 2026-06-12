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

  int N, Q;
  cin >> N >> Q;
  vector<ll> A(N);
  rep(i, 0, N) { cin >> A[i]; }
  int M = 5e5 + 1;
  fenwick_tree<ll> ft_val(M), ft_cnt(M);
  rep(i, 0, N) {
    ft_val.add(A[i], A[i]);
    ft_cnt.add(A[i], 1);
  }
  ll T, X, Y;
  ll L, R;
  rep(q, 0, Q) {
    cin >> T;
    if (T == 1) {
      cin >> X >> Y;
      X--;
      ll a = A[X];
      A[X] = Y;
      ft_val.add(a, -a);
      ft_val.add(Y, Y);
      ft_cnt.add(a, -1);
      ft_cnt.add(Y, 1);
    } else {
      cin >> L >> R;
      if (L >= R) {
        cout << L * N << endl;
      } else {
        ll m = ft_val.sum(L, R);
        ll l = L * ft_cnt.sum(0, L);
        ll r = R * ft_cnt.sum(R, M);
        cout << l + m + r << endl;
      }
    }
  }
}