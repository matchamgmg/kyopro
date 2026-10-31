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

  int N;
  cin >> N;
  vector<vector<int>> G(N);
  rep(i, 0, N - 1) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    G[u].push_back(v);
    G[v].push_back(u);
  }
  vector<int> C(N);
  rep(i, 0, N) {
    cin >> C[i];
    C[i]--;
  }

  const int INF = 1e9;
  vector<int> dist1(N, INF);
  vector<int> dist2(N, INF);
  auto BFS = [&](vector<int> &dist, int s) -> void {
    queue<int> que;
    dist[s] = 0;
    que.emplace(s);
    while (!que.empty()) {
      int u = que.front();
      que.pop();
      for (int v : G[u]) {
        if (dist[v] < INF)
          continue;
        dist[v] = dist[u] + 1;
        que.emplace(v);
      }
    }
  };

  BFS(dist1, 0);
  int s = max_element(all(dist1)) - dist1.begin();
  BFS(dist2, s);
  int max_d = *max_element(all(dist2));
  // cout << max_d << endl;
  int t = -1;
  rep(i, 0, N) {
    if (G[i].size() > 1) {
      t = i;
    }
  }

  if (t == -1) {
    if (C[0] == C[1]) {
      cout << 1 << endl;
    } else {
      cout << 0 << endl;
    }
    return 0;
  }

  struct S {
    ll siz = 0;
    ll ans = 0;
    map<pair<int, int>, ll> mp;
  };

  auto merge = [&](S &a, S &b, int d) -> void {
    if (a.siz < b.siz) {
      swap(a, b);
    }
    a.siz += b.siz;
    a.ans += b.ans;
    for (auto &[k, v] : b.mp) {
      a.ans += v * a.mp[{max_d + 2 * d - k.first, k.second}];
      // cout << max_d + 2 * d - k.first << endl;
      if (a.mp[{max_d + 2 * d - k.first, k.second}] == 0) {
        a.mp.erase({max_d + 2 * d - k.first, k.second});
      }
    }
    for (auto &[k, v] : b.mp) {
      a.mp[k] += v;
    }
  };

  vector<S> ans(N);
  auto DFS = [&](auto f, int u, int d, int p) -> void {
    int cnt = 0;
    for (int v : G[u]) {
      if (v == p) {
        continue;
      }
      cnt++;
      f(f, v, d + 1, u);
      merge(ans[u], ans[v], d);
    }
    if (cnt == 0) {
      ans[u].mp[{d, C[u]}]++;
      ans[u].siz++;
    }
    return;
  };

  DFS(DFS, t, 0, -1);
  cout << ans[t].ans << endl;
}