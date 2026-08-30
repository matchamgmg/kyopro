#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
using Graph = vector<vector<int>>;

//ofstream outfile("out.txt");

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

    int u = s;
    path.push_back(u);
    while(dist[u].second!=g){
        u = dist[u].second;
        path.push_back(u);
    }
    path.push_back(g);
}

void new_bfs(Graph &G,int &size_vec,vector<int> &dist,int &s,vector<int> &seen){
    if(size_vec==0) return;

    queue<int> que;
    dist[s] = 0;
    seen[s] = 0;
    que.push(s);
    while(!que.empty()){
        int u = que.front();
        int d = dist[u] + 1;
        que.pop();
        for(int v : G[u]){
            if(dist[v]!=-1) continue;
            dist[v] = d;
            que.push(v);
        }
    }
}

void grouping(vector<vector<int>> &new_group,vector<vector<int>> &dist,vector<int> &seen,priority_queue<pair<int,int>> &count_city,int &s,const int N,const int LA,const int LB,int &now_A_size){
    int next_city = s;
    while(!count_city.empty()){
        if(count_city.top().first==0){
            break;
        }
        if(seen[next_city]>=1){
            count_city.pop();
            next_city = count_city.top().second;
            continue;
        }

        vector<pair<int,int>> dist_city_vec(N);
        rep(i,N){
            dist_city_vec[i] = {dist[next_city][i],i};
        }
        sort(ALL(dist_city_vec));
        vector<int> ret;
        int ret_size = 0;
        int v = next_city;
        next_city = -1;
        int last_dist = 0;
        rep(i,N){
            int u = dist_city_vec[i].second;
            if(ret_size<LB && now_A_size<LA){
                if(seen[u]>=1 || dist[v][u]==-1) continue;
                if(dist[v][u]-last_dist>1){
                    break;
                }
                ret.push_back(u);
                seen[u]++;
                ret_size++;
                now_A_size++;
                last_dist = dist[v][u];
            }
        }
        count_city.pop();
        next_city = count_city.top().second;
        new_group.push_back(ret);

        /*
        rep(i,ret_size){
            cout << ret[i] << " ";
        }
        cout << endl;
        */
    }
}

void grouping2(vector<vector<int>> &new_group,vector<vector<int>> &dist,vector<int> &seen,const int N,const int LA,const int LB,int &now_A_size){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> group_size_group_idx;
    rep(i,new_group.size()){
        group_size_group_idx.push({(int)new_group[i].size(),i});
    }
    while(now_A_size<LA && group_size_group_idx.top().first<LB){
        int size_grp = group_size_group_idx.top().first;
        int idx_grp = group_size_group_idx.top().second;
        group_size_group_idx.pop();

        vector<pair<int,int>> dist_city(N,pair<int,int>());
        rep(i,N){
            dist_city[i] = {dist[new_group[idx_grp].front()][i],i};
        }
        sort(ALL(dist_city));
        rep(i,N){
            int d = dist_city[i].first;
            int u = dist_city[i].second;
            if(d<0) continue;
            bool exist = false;
            rep(j,size_grp){
                if(new_group[idx_grp][j]==u){
                    exist = true;
                    break;
                }
            }
            if(exist) continue;
            new_group[idx_grp].push_back(u);
            now_A_size++;
            size_grp++;
            group_size_group_idx.push({size_grp,idx_grp});
            break;
        }
    }
}

void make_A(vector<int> &A,int &LA,int &LB,vector<int> &list,int &size_list,int &N){
    //long_path上で各頂点を通る回数を集計
    vector<int> city_count(N,0);
    rep(i,size_list){
        city_count[list[i]]++;
    }

    /*
    rep(i,N){
        cout << city_count[i] << " ";
    }
    cout << endl;
    */

    //long_pathからグラフを構成
    Graph newG(N,vector<int>(N,-1));
    vector<int> size_vec(N,0);
    rep(i,size_list-1){
        bool flag = false;
        rep(j,newG[list[i]].size()){
            if(newG[list[i]][j]==-1) break;
            if(newG[list[i]][j]==list[i+1]){
                flag = true;
                break;
            }
        }
        if(flag) continue;
        else{
            newG[list[i]][size_vec[list[i]]] = list[i+1];
            newG[list[i+1]][size_vec[list[i+1]]] = list[i];
            size_vec[list[i]]++;
            size_vec[list[i+1]]++;
        }
    }

    //cout << "new_graph" << endl;

    //新しいグラフについて頂点をグループ化
    //幅優先探索
    vector<vector<int>> new_dist(N,vector<int>(N,-1));
    vector<int> new_seen(N,-1);//-1..not exist,0..false,1..true
    vector<vector<int>> new_group;
    rep(i,N){
        new_bfs(newG,size_vec[i],new_dist[i],i,new_seen);
    }
    priority_queue<pair<int,int>> count_city;
    rep(i,N){
        count_city.push({city_count[i],i});
    }
    int s = count_city.top().second;
    int now_A_size = 0;
    grouping(new_group,new_dist,new_seen,count_city,s,N,LA,LB,now_A_size);
    //seen[i]..-1 or 1
    //要素数の少ないグループに要素を付け加える
    if(now_A_size<LA){
        grouping2(new_group,new_dist,new_seen,N,LA,LB,now_A_size);
    }
    
    //グループをもとにAを構成
    int A_idx = 0;
    rep(i,new_group.size()){
        rep(j,new_group[i].size()){
            A[A_idx] = new_group[i][j];
            A_idx++;
        }
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

bool q_move(const vector<int> &long_path,int &idx,vector<int> &B,int &cnt){
    bool check = false;
    int next_one = long_path[idx+1];
    rep(i,B.size()){
        if(B[i]==next_one) check = true;
    }
    if(!check) return false;

    cout << "m" << " " << next_one << endl;
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

void q_switch(vector<int> &A,vector<int> &B,int &cnt,int &LA,int &LB,vector<int> &next,int &size_next){
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

    cout << "s" << " " << l << " " << PA << " " << PB << endl;
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

    //city_listの順で最短路を求める
    //pathの取り方どうするよ？
    map<pair<int,int>,vector<int>> path;
    vector<vector<pair<int,int>>> dist(N,vector<pair<int,int>>(N,{-1,-1}));
    int s = 0;
    int g = 0;

    rep(i,T){
        g = city_list[i];
        bfs(G,dist[g],g);
        make_path(path[{s,g}],dist[g],s,g);
        s = g;
    }

    //pathを一つにまとめる
    vector<int> long_path;
    s = 0;
    long_path.push_back(s);
    rep(i,city_list.size()){
        g = city_list[i];
        for(auto u : path[{s,g}]){
            if(u==long_path.back()) continue;
            long_path.push_back(u);
        }
        s = g;
    }
    int size_long_path = long_path.size();

    //cout << "long_path_length " << size_long_path << endl;

    /*
    rep(i,long_path.size()){
        cout << long_path[i] << " ";
    }
    cout << endl;
    */

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
    
    //cout << "cities " << onpath.size() << endl;

    //配列Aを決める
    vector<int> A = vector<int>(LA,0);
    make_A(A,LA,LB,long_path,size_long_path,N);
    rep(i,LA){
        cout << A[i] << " ";
    }
    cout << endl;
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
            if(!q_move(long_path,idx,B,cnt)) break;
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

        q_switch(A,B,cnt,LA,LB,next,size_next);
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
   //outfile.close();
}