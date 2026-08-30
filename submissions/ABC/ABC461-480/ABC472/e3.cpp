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
  int N, M;
  cin >> N >> M;
  vector<vector<int>> G(N);
  rep(i, 0, M) {
    int a, b;
    cin >> a >> b;
    a--, b--;
    G[a].emplace_back(b);
    G[b].emplace_back(a);
  }

  vector<int> color(N, -1);
  stack<int> st;
  auto dfs = [&](auto f, int u, int c) -> bool {
    bool res = false;
    if (color[u] != -1) {
      if (color[u] == c) {
        return false;
      } else {
        st.push(u);
        return true;
      }
    }
    color[u] = c;
    c ^= 1;
    st.push(u);
    for (int v : G[u]) {
      res |= f(f, v, c);
      if (res) {
        break;
      }
    }
    if (!res) {
      st.pop();
    }
    return res;
  };

  dfs(dfs, 0, 0);
  if (st.empty()) {
    cout << -1 << endl;
    return;
  }
  vector<int> odd_cycle;
  odd_cycle.push_back(st.top());
  st.pop();
  while (!st.empty() && st.top() != odd_cycle.front()) {
    odd_cycle.push_back(st.top());
    st.pop();
  }
  cout << odd_cycle.size() << endl;
  for (int u : odd_cycle) {
    cout << u + 1 << ' ';
  }
  cout << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}