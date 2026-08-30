#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
//using mint = modint1000000007;
using vi = vector<int>;
using vll = vector<ll>;
using vld = vector<ld>;
using vmint = vector<mint>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using vvld = vector<vld>;
using vvmint = vector<vmint>;
using vvvi = vector<vvi>;
using vvvll = vector<vvll>;
using vvvld = vector<vvld>;
using vvvmint = vector<vvmint>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

struct MyUF {
    vi siz,par;
    vvi Kth;
    MyUF(int n) : siz(n,1),par(n,-1),Kth(n){
        rep(i,0,n){
            Kth[i].push_back(i);
        }
    }
    int root(int x){
        if(par[x]==-1) return x;
        return par[x] = root(par[x]);
    }
    bool issame(int x,int y){
        return root(x)==root(y);
    }
    bool merge(int x_,int y_){
        int x = root(x_); int y = root(y_);
        if(x==y) return false;
        //union by number
        if(siz[x] < siz[y]) swap(x,y);
        rep(i,0,Kth[y].size()){
            Kth[x].push_back(Kth[y][i]);
        }
        sort(all(Kth[x]),greater<int>{});
        while(Kth[x].size()>10){
            Kth[x].pop_back();
        }
        siz[x] += siz[y];
        par[y] = x;
        return true;
    }
    int query(int v,int k){
        int x = root(v);
        if(siz[x]<k) return -1;
        return Kth[x][k-1]+1;
    }
};

int main()
{
    int N,Q;
    cin >> N >> Q;
    MyUF uf(N);
    rep(i,0,Q){
        int t;
        cin >> t;
        if(t==1){
            int u,v;
            cin >> u >> v;
            u--;
            v--;
            uf.merge(u,v);
        }else if(t==2){
            int v,k;
            cin >> v >> k;
            v--;
            cout << uf.query(v,k) << endl;
        }
    }
}