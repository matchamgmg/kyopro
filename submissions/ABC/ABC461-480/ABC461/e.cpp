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

struct S {
  long long val;
  int siz;
  S() {}
  S(long long v, int s) : val(v), siz(s) {}
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, Q;
  cin >> N >> Q;

  fenwick_tree<ll> ftR(Q), ftC(Q);
  vector<vector<int>> last(N, vector<int>(2, -1));
  ll ans = 0;
  rep(q, 0, Q) {
    int t;
    cin >> t;
    t--;
    int x;
    cin >> x;
    x--;

    if (t == 0) {
      if (last[x][t] == -1) {
        ans += N;
      } else {
        ans += ftC.sum(last[x][t], Q);
      }

      if (last[x][t] != -1) {
        ftR.add(last[x][t], -1);
      }
      ftR.add(q, 1);
      last[x][t] = q;
    } else {
      if (last[x][t] == -1) {
        ans -= ftR.sum(0, Q);
      } else {
        ans -= ftR.sum(last[x][t], Q);
      }
      if (last[x][t] != -1) {
        ftC.add(last[x][t], -1);
      }
      ftC.add(q, 1);
      last[x][t] = q;
    }
    cout << ans << endl;
  }
}