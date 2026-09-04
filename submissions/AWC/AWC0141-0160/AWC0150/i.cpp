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

  int N, M;
  cin >> N >> M;
  struct Offer {
    int w;
    int k;
    vector<int> P;
  };

  vector<Offer> offers(M);
  vector<int> ans(M);
  int max_w = 0;
  rep(i, 0, M) {
    int w, k;
    cin >> w >> k;
    vector<int> P(k);
    rep(j, 0, k) {
      cin >> P[j];
      P[j]--;
    }
    offers[i] = {w, k, P};
    chmax(max_w, w);
  }

  auto f = [&](int m, int t) -> bool {
    vector<vector<int>> G(N);
    rep(i, 0, t + 1) {
      if (offers[i].w <= m) {
        continue;
      }
      rep(j, 0, offers[i].k - 1) {
        G[offers[i].P[j]].push_back(offers[i].P[j + 1]);
      }
    }

    vector<bool> seen(N, false);
    vector<bool> visited(N, false);
    auto dfs = [&](auto g, int u) -> bool {
      if (visited[u]) {
        return false;
      }
      if (seen[u]) {
        return true;
      }
      seen[u] = true;
      bool res = false;
      for (int v : G[u]) {
        res |= g(g, v);
      }
      visited[u] = true;
      return res;
    };

    bool res = false;
    rep(i, 0, N) {
      res |= dfs(dfs, i);
    }
    return !res;
  };

  int ng = -1;
  rep(i, 0, M) {
    int ok = max_w;
    if (i >= 1) {
      ng = ans[i - 1] - 1;
    }
    while (ok - ng > 1) {
      int mid = (ok + ng) / 2;
      (f(mid, i) ? ok : ng) = mid;
    }
    ans[i] = ok;
  }
  v_cout(ans);

  vector<vector<int>> G(N);
  rep(i, 0, M) {
    if (offers[i].w <= ans[M - 1]) {
      continue;
    }
    rep(j, 0, offers[i].k - 1) {
      G[offers[i].P[j]].push_back(offers[i].P[j + 1]);
    }
  }

  vector<int> topo;
  vector<bool> seen(N, false);
  auto g = [&](auto g, int u) -> void {
    if (seen[u]) {
      return;
    }
    seen[u] = true;
    for (int v : G[u]) {
      g(g, v);
    }
    topo.push_back(u + 1);
    return;
  };
  rep(i, 0, N) {
    g(g, i);
  }
  reverse(all(topo));
  v_cout(topo);
}
