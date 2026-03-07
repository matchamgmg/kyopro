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

int main(){
    int N,M;
    cin >> N >> M;
    PotentializedUnionFind<long long> puf(N);
    for(int i=0 ; i<M ; i++){
        int l,r;
        long long d;
        cin >> l >> r >> d;
        l--; r--;
        if(!puf.issame(l,r)){
            puf.unite(l,r,d);
        }else if(puf.diff(l,r)!=d){
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
}