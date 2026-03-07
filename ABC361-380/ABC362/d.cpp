#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define INF 1LL<<60

struct Edge{
    ll to;
    ll w;
    Edge(ll a,ll b):to(a),w(b){}
};

using Graph = vector<vector<Edge>>;

template<class T> bool chmin(T &a,T b){
    if(a>b){
        a=b;
        return true;
    }
    else return false;
}

int main()
{
    ll N,M;
    cin >> N >> M;
    vector<ll> A(N),B(M);
    Graph G = Graph(N);
    rep(i,N){
        cin >> A[i];
    }
    ll u,v,b;
    rep(i,M){
        cin >> u >> v >> b;
        u--;
        v--;
        G[u].push_back({v,b});
        G[v].push_back({u,b});
    }

    ll s = 0;
    vector<ll> dist(N,INF);
    dist[s] = A[0];

    priority_queue<pair<ll,ll>,vector<pair<ll,ll>>,greater<pair<ll,ll>>> que;
    que.push({dist[s],s});

    while(!que.empty()){
        ll v = que.top().second;
        ll d = que.top().first;
        que.pop();

        if(d>dist[v]) continue;

        for(auto e : G[v]){
            if(chmin(dist[e.to],dist[v] + e.w + A[e.to])){
                que.push({dist[e.to],e.to});
            }
        }
    }

    for(int v=1;v<N;v++){
        cout << dist[v] << " ";
    }
}