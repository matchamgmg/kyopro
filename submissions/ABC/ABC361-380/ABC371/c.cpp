#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvll = vector<vll>;
#define INF 1<<29

int main()
{
    int N,MG;
    cin >> N >> MG;
    set<pair<int,int>> G;
    rep(i,0,MG){
        int u,v;
        cin >> u >> v;
        u--;
        v--;
        G.insert({u,v});
        G.insert({v,u});
    }
    int MH;
    cin >> MH;
    set<pair<int,int>> H;
    rep(i,0,MH){
        int u,v;
        cin >> u >> v;
        u--;
        v--;
        H.insert({u,v});
        H.insert({v,u});
    }
    vvi A(N,vi(N,0));
    rep(i,0,N){
        for(int j=i+1;j<N;j++){
            cin >> A[i][j];
            A[j][i] = A[i][j];
        }
    }
    vi idx;
    rep(i,0,N){
        idx.push_back(i);
    }
    int min_cost = INF;
    int cost = 0;
    do{
        /*rep(i,0,N){
            cout << idx[i] << " ";
        }
        cout << endl;*/
        cost = 0;
        rep(i,0,N){
            rep(j,0,i){
                if(G.count({idx[i],idx[j]})!=H.count({i,j})){
                    cost += A[i][j];
                }else{
                    continue;
                }
            }
        }
        //cout << cost << endl;
        min_cost = min(min_cost,cost);
    }while(next_permutation(ALL(idx)));
    cout << min_cost << endl;
}