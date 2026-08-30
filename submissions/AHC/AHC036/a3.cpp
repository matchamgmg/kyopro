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

void make_A(vector<int> &A,vector<int> &list,int &size_list){
    rep(i,size_list){
        A[i] = list[i];
    }
}

int make_next(vector<int> &next,int &LB,vector<int> &long_path,int &size_long_path,int &idx){
    int size_next = 0;
    int next_idx = 1;
    while(idx+next_idx<size_long_path){
        if(size_next==0){
            next[size_next] = long_path[idx+next_idx];
            size_next++;
            next_idx++;
        }else if(size_next==LB){
            bool flag = false;
            rep(i,size_next){
                if(next[i]==long_path[idx+next_idx]){
                    flag = true;
                }
            }
            if(flag){
                next_idx++;
            }else{
                break;
            }
        }else{
            bool flag = false;
            rep(i,size_next){
                if(next[i]==long_path[idx+next_idx]){
                    flag = true;
                }
            }
            if(flag){
                next_idx++;
            }else{
                next[size_next] = long_path[idx+next_idx];
                size_next++;
                next_idx++;
            }
        }
    }
    return size_next;
}

bool q_move(const vector<int> &long_path,int &idx,vector<int> &B,int &cnt,ofstream &outfile){
    bool check = false;
    int next_one = long_path[idx+1];
    rep(i,B.size()){
        if(B[i]==next_one) check = true;
    }
    if(!check) return false;

    outfile << "m" << " " << next_one << endl;
    idx++;
    cnt++;
    return true;
}

int calc_score(vector<int> &partial_A,vector<int> &next,int &size_next){
    int score = 0;
    int size_partial_A = partial_A.size();
    rep(i,size_next){
        int n = next[i];
        bool have = false;
        rep(j,size_partial_A){
            if(partial_A[j]==n){
                score++;
                have = true;
                break;
            }
        }
        if(!have) return score;
    }
    return score;
}

void q_switch(vector<int> &A,vector<int> &B,int &cnt,int &LA,int &LB,vector<int> &next,int &size_next,ofstream &outfile){
    int l = 0,PA = 0,PB = 0;
    int next_one = next.front();
    vector<int> next_idx_list;
    rep(i,LA){
        if(A[i]==next_one){
            next_idx_list.push_back(i);
        }
    }
    int best_score = -1;
    int best_len = -1;
    for(int next_idx : next_idx_list){
        //cout << "next_idx" << " " << next_idx << endl;
        for(int i = next_idx;i>=0&&i+LB>next_idx;i--){
            vector<int> partial_A;
            int size_partial_A = min(LB,LA-i);
            //cout << "size_partial_A" << " " << size_partial_A << endl;
            for(int j = 0;j<size_partial_A;j++){
                partial_A.push_back(A[i+j]);
            }

            /*
            rep(j,size_partial_A){
                cout << partial_A[j] << " ";
            }
            cout << endl;
            */

            int score = calc_score(partial_A,next,size_next);
            if(score>best_score || (score==best_score && size_partial_A>best_len)){
                best_score = score;
                best_len = size_partial_A;
                l = best_len;
                PA = i;
                PB = 0;
            }
        }
    }

    /*
    int size_next = next.size();
    rep(i,size_next){
        cout << next[i] << " ";
    }
    cout << endl;
    */

    outfile << "s" << " " << l << " " << PA << " " << PB << endl;
    rep(i,l){
        B[i] = A[PA+i];
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

    ofstream outfile("out.txt");

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
    cout << "long_path_length " << size_long_path << endl;

    //long_path上で登場する頂点のみを残す
    vector<int> onpath;
    set<int> onpath_set;
    rep(i,size_long_path){
        if(!onpath_set.count(long_path[i])){
            onpath.push_back(long_path[i]);
            onpath_set.insert(long_path[i]);
        }
    }
    int size_onpath = onpath.size();

    //配列Aを決める
    vector<int> A = vector<int>(LA,0);
    make_A(A,onpath,size_onpath);
    rep(i,LA){
        outfile << A[i] << " ";
    }
    outfile << endl;
    vector<int> B = vector<int>(LB,-1);

    //シミュレーション
    int cnt = 0;
    int idx = 0;
    int size_next = 0;
    vector<int> next = vector<int>(LB);
    size_next = make_next(next,LB,long_path,size_long_path,idx);

    rep(i,LB){
        next[i] = long_path[i+1];
    }
    while(true){
        while(idx<size_long_path-1){
            if(!q_move(long_path,idx,B,cnt,outfile)) break;
        }
        if(idx>=size_long_path-1){
            //cout << "goal" << endl;
            break;
        }
        //nextを再構成
        size_next = make_next(next,LB,long_path,size_long_path,idx);

        /*
        rep(i,size_next){
            cout << next[i] << " ";
        }
        cout << endl;
        */

        q_switch(A,B,cnt,LA,LB,next,size_next,outfile);
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
   outfile.close();
}