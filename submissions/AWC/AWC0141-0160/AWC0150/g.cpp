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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<int> T(N);
  rep(i, 0, N) {
    cin >> T[i];
    T[i]--;
  }

  vector<int> st;
  vector<bool> seen(N, false);
  vector<bool> visited(N, false);
  vector<bool> seen2(N, false);
  vector<int> ans(N, 0);
  auto cycle = [&](auto f, int u) -> bool {
    if (visited[u]) {
      return false;
    }
    if (seen[u]) {
      st.push_back(u);
      return true;
    }
    seen[u] = true;
    st.push_back(u);
    bool res = false;
    res |= f(f, T[u]);
    if (!res) {
      st.pop_back();
    }
    visited[u] = true;
    return res;
  };

  rep(i, 0, N) {
    if (cycle(cycle, i)) {
      int len = 0;
      vector<int> cyc;
      int a = st.back();
      cyc.push_back(a);
      seen2[a] = true;
      st.pop_back();
      len++;
      while (!st.empty() && st.back() != a) {
        cyc.push_back(st.back());
        seen2[st.back()] = true;
        st.pop_back();
        len++;
      }
      st.clear();
      for (int s : cyc) {
        ans[s] = len;
      }
      if (len == 1) {
        cout << len << endl;
      }
    }
  }

  auto solve = [&](auto f, int u) -> int {
    if (seen2[u]) {
      return ans[u];
    }
    seen2[u] = true;
    ans[u] = f(f, T[u]) + 1;
    return ans[u];
  };

  rep(i, 0, N) {
    solve(solve, i);
  }

  rep(q, 0, Q) {
    int s;
    cin >> s;
    s--;
    cout << ans[s] << endl;
  }
}