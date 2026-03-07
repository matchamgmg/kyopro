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
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

struct edge{
    int u,v;
    ll t;
};
#define INF 1LL<<60

int main()
{
    
    // はじめにワーシャルフロイド法によりO(N^3)で任意の島i,jについて
    // 移動時間を求める
    // 島i,j間の移動にかかる時間をt(i,j)とおくと
    // t(1,x) ,..., t(x,x) ,...,t(x,N) + 使う橋を渡る時間の合計
    // 橋の順番がK!,橋の向きが2^K

    int N,M;
    cin >> N >> M;
    vector<edge> B(M);
    vvll dist(N,vll(N,INF));
    rep(i,0,N){
        dist[i][i] = 0;
    }
    rep(i,0,M){
        edge e;
        cin >> e.u >> e.v >> e.t;
        e.u--;
        e.v--;
        B[i] = e;
        chmin(dist[e.u][e.v],e.t);
        chmin(dist[e.v][e.u],e.t);
    }

    // Floyd-Warshall
    rep(i,0,N){// via
        rep(j,0,N){// start
            rep(k,0,N){// goal
                chmin(dist[j][k],dist[j][i] + dist[i][k]);
            }
        }
    }

    auto calc_time = [&](vi &BK,int bits,int &K) -> ll {
        ll res = 0;
        int u,v;
        u = 0;
        rep(i,0,K){
            if(bits&1){
                v = B[BK[i]].v;
            }else{
                v = B[BK[i]].u;
            }
            res += dist[u][v];
            res += B[BK[i]].t;
            if(bits&1){
                u = B[BK[i]].u;
            }else{
                u = B[BK[i]].v;
            }
            bits >>= 1;
        }
        res += dist[u][N-1];
        return res;
    };

    auto muki = [&](vi &BK,int &K) -> ll {
        ll res = INF;
        for(int i=0;i<(1<<K);i++){
            chmin(res,calc_time(BK,i,K));
        }
        return res;
    };

    auto query = [&](){
        int K;
        cin >> K;
        vi BK(K);
        rep(i,0,K){
            cin >> BK[i];
            BK[i]--;
        }
        sort(all(BK));
        ll ans = INF;
        do{
            chmin(ans,muki(BK,K));
        }while(next_permutation(all(BK)));
        printf("%lld ",ans);
    };

    int Q;
    cin >> Q;
    rep(i,0,Q){
        query();
    }
}