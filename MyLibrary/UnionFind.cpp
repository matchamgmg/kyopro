/*
UnionFind木
グループ分けを木構造で管理

関数
issame(x, y) : x, yが同じグループに属するかを調べる
unite(x, y) : xを含むグループとyを含むグループを併合する
root(x) : xを含むグループの根を返す

root(x)の計算量
union_by_size : 木を併合するときに、頂点数の多い方の木の根に
    頂点数の少ない方の木を接続する
    xの深さが+1されるとき、必ず木の頂点数は*2以上になる
    O(log N)
経路圧縮 : root(x)の際、xから上へと進んでいって根に到達するまでの
    経路中の頂点に対し、その親を根に張り替える
    O(log N)
union_by_size + 経路圧縮 : O(a(N)) =~ O(1)

連結成分の個数
すべての頂点についてグルーピングを終えた後について
根となる頂点の種類数を数える
*/
#include <bits/stdc++.h>
using namespace std;

struct UnionFind {
    int n;
    int group_num;
    vector<int> par;
    UnionFind(int n_) : n(n_), group_num(n_) { par.assign(n_, -1); }

    int root(int x) {
        if (par[x] < 0) return x;
        return par[x] = root(par[x]);
    }

    int size(int x) { return -par[root(x)]; }

    bool same(int x, int y) { return root(x) == root(y); }

    bool merge(int x, int y) {
        x = root(x);
        y = root(y);
        if (x == y) return false;
        if (size(x) > size(y)) swap(x, y);
        par[y] += par[x];
        par[x] = y;
        group_num--;
        return true;
    }

    int groups() { return group_num; }
};

int main() {
    int n = 8;
    int a = (n & -n);
    cout << a << endl;
}