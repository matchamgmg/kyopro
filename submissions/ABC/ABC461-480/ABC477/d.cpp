/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

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

  int N, Q;
  cin >> N >> Q;
  struct Query {
    int t;
    int x;
    char c;
  };
  vector<Query> queries(Q);
  rep(q, 0, Q) {
    int t;
    cin >> t;
    if (t == 1) {
      int x;
      cin >> x;
      x--;
      queries[q] = {t, x, '.'};
    } else {
      char c;
      cin >> c;
      queries[q] = {t, -1, c};
    }
  }
  vector<bool> masked(N, false);
  set<int> st1, st2; // 未確定で、マスクされてる
  vector<char> ans(N, '.');
  queue<int> que;

  rep(q, 0, Q) {
    if (queries[q].t == 1) {
      masked[queries[q].x] = masked[queries[q].x] ^ true;
    }
  }
  rep(i, 0, N) {
    if (masked[i]) {
      st1.insert(i);
    } else {
      st2.insert(i);
    }
  }
  rrep(q, 0, Q) {
    if (queries[q].t == 1) {
      int x = queries[q].x;
      masked[x] = masked[x] ^ true;
      if (masked[x]) {
        if (st2.contains(x)) {
          st2.erase(x);
          st1.insert(x);
        }
      } else {
        if (st1.contains(x)) {
          st1.erase(x);
          st2.insert(x);
        }
      }
    } else {
      for (int i : st2) {
        ans[i] = queries[q].c;
      }
      st2.clear();
    }
  }
  for (int i : st2) {
    ans[i] = 'a';
  }
  rep(i, 0, N) {
    cout << ans[i];
  }
  cout << endl;
}