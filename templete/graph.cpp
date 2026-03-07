#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
using Graph = vector<vector<int>>;

vector<bool> seen;
void dfs(const Graph &G,int v){
    seen[v] = true;

    for(auto next_v : G[v]){
        if(seen[next_v]) continue;
        dfs(G,next_v);
    }
}

vector<int> bfs(const Graph &G,int s){
    int N = G.size();
    vector<int> dist(N,-1);
    queue<int> que;

    dist[s] = 0;
    que.push(s);

    while(!que.empty()){
        int v = que.front();
        que.pop();

        for(int x : G[v]){
            if(dist[x] != -1)continue;
            dist[x] = dist[v] + 1;
            que.push(x);
        }
    }
    return dist;
}

int main()
{
    
}