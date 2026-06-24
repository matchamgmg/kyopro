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

  ll H, W, N;
  cin >> H >> W >> N;
  set<pair<int, int>> st;
  int a, b;
  rep(i, 0, N) {
    cin >> a >> b;
    a--;
    b--;
    st.emplace(a, b);
  }

  pair<int, int> e = {-1, -1};

  vector<vector<pair<int, int>>> left_up(H, vector<pair<int, int>>(W, e));

  rep(i, 0, H) {
    rep(j, 0, W) {
      if (st.contains({i, j})) {
        continue;
      }
      if (i == 0 || j == 0) {
        left_up[i][j] = {i, j};
      } else {
        if (left_up[i - 1][j - 1] != e && left_up[i - 1][j] != e && left_up[i][j - 1] != e) {
          left_up[i][j] = {max(left_up[i - 1][j].first, left_up[i - 1][j - 1].first),
                           max(left_up[i][j - 1].second, left_up[i - 1][j - 1].second)};
        } else {
          left_up[i][j] = {i, j};
        }
      }
    }
  }

  ll ans = 0;
  rep(i, 0, H) {
    rep(j, 0, W) {
      if (st.contains({i, j})) {
        continue;
      }
      auto p = left_up[i][j];
      ans += min(i - p.first, j - p.second) + 1;
    }
  }
  cout << ans << endl;
}