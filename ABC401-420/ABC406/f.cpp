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
using vb = vector<bool>;
using vvb = vector<vb>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

// 根付き木の深さ優先探索の行きがけ順に頂点番号を振ると
// 頂点 v を根とする部分木の任意の頂点番号 v' は v<v'
// 頂点 v を根とする部分木に含まれる頂点 : [行きがけ順の番号, 帰りがけ順の番号)

int op(int a, int b){ return a+b; }

int e(){ return 0; }

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<vector<int>> G(N);
    vector<pair<int,int>> Edges(N-1);
    rep(i,0,N-1){
        int u, v;
        cin >> u >> v;
        u--; v--;
        if(u>v) swap(u,v);
        Edges[i] = {u,v};
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }
    vector<int> L(N,-1);
    vector<int> R(N,-1);

    int cnt = 0;
    auto DFS = [&](auto f, int s, int p) -> void {
        L[s] = cnt;
        cnt++;
        for(int v : G[s]){
            if(v == p) continue;
            f(f, v, s);
        }
        R[s] = cnt;
        return;
    };
    DFS(DFS, 0, -1);

    vector<int> A(N,1);
    segtree<int,op,e> seg(A);

    int Q;
    cin >> Q;
    for(;Q--;){
        int t,x,y,w;
        cin >> t;
        if(t == 1){
            cin >> x >> w;
            x--;
            seg.set(L[x],seg.get(L[x]) + w);
        }else{
            cin >> y;
            y--;
            int par = Edges[y].first, chi = Edges[y].second;
            if(L[par] > L[chi]) swap(par, chi);
            cout << abs(seg.all_prod() - 2*seg.prod(L[chi],R[chi])) << endl;
        }
    }
}