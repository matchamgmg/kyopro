/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

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

void solve() {
  struct IWAI {
    ll H;
    ll A;
    ll S;
  };

  vector<IWAI> iwai(2);
  rep(i, 0, 2) {
    ll h, a, s;
    cin >> h >> a >> s;
    iwai[i] = {h, a, s};
  }

  map<pair<ll, ll>, mint> mp;
  auto dfs = [&](auto f, ll HC, ll HG) -> mint {
    if (HC == 0) {
      return mint(0);
    }
    if (HG == 0) {
      return mint(1);
    }
    if (mp.contains({HC, HG})) {
      return mp[{HC, HG}];
    }
    if (HC > iwai[1].A && HG > iwai[0].A) {
      return f(f, HC - iwai[1].A, HG - iwai[0].A);
    }
    if (iwai[0].S > iwai[1].S) {
      HG = max(0LL, HG - iwai[0].A);
      if (HG > 0) {
        // HC < A
        return f(f, 1, HG) * mint(10).inv() + f(f, 0, HG) * mint(10).inv() * mint(9);
      } else {
        HC = max(0LL, HC - iwai[1].A);
        if (HC > 0) {
          return mint(10).inv() * mint(9) + f(f, HC, 1) * mint(10).inv();
        } else {
          return mint(10).inv() * mint(9) + f(f, 1, 1) * mint(10).inv() * mint(10).inv();
        }
      }
    } else if (iwai[0].S < iwai[1].S) {
      HC = max(0LL, HC - iwai[1].A);
      if (HC > 0) {
        // HG < A
        return f(f, HC, 1) * mint(10).inv() + f(f, HC, 0) * mint(10).inv() * mint(9);
      } else {
        HG = max(0LL, HG - iwai[0].A);
        if (HG > 0) {
          return mint(10).inv() * mint(9) + f(f, 1, HG) * mint(10).inv();
        } else {
          return mint(10).inv() * mint(9) + f(f, 1, 1) * mint(10).inv() * mint(10).inv();
        }
      }
    } else {
      //
      mint res = 0;
      HG = max(0LL, HG - iwai[0].A);
      if (HG > 0) {
        // HC < A
        res += f(f, 1, HG) * mint(10).inv() + f(f, 0, HG) * mint(10).inv() * mint(9);
      } else {
        HC = max(0LL, HC - iwai[1].A);
        if (HC > 0) {
          res += mint(10).inv() * mint(9) + f(f, HC, 1) * mint(10).inv();
        } else {
          res += mint(10).inv() * mint(9) + f(f, 1, 1) * mint(10).inv() * mint(10).inv();
        }
      }
      //
      HC = max(0LL, HC - iwai[1].A);
      if (HC > 0) {
        // HG < A
        res += f(f, HC, 1) * mint(10).inv() + f(f, HC, 0) * mint(10).inv() * mint(9);
      } else {
        HG = max(0LL, HG - iwai[0].A);
        if (HG > 0) {
          res += mint(10).inv() * mint(9) + f(f, 1, HG) * mint(10).inv();
        } else {
          res += mint(10).inv() * mint(9) + f(f, 1, 1) * mint(10).inv() * mint(10).inv();
        }
      }
      return res * mint(2).inv();
    }
  };

  dfs(dfs, iwai[0].H, iwai[1].H);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}