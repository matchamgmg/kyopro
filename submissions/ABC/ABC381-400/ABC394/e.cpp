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

#define INF 1<<29

// 文字列Sが回文であるとき、文字列xSxは回文である
// S = "" も含む
// 文字列の長さ = パスの長さ
// パスが必ず回文となるようなグラフ上の最短経路問題
// (回文の左端の頂点, 回文の右端の頂点) の状態を遷移するグラフ

int main()
{
    int N;
    cin >> N;
    vector<string> S(N);
    rep(i,0,N){
        cin >> S[i];
    }
    vector<vector<pair<int,int>>> G1(26),G2(26);
    rep(i,0,N){
        rep(j,0,N){
            if(S[i][j]!='-'){
                G1[S[i][j]-'a'].push_back({i,j});
                G2[S[i][j]-'a'].push_back({j,i});
            }
        }
    }
    vvi G(N*N);
    rep(i,0,26){
        for(auto p1 : G1[i]){
            for(auto p2 : G2[i]){
                // (j2,i1) -> (i2,j1)
                G[p2.first*N + p1.first].push_back(p2.second*N + p1.second);
            }
        }
    }
    vvi dist(N,vi(N,INF));
    rep(i,0,N){
        chmin(dist[i][i],0);
    }
    rep(i,0,N){
        rep(j,0,N){
            if(S[i][j]!='-'){
                chmin(dist[i][j],1);
            }
        }
    }
    queue<pair<int,int>> q;
    rep(i,0,N)rep(j,0,N){
        if(dist[i][j]<INF){
            q.push({dist[i][j],i*N+j});
        }
    }
    while(!q.empty()){
        auto p = q.front();
        q.pop();
        int u = p.second;
        int d = p.first;
        for(int next : G[u]){
            if(dist[next/N][next%N]<=d+2) continue;
            chmin(dist[next/N][next%N],d+2);
            q.push({dist[next/N][next%N],next});
        }
    }
    rep(i,0,N){
        rep(j,0,N){
            cout << (dist[i][j]==INF ? -1 : dist[i][j]) << ' ';
        }
        cout << endl;
    }
}