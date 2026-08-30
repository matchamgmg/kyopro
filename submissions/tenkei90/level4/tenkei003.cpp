#include <bits/stdc++.h>
#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<vector<int>> G(N,vector<int>());
    int s,g;
    rep(i,0,N-1){
        int a,b;
        cin >> a >> b;
        G[a-1].push_back(b-1);
        G[b-1].push_back(a-1);
        s = a-1;
    }
    //木の直径を求める
    //bfs
    vector<int> dist_s(N,-1);
    dist_s[s] = 0;
    queue<int> que;
    que.push(s);
    while(!que.empty()){
        int u = que.front();
        que.pop();
        for(int v : G[u]){
            if(dist_s[v]!=-1) continue;
            dist_s[v] = dist_s[u] + 1;
            que.push(v);
            g = v;
        }
    }

    vector<int> dist_g(N,-1);
    dist_g[g] = 0;
    que.push(g);
    while(!que.empty()){
        int u = que.front();
        que.pop();
        for(int v : G[u]){
            if(dist_g[v]!=-1) continue;
            dist_g[v] = dist_g[u] + 1;
            que.push(v);
            s = v;
        }
    }

    int ans = dist_g[s] + 1;
    cout << ans << endl;
}