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

struct Operation {
  char d;
  int r, c, h, w;
};

void print_answer(const vector<Operation> &ans) {
  for (const auto &op : ans) {
    cout << op.d << ' ' << op.r << ' ' << op.c << ' ' << op.h << ' ' << op.w << '\n';
  }
}

void printf_answer(const vector<Operation> &ans) {
  ofstream ofs("output.txt");
  for (const auto &op : ans) {
    ofs << op.d << ' ' << op.r << ' ' << op.c << ' ' << op.h << ' ' << op.w << '\n';
  }
  ofs.close();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;

  const int M = N * N;

  vector<vector<int>> a(N, vector<int>(N));
  vector<int> card_at(M);
  vector<int> pos(M);

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cin >> a[i][j];
      int v = i * N + j;
      card_at[v] = a[i][j];
      pos[a[i][j]] = v;
    }
  }

  vector<string> V(N);
  vector<string> H(N - 1);

  for (int i = 0; i < N; ++i)
    cin >> V[i];
  for (int i = 0; i < N - 1; ++i)
    cin >> H[i];

  vector<vector<int>> graph(M);

  auto add_edge = [&](int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
  };

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j + 1 < N; ++j) {
      if (V[i][j] == '0') {
        add_edge(i * N + j, i * N + (j + 1));
      }
    }
  }

  for (int i = 0; i + 1 < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (H[i][j] == '0') {
        add_edge(i * N + j, (i + 1) * N + j);
      }
    }
  }

  /*
   * BFSで全域木を作る。
   */
  vector<int> parent(M, -1);
  vector<vector<int>> tree(M);

  queue<int> que;
  parent[0] = 0;
  que.push(0);

  while (!que.empty()) {
    int v = que.front();
    que.pop();

    for (int to : graph[v]) {
      if (parent[to] != -1)
        continue;

      parent[to] = v;
      tree[v].push_back(to);
      tree[to].push_back(v);
      que.push(to);
    }
  }

  vector<int> degree(M);
  vector<bool> active(M, true);

  queue<int> leaves;

  for (int v = 0; v < M; ++v) {
    degree[v] = tree[v].size();
    if (degree[v] == 1) {
      leaves.push(v);
    }
  }

  vector<Operation> answer;

  auto swap_adjacent = [&](int u, int v) {
    int ur = u / N;
    int uc = u % N;
    int vr = v / N;
    int vc = v % N;

    if (ur == vr) {
      int c = min(uc, vc);
      answer.push_back({'H', ur, c, 1, 2});
    } else {
      int r = min(ur, vr);
      answer.push_back({'V', r, uc, 2, 1});
    }

    int card_u = card_at[u];
    int card_v = card_at[v];

    swap(card_at[u], card_at[v]);
    pos[card_u] = v;
    pos[card_v] = u;
  };

  int remaining = M;

  while (remaining > 1) {
    int leaf = -1;

    while (!leaves.empty()) {
      int v = leaves.front();
      leaves.pop();

      if (active[v] && degree[v] == 1) {
        leaf = v;
        break;
      }
    }

    assert(leaf != -1);

    /*
     * leaf に置くべきカード番号は leaf。
     */
    int start = pos[leaf];

    /*
     * 現在の active な木上で start から leaf への経路を求める。
     */
    vector<int> prev(M, -1);
    queue<int> q;

    prev[start] = start;
    q.push(start);

    while (!q.empty()) {
      int v = q.front();
      q.pop();

      if (v == leaf)
        break;

      for (int to : tree[v]) {
        if (!active[to])
          continue;
        if (prev[to] != -1)
          continue;

        prev[to] = v;
        q.push(to);
      }
    }

    assert(prev[leaf] != -1);

    vector<int> path;
    for (int v = leaf;; v = prev[v]) {
      path.push_back(v);
      if (v == start)
        break;
    }
    reverse(path.begin(), path.end());

    /*
     * 目的カードを path に沿って leaf まで動かす。
     */
    for (int i = 0; i + 1 < (int)path.size(); ++i) {
      swap_adjacent(path[i], path[i + 1]);
    }

    assert(card_at[leaf] == leaf);

    /*
     * leafを木から削除する。
     */
    active[leaf] = false;
    --remaining;

    for (int to : tree[leaf]) {
      if (!active[to])
        continue;

      --degree[to];
      if (degree[to] == 1) {
        leaves.push(to);
      }
    }

    degree[leaf] = 0;
  }

  assert((int)answer.size() <= 100000);
  printf_answer(answer);

  return 0;
}