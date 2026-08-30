#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

template<typename T>
struct segtree{
    int n;
    vector<T> dat;
    segtree(int n_) : n(),dat(n_*4,0){
        int x = 1;
        while(n_ > x){
            x *= 2;
        }
        n = x;
    }

    void update(int i,T x){
        i += n-1;
        dat[i] = x;
        while(i>0){
            i = (i-1)/2;
            dat[i] = dat[i*2+1] + dat[i*2+2];
        }
    }

    T query(int a,int b){return query_sub(a,b,0,0,n);}
    T query_sub(int a,int b,int k,int l,int r){
        if(r<=a||b<=l){
            return 0;
        }else if(a<=l&&r<=b){
            return dat[k];
        }else{
            T vl = query_sub(a,b,k*2+1,l,(l+r)/2);
            T vr = query_sub(a,b,k*2+2,(l+r)/2,r);
            return (vl+vr);
        }
    }
};

int main()
{
    int H,W,Y;
    cin >> H >> W >> Y;
    vector<vector<int>> A(H,vector<int>(W,0));
    segtree<int> seg(H*W);
    vector<vector<int>> idx(1e5+1,vector<int>());
    rep(i,H){
        rep(j,W){
            cin >> A[i][j];
            idx[A[i][j]].push_back(i*H+j);
            seg.update(i*H+j,1);
        }
    }
    int i=1e5;
    rep(j,Y){
        while(i>0&&i>j){
            i--;
        }
        for(int k : idx[i]){
            seg.update(k,0);
        }
        cout << seg.query(0,H*W-1) << endl;
    }
    
}