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
    int y;
    int index;
    Node(int n, int idx) : y(n), index(idx) {}
    const bool operator<(const Node &rhs) const { return y == rhs.y ? index < rhs.index : y < rhs.y; }
  };

  // Trie : node_index -> y, node_index
  // Array_to_node_index : x_i -> node_index
  // nodes : node_inedx -> node

  vector<set<Node>> Trie;
  vector<int> Array_to_node_index;
  vector<Node> nodes;
  int node_num = 0;
  nodes.emplace_back(0, node_num);
  Trie.emplace_back();
  Array_to_node_index.emplace_back(node_num++);

  int N;
  cin >> N;

  int x, y;
  rep(i, 0, N) {
    cin >> x >> y;
    int node_index = Array_to_node_index[x];
    if ((*Trie[node_index].lower_bound(Node{y, -1})).y != y) {
      nodes.emplace_back(y, node_num);
      Trie.emplace_back();
      Trie[node_index].emplace(y, node_num);
      Array_to_node_index.emplace_back(node_num++);
    } else {
      Array_to_node_index.emplace_back((*Trie[node_index].lower_bound(Node{y, -1})).index);
    }
  }

  vector<set<int>> Node_to_array(node_num);
  rep(i, 0, N + 1) { Node_to_array[Array_to_node_index[i]].emplace(i); }

  vector<int> ans;
  auto DFS = [&](auto f, int u) -> void {
    for (int a : Node_to_array[u]) {
      ans.emplace_back(a);
    }
    for (auto &n : Trie[u]) {
      f(f, n.index);
    }
  };

  DFS(DFS, 0);

  rep(i, 1, N + 1) { cout << ans[i] << " \n"[i == N]; }
}