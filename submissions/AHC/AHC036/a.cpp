#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
using Graph = vector<vector<int>>;

void bfs(const Graph &G,vector<pair<int,int>> &dist,int s){
    if(dist[s].first!=-1) return;

    queue<int> que;
    dist[s].first = 0;
    que.push(s);
    while(!que.empty()){
        int u = que.front();
        int d = dist[u].first + 1;
        que.pop();
        for(int v : G[u]){
            if(dist[v].first!=-1) continue;
            dist[v].first = d;
            dist[v].second = u;
            que.push(v);
        }
    }
}

void make_path(vector<int> &path,vector<pair<int,int>> &dist,int s,int g){
    if(path.size()!=0) return;

    vector<int> ret;
    int u = g;
    ret.push_back(u);
    while(dist[u].second!=s){
        u = dist[u].second;
        ret.push_back(u);
    }
    ret.push_back(s);
    int size_ret = ret.size();
    rep(i,size_ret){
        path.push_back(ret[size_ret-i-1]);
    }

}

void make_A(vector<int> &A,int N){
    rep(i,N){
        A[i] = i;
    }
}

bool q_move(const vector<int> &long_path,int &idx,vector<int> &B,int &cnt){
    bool check = false;
    int next = long_path[idx+1];
    rep(i,B.size()){
        if(B[i]==next) check = true;
    }
    if(!check) return false;

    cout << "m" << " " << next << endl;
    idx++;
    cnt++;
    return true;
}

void q_switch(const vector<int> &long_path,int &idx,vector<int> &A,vector<int> &B,int &cnt,int &LA,int &LB){
    int l,PA,PB;
    int next = long_path[idx+1];
    l = min(LB,LA-next);
    PA = next;
    PB = 0;
    cout << "s" << " " << l << " " << PA << " " << PB << endl;
    rep(i,l){
        B[i] = A[next+i];
    }
    cnt++;
}

int main()
{
    int N,M,T,LA,LB;
    cin >> N >> M >> T >> LA >> LB;
    Graph G = Graph(N);
    int u,v;
    rep(i,M){
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    vector<int> city_list(T);
    rep(i,T){
        cin >> city_list[i];
    }
    int x,y;
    rep(i,N){
        cin >> x >> y;
    }

    //city_listの順で最短路を求める
    map<pair<int,int>,vector<int>> path;
    vector<vector<pair<int,int>>> dist(N,vector<pair<int,int>>(N,{-1,-1}));
    int s = 0;
    int g = 0;
    rep(i,T){
        bfs(G,dist[s],s);
        g = city_list[i];
        make_path(path[{s,g}],dist[s],s,g);
        s = g;
    }

    //pathを一つにまとめる
    vector<int> long_path;
    s = 0;
    long_path.push_back(s);
    rep(i,T){
        g = city_list[i];
        for(auto u : path[{s,g}]){
            if(u==long_path.back()) continue;
            long_path.push_back(u);
        }
        s = g;
    }
    int size_long_path = long_path.size();

    /*
    rep(i,long_path.size()){
        cout << long_path[i] << " ";
    }
    cout << endl;
    */

    //配列Aを決める
    vector<int> A = vector<int>(LA,0);
    make_A(A,N);
    rep(i,LA){
        cout << A[i] << " ";
    }
    cout << endl;
    vector<int> B = vector<int>(LB,-1);

    //シミュレーション
    int cnt = 0;
    int idx = 0;
    while(true){
        while(idx<size_long_path-1){
            if(!q_move(long_path,idx,B,cnt)) break;
        }
        if(idx>=size_long_path-1){
            //cout << "goal" << endl;
            break;
        }
        q_switch(long_path,idx,A,B,cnt,LA,LB);
        if(cnt>200000) break;
    }

    /*
    rep(i,N){
        for(auto u : dist[i]){
            cout << u.first << " ";
        }
        cout << endl;
    }
    s = 0;
    rep(i,T){
        g = city_list[i];
        for(auto u : path[{s,g}]){
            cout << u << " ";
        }
        cout << endl;
        s = g;
    }
    */
}