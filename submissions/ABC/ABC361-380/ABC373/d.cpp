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


vector<bool> seen;
vi order;
void rec(const vector<vector<pair<int,ll>>> &G,int v){
    seen[v] = true;
    for(auto next_vw : G[v]){
        int next_v = next_vw.first;
        if(seen[next_v]) continue;
        rec(G,next_v);
    }
    order.push_back(v);
}

int main()
{
    int N,M;
    cin >> N >> M;
    vector<vector<pair<int,ll>>> G(N);
    rep(i,0,M){
        int u,v;
        ll w;
        cin >> u >> v >> w;
        u--;
        v--;
        G[u].push_back({v,w});
        G[v].push_back({u,-w});
    }

    //トポロジカルソート
    
    seen.assign(N,false);
    order.clear();
    
    rep(v,0,N){
        if(seen[v]) continue;
        rec(G,v);
    }
    reverse(ALL(order));
    
    
    vll ans(N,0);
    vector<bool> used(N,false);
    queue<int> que;
    rep(i,0,N){
        if(used[order[i]]) continue;
        que.push(order[i]);
        used[order[i]] = true;
        ans[order[i]] = 0;
        while(!que.empty()){
            int v = que.front();
            que.pop();
            for(auto next_vw : G[v]){
                int next_v = next_vw.first;
                if(used[next_v]) continue;
                que.push(next_v);
                used[next_v] = true;
                ans[next_v] = ans[v] + next_vw.second;
            }
        }
    }

    rep(i,0,N){
        cout << ans[i] << " ";
    }
    cout << endl;
}