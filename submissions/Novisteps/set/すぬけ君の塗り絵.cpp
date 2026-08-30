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

  ll H, W, N;
  cin >> H >> W >> N;
  set<pair<int, int>> st;
  rep(i, 0, N) {
    int x, y;
    cin >> x >> y;
    x--;
    y--;
    st.emplace(x, y);
  }
  vector<ll> ans(10, 0);
  vector<set<vector<ll>>> box(10);
  ans[0] = (H - 2) * (W - 2);
  for (auto [x, y] : st) {
    rep(s, -1, 2) {
      rep(t, -1, 2) {
        int cnt = 0;
        bool ok = true;
        vector<ll> id;
        rep(i, -1, 2) {
          rep(j, -1, 2) {
            if (!(0 <= x + s + i && x + s + i < H && 0 <= y + t + j &&
                  y + t + j < W)) {
              ok = false;
              continue;
            }
            if (st.contains({x + s + i, y + t + j})) {
              cnt++;
              id.emplace_back((x + s + i) * W + (y + t + j));
            } else {
              id.emplace_back(-1);
            }
          }
        }
        if (ok) {
          box[cnt].emplace(id);
        }
      }
    }
  }
  rep(i, 1, 10) {
    ans[0] -= box[i].size();
    ans[i] = box[i].size();
  }
  // ll sum = 0;
  rep(i, 0, 10) {
    cout << ans[i] << endl;
    // sum += ans[i];
  }
  // cout << sum << endl;
}