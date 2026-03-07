## 木DP
根つき木に関して，部分木に関する値を利用して計算する
1. 部分木の値をマージ
2. マージした値に対する計算

無向グラフのとき
```cpp
auto dfs = [&](auto f, int u, int p = -1){
    if(G[u].size()<=1){
        dp[u] = 0;  // 初期値
        return;
    }
    int dp_cum = 0;
    for(int v : G[u]){
        if(v==p) continue;
        f(f,v,u);
        dp_cum = merge(dp_cum,dp[v]);   // 部分木の情報をマージ
    }
    dp[u] = add_root(dp_cum);   // マージした結果に対する計算
}
```
## 全方位木DP（Rerooting）
$dp[i][j]$:=頂点$i$を根とする部分木について，"$i$から出る$j$番目の有向辺に対応する部分木"に関する値

モノイド（結合則＋単位元）をのせる

頂点数$N$に対して有向辺の数=$2(N-1)$．計算量$O(N)$

1. 一つの頂点$u$に対してDFSで木DP
2. $u$からBFSして値を伝搬する（DFSでも可）

部分木のマージの仕方を工夫する必要がある

次数$d$の頂点について任意の$d-1$個の部分木をマージするとき，左右からの累積マージを考えると，$i$番目の部分木を除いてマージする操作について$\mathrm{merge}(\mathrm{merge}[0,i),\mathrm{merge}(i,d-1])$として表現でき，$O(1)$で可能

* DPテーブルに乗せる値の型
* 頂点同士をマージする関数およびその単位元
* 親に値を返す関数
* 個に頂点が存在しないときの値

これらを問題ごとに考える

$N$頂点からなる木$T$の各頂点$i = 1,2,\dots,N$について，$f(1),f(2),\dots,f(N)$を求めるとき，はじめ，頂点$1$を根としてDFSをすることで，$T$の各頂点$v$について，$v$の子$w$を根とする$v$の部分木全体についての値$g_v(w)$が求められる．特に，頂点$1$について，頂点$1$のすべての子について$g_1(w)$が求まっているので，$f(1) = \underset{w}{\mathrm{merge}} ({g_1(w)})$が求まる．

(実際は$f(1)=\mathrm{merge}(\mathrm{apply}(g_1(w)))$)

2回目のDFSでは，頂点$1$の子が頂点$2$であるとき$f(2) = \mathrm{merge}(\underset{w_1\neq2}{\mathrm{merge}}(g_1(w_1)), g_2(w_2))$となる．$\underset{w_1\neq2}{\mathrm{merge}}(g_1(w_1))$は，$f(1)$から，頂点$2$を根とする部分木による寄与を取り除いたものである．これを繰り返すことにより，すべての頂点$v$について$f(v)$が求められる．

左右からの累積マージをとることで，$\underset{w_1\neq2}{\mathrm{merge}}(g_1(w_1))$を$O(1)$で実現する．
```
in DFS1():
Data memo[v]: 各頂点vについて，子頂点のg_v(w)の累積マージ(結果だけを持つ)
```

```
in DFS2(int s, int p, Data val):
int n :sの子の数
vector<Data> ds: sのそれぞれの子についてのg_s(w)の値(memo[w])

ds: {val(親ノードからの引継ぎ)} で初期化
for(int v : G[s]){
    if(v == p) continue;
    ds.push_back(apply(memo[v]))
}
int n = ds.size(); int idx = 1
vector<Data> head, tail: 左右からの累積マージ
f(s) = head[n]
for(int v : G[s]){
    if(v == p) continue;
    Data sub: vの寄与を取り除く
    sub = merge(head[idx], tail[idx+1])
    DFS2(v, s, sub)
    idx++;
}
```

総和だけを保持すれば十分な場合，累積マージの実装をさぼることができる．