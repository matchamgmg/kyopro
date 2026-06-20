#include <atcoder/all>
#include <bits/stdc++.h>
using namespace std;

/*
全方位木DP
重みなしver. と重みありver.

自分自身に関する値を適切に設定する必要がある
    dfs1の葉ノードの初期値
    dfs2の累積mergeの初期値
*/

// 重みなしver.
template <class T, auto e, auto merge, auto add_root> struct Rerooting {
  vector<vector<T>> dp;
  vector<T> ans;
  vector<vector<int>> G;

  Rerooting() {}
  Rerooting(int N) : G(N) {
    dp.resize(N);
    ans.assign(N, e());
  }
  Rerooting(vector<vector<int>> F) : G(F) {
    int N = F.size();
    dp.resize(N);
    ans.assign(N, e());
  }

  void add_edge(int a, int b) { G[a].push_back(b); }
  void build() { // 全方位木DP
    dfs1(0);
    dfs2(0);
  }

  T dfs1(int u, int p = -1) {
    T dp_cum = e();
    int siz = G[u].size();
    dp[u].assign(siz, e());
    for (int i = 0; i < siz; ++i) {
      int v = G[u][i];
      if (v == p)
        continue;
      dp[u][i] = dfs(v, u);
      dp_cum = merge(dp_cum, dp[u][i]);
    }
    return add_root(dp_cum);
  }
  void dfs2(int u, const T &dp_p, int p = -1) {
    int siz = G[u].size();
    for (int i = 0; i < siz; ++i) {
      if (G[u][i] == p)
        dp[u][i] = dp_p;
    }
    vector<T> dp_l(siz + 1, e()), dp_r(siz + 1, e());
    for (int i = 0; i < siz; ++i) {
      dp_l[i + 1] = merge(dp_l[i], dp[u][i]);
    }
    for (int i = siz - 1; i >= 0; --i) {
      dp_r[i] = merge(dp_r[i + 1], dp[u][i]);
    }
    ans[u] = add_root(dp_l[siz]);
    for (int i = 0; i < siz; ++i) {
      int v = G[u][i];
      if (v == p)
        continue;
      dfs2(v, add_root(merge(dp_l[i], dp_r[i + 1])), u);
    }
  }
};

int main() {}