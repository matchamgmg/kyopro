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
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

int main()
{
    int N,M;
    cin >> N >> M;
    vvi G(N,vi(N,0));
    rep(i,0,M){
        int u,v;
        cin >> u >> v;
        u--;
        v--;
        if(u<v) swap(u,v);
        G[u][v] = 1;
    }
    int ans = 1e9;
    vvi G2(N,vi(N,0));
    if(N<=5){
        // 連結成分数は高々1
        vi idx(N);
        iota(all(idx),0);
        do{
            rep(i,0,N){
                rep(j,0,i){
                    G2[i][j] = 0;
                }
            }
            rep(i,0,N){
                int u = i;
                int v = (i+1)%N;
                if(u<v) swap(u,v);
                G2[u][v] = 1;
            }
            int cost = 0;
            rep(i,0,N){
                rep(j,0,i){
                    cost += (G[i][j]!=G2[i][j]);
                }
            }
            chmin(ans,cost);
        }while(next_permutation(all(idx)));
    }else{
        // 連結成分数は高々2
        // 1のとき
        vi idx(N);
        iota(all(idx),0);
        do{
            rep(i,0,N){
                rep(j,0,i){
                    G2[i][j] = 0;
                }
            }
            rep(i,0,N){
                int u = i;
                int v = (i+1)%N;
                if(u<v) swap(u,v);
                G2[u][v] = 1;
            }
            int cost = 0;
            rep(i,0,N){
                rep(j,0,i){
                    cost += G[i][j]!=G2[i][j];
                }
            }
            chmin(ans,cost);
        }while(next_permutation(all(idx)));
        // 2のとき
        // 片方の連結成分が3
        vi g1,g2;
        auto dfs = [&](auto f, int num, int current, int idx) -> void {
            if(current == num){
                do{
                    do{
                        rep(i,0,N){
                            rep(j,0,i){
                                G2[i][j] = 0;
                            }
                        }
                        rep(i,0,g1.size()){
                            int u = i;
                            int v = (i+1)%g1.size();
                            if(u<v) swap(u,v);
                            G2[u][v] = 1;
                        }
                        rep(i,0,g2.size()){
                            int u = i;
                            int v = (i+1)%g2.size();
                            if(u<v) swap(u,v);
                            G2[u][v] = 1;
                        }
                        int cost = 0;
                        rep(i,0,N){
                            rep(j,0,i){
                                cost += G[i][j]!=G2[i][j];
                            }
                        }
                        chmin(ans,cost);
                    }while(next_permutation(all(g2)));
                }while(next_permutation(all(g1)));
                return;
            }
            if(idx == N) return;
            f(f,num,current,idx+1);
            g1.push_back(idx);
            f(f,num,current+1,idx+1);
            g1.pop_back();
            return;
        };
        dfs(dfs,3,0,0);
        if(N==8){
            // 4
            dfs(dfs,4,0,0);
        }
    }
    cout << ans << endl;
}