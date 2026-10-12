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

const ll INF = 1e18;
struct S {
  int num;
  ll prefix;
  ll left;
  ll middle;
  ll right;
  ll suffix;
  ll best, second_best;
};

S op(const S &a, const S &b) {
  S res;
  if (a.num == 0 && b.num == 0) {
    res.num = 0;
    res.left = 0;
    res.right = 0;
    res.middle = 0;
    res.prefix = a.prefix + b.prefix;
    res.suffix = res.prefix;
    res.best = 0;
    res.second_best = 0;
    return res;
  }
  if (a.num + b.num == 1) {
    res.num = 1;
    res.left = max(a.left, b.left);
    res.right = res.left;
    res.mid = 0;
    res.best = res.left;
    res.second_best = 0;
    if (a.num == 0) {
      res.prefix = a.suffix + b.prefix;
      res.suffix = b.suffix;
    } else {
      res.prefix = a.prefix;
      res.suffix = a.suffix + b.prefix;
    }
    return res;
  }
  if (a.num == 1 && b.num == 1) {
    res.prefix = a.prefix;
    res.suffix = b.suffix;
    res.middle = a.suffix + b.prefix;
    if (res.middle >= 0) {
      res.num = 1;
      res.left = a.left + res.middle + b.right;
      res.right = res.left;
      res.middle = 0;
      res.best = res.left;
      res.second_best = 0;
    } else {
      res.num = 2;
      res.left = a.left;
      res.right = b.right;
      res.best = max(res.left, res.right);
      res.second_best = min(res.left, res.right);
    }
    return res;
  }
  if (a.num + b.num == 2) {
    res.num = 1;
    if (a.num == 0) {
      res.left = b.left;
      res.right = b.right;
      res.mid = b.mid;
      res.prefix = a.suffix + b.prefix;
      res.suffix = b.suffix;
      res.best = b.best;
      res.second_best = b.second_best;
    } else {
      res.left = a.left;
      res.right = a.right;
      res.mid = a.mid;
      res.prefix = a.prefix;
      res.suffix = a.suffix + b.prefix;
      res.best = a.best;
      res.second_best = a.second_best;
    }
    return res;
  }
  if (a.num + b.num == 3) {
    res.num = 2;
    if (a.num == 1) {
      res.middle = a.suffix + b.prefix;
      if (res.middle >= max(-a.right, max(-b.left, -b.right))) {
        res.prefix = a.prefix;
        res.suffix = b.suffix;
        res.left = a.right + res.middle + b.left;
        res.right = b.right;
        res.middle = b.middle;
        res.best = max(res.left, res.right);
        res.second_best = min(res.left, res.right);
      } else {
        if (a.right < b.second_best) {
          res.prefix = a.prefix + a.right + a.suffix + b.prefix;
          res.suffix = b.suffix;
          res.left = b.left;
          res.right = b.right;
          res.middle = b.middle;
          res.best = b.best;
          res.second_best = b.second_best;
        } else if (b.left > b.right) {
          res.prefix = a.prefix;
          res.suffix = b.middle + b.right + b.suffix;
          res.left = a.right;
          res.right = b.left;
          res.best = max(res.left, res.right);
          res.second_best = min(res.left, res.right);
        } else {
          res.prefix = a.prefix;
          res.suffix = b.suffix;
          res.left = a.right;
          res.right = b.right;
          res.middle = a.suffix + b.prefix + b.left + b.middle;
          res.best = max(res.left, res.right);
          res.second_best = min(res.left, res.right);
        }
      }
    } else {
      res.middle = a.suffix + b.prefix;
      if (res.middle >= max(-b.left, max(-a.left, -a.right))) {
        res.prefix = a.prefix;
        res.suffix = b.suffix;
        res.left = a.left;
        res.right = a.right + res.middle + b.left;
        res.middle = a.middle;
        res.best = max(res.left, res.right);
        res.second_best = min(res.left, res.right);
      } else {
        if (b.left < a.second_best) {
          res.prefix = a.prefix;
          res.suffix = b.suffix + b.left + b.prefix + a.suffix;
          res.left = a.left;
          res.right = a.right;
          res.middle = a.middle;
          res.best = a.best;
          res.second_best = a.second_best;
        } else if (a.left > a.right) {
          res.prefix = a.prefix;
          res.suffix = b.suffix;
          res.middle = a.middle + a.right + a.suffix + b.prefix;
          res.left = a.left;
          res.right = b.left;
          res.best = max(res.left, res.right);
          res.second_best = min(res.left, res.right);
        } else {
          res.prefix = a.prefix + a.left + a.middle;
          res.suffix = b.suffix;
          res.left = a.right;
          res.right = b.left;
          res.best = max(res.left, res.right);
          res.second_best = min(res.left, res.right);
        }
      }
    }
    return res;
  }
  if (a.num == 2 && b.num == 2) {
    res.num = 2;
    res.middle = a.suffix + b.prefix;
  }
}
S e() {
  return S{0, 0, 0, 0, 0, 0, 0, 0};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
}