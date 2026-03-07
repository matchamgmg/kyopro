/*
ポテンシャル付きUnionFind
UnionFindの各ノードに重みをもたせて、ノード間の距離を管理する
weight[x] : xのroot(x)からの距離
diff_weight[x] : x,par[x]間の距離

関連用語 : 差分制約系, アーベル群(足し算と引き算ができる代数系)

クエリ
unite(x, y, w) : weight(y) = weight(x) + w
    となるようにxとyを併合する
issame(x, y) : xとyが同じグループに属するかを判定する
diff(x, y) : xとyが同じグループに属するとき、
    weight(y) - weight(x)を返す

最小全域木 Minimum Spanning Tree (クラスカル法)
    辺のコストが小さい順に、その辺を加えても閉路にならない(issame()=false)
    場合、uniteしていく
*/

#include <bits/stdc++.h>
using namespace std;

template<class T>
struct PotentializedUnionFind
{
    vector<int> par,siz;
    vector<T> diff_weight;

    PotentializedUnionFind(int n, T AbelSumUnity = 0) : siz(n, 1), diff_weight(n, AbelSumUnity){
        par.resize(n);
        iota(par.begin(),par.end(),0);
    }

    int root(int x){
        if(par[x]==x) return x;
        else{
            int r = root(par[x]);
            diff_weight[x] += diff_weight[par[x]];
            return par[x] = r;
        }
    }

    bool issame(int x, int y){
        return root(x) == root(y);
    }

    T weight(int x){
        root(x);
        return diff_weight[x];
    }

    int diff(int x, int y){
        //issame(x, y) = false;となる場合の動作は不定
        return weight(y) - weight(x);
    }

    bool unite(int x, int y, T w){
        w += weight(x); w -= weight(y);
        x = root(x); y = root(y);
        if(x==y) false;
        if(siz[x] < siz[y]){
            swap(x,y);
            w = -w;
        }
        par[y] = x;
        siz[x] += siz[y];
        diff_weight[y] = w;
        return true;
    }

    int size(int x){
        return siz[par[x]];
    }

    int groups(){
        int res = 0;
        for(int i=0 ; i<par.size() ; i++){
            if(root(i)==i) res++;
        }
        return res;
    }
};