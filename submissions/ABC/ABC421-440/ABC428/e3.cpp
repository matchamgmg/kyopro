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

  struct Node {
    int dist;
    int from;
    Node() : dist(-1), from(-1) {}
    Node(int f) : dist(0), from(f) {}
    Node(int d, int f) : dist(d), from(f) {}
    bool operator<(const Node &rhs) const { return dist == rhs.dist ? from < rhs.from : dist < rhs.dist; }
    bool operator>(const Node &rhs) const { return dist == rhs.dist ? from > rhs.from : dist > rhs.dist; }
    Node operator+(int x) { return Node(dist + x, from); }
  };

  int N;
  cin >> N;
  vector<vector<int>> G(N);
  int U, V;
  rep(i, 0, N - 1) {
    cin >> U >> V;
    U--;
    V--;
    G[U].emplace_back(V);
    G[V].emplace_back(U);
  }

  vector<vector<Node>> plane(N), left(N), right(N);
  vector<Node> dp(N);

  auto dfs1 = [&](auto f, int u, int p) -> Node {
    Node res = Node(u);
    int cnt = 0;
    for (int v : G[u]) {
      if (v == p) {
        plane[u].emplace_back(Node(u));
      } else {
        plane[u].emplace_back(f(f, v, u) + 1);
      }
      chmax(res, plane[u][cnt]);
      cnt++;
    }
    return res;
  };

  auto dfs2 = [&](auto f, int u, int p, Node n) -> void {
    int siz = G[u].size();
    left[u].assign(siz + 1, Node(u));
    right[u].assign(siz + 1, Node(u));
    int cnt = 0;
    for (int v : G[u]) {
      if (v == p) {
        plane[u][cnt] = n;
      }
      left[u][cnt + 1] = max(left[u][cnt], plane[u][cnt]);
      cnt++;
    }
    rep(i, 0, siz) { right[u][siz - i - 1] = max(right[u][siz - i], plane[u][siz - i - 1]); }

    dp[u] = left[u][siz];

    cnt = 0;
    for (int v : G[u]) {
      if (v == p) {
        cnt++;
        continue;
      }
      Node arg = max(left[u][cnt], right[u][cnt + 1]);
      f(f, v, u, arg + 1);
      cnt++;
    }
  };

  dfs1(dfs1, 0, -1);
  dfs2(dfs2, 0, -1, Node(-1));

  rep(i, 0, N) { cout << dp[i].from + 1 << endl; }
}