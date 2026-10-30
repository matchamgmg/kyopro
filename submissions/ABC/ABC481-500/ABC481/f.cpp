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

const ll MOD = 998244353;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  struct Edge {
    int to;
    int w;
  };

  vector<vector<Edge>> G(N);
  rep(i, 0, M) {
    int a, b, c;
    cin >> a >> b >> c;
    a--, b--;
    G[a].emplace_back(b, c);
  }

  const int INF = 1e9;
  vector<int> dist(N, INF);
  queue<int> que;
  dist[0] = 0;
  que.emplace(0);
  while (!que.empty()) {
    int u = que.front();
    int d = dist[u];
    que.pop();
    for (auto &e : G[u]) {
      if (dist[e.to] != INF) {
        continue;
      }
      dist[e.to] = d + 1;
      que.emplace(e.to);
    }
  }

  vector<vector<int>> layer(N);
  vector<set<int>> st(N);
  rep(i, 0, N) {
    layer[dist[i]].push_back(i);
    st[dist[i]].insert(i);
  }

  vector<ll> cost(N, 0);
  vector<int> rank(N, -1);

  rank[0] = 0;
  cost[0] = 0;
  rep(i, 0, N - 1) {
    if (layer[i + 1].size() == 0) {
      break;
    }
    pq_g<pair<int, int>> que;
    rep(j, 0, layer[i].size()) {
      que.emplace(rank[layer[i][j]], layer[i][j]);
    }
    map<int, pair<int, int>> mp;
    while (!que.empty()) {
      auto p = que.top();
      que.pop();
      int u = p.second;
      int r = p.first;
      for (auto &e : G[u]) {
        if (!st[i + 1].contains(e.to)) {
          continue;
        }
        int nr = r * 10 + e.w;
        if (mp.contains(e.to)) {
          if (mp[e.to].first > nr) {
            mp[e.to] = {nr, u};
          }
        } else {
          mp[e.to] = {nr, u};
        }
      }
    }
    vector<pair<pair<int, int>, int>> temp;
    for (auto &[v, p] : mp) {
      temp.emplace_back(p, v);
    }
    sort(all(temp));
    int max_nr = 0;
    int nr = -1;
    for (auto &p : temp) {
      if (max_nr < p.first.first) {
        nr++;
        max_nr = p.first.first;
      }
      rank[p.second] = nr;
      cost[p.second] = (cost[p.first.second] * 10 + p.first.first % 10) % MOD;
    }
  }
  rep(i, 1, N) {
    cout << cost[i] << ' ';
  }
  cout << endl;
}