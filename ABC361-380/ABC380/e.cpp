#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

struct UnionFind
{
    vector<int> par, siz, color, left, right, cnt_col;
    /*
    par[i] : iを含むグループの根
    siz[i] : iを根とする木の頂点数
    */

    //初期化
    UnionFind(int n) : par(n, -1), siz(n, 1), color(n, 0), left(n, 0), right(n, 0),cnt_col(n, 1) {
        for(int i=0;i<n;i++){
            color[i] = i;
            left[i] = i;
            right[i] = i;
        }
    }

    //根を求める
    int root(int x){
        if(par[x]==-1) return x;
        else return par[x] = root(par[x]);
    }

    //グループの色を変更する
    void change_col(int x, int col){
        cnt_col[color[root(x)]] -= siz[root(x)];
        color[root(x)] = col;
        cnt_col[col] += siz[root(x)];
    }

    //xとyが同じグループに属するかどうか
    bool issame(int x, int y){
        return root(x) == root(y);
    }

    //xを含むグループとyを含むグループとを併合する
    bool unite(int x, int y){
        x = root(x); y = root(y);
        if(x==y) return false;
        if(siz[x] < siz[y]) swap(x,y);  //分岐を書くのを省略しているだけ
        par[y] = x;
        siz[x] += siz[y];
        left[x] = min(left[x], left[y]);
        right[x] = max(right[x], right[y]);
        return true;
    }

    //連結成分の個数を求める
    int groups(){
        int res = 0;
        for(int i=0;i<(int)par.size();i++){
            if(root(i)==i) res++;
        }
        return res;
    }

    //xを含むグループのサイズを返す
    int size(int x){
        return siz[root(x)];
    }
};

int main()
{
    int N,Q;
    cin >> N >> Q;
    UnionFind uf(N);
    rep(i,0,Q){
        int q;
        cin >> q;
        if(q==1){
            int x,c;
            cin >> x >> c;
            x--; c--;
            uf.change_col(x,c);
            if(uf.left[uf.root(x)]>0){
                if(uf.color[uf.root(x)]==uf.color[uf.root(uf.left[uf.root(x)]-1)]){
                    uf.unite(x,uf.left[uf.root(x)]-1);
                }
            }
            if(uf.right[uf.root(x)]<N-1){
                if(uf.color[uf.root(x)]==uf.color[uf.root(uf.right[uf.root(x)]+1)]){
                    uf.unite(x,uf.right[uf.root(x)]+1);
                }
            }
        }else if (q==2){
            int c;
            cin >> c;
            c--;
            cout << uf.cnt_col[c] << endl;
        }
    }
}