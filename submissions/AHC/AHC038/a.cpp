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
using Graph = vvi;

/*ロボットアームをつくる*/
void design_arm(vpii &arm,vpii &parent_length,Graph &arm_graph,vector<bool> &is_leaf,const int N,const int V){
    int V_capacity = V;
    vi parent;
    vi edge_length;
    if(N==15||V==5){
        vi par = {0,1,2,3};
        vi edge_len = {8,4,2,1};
        rep(i,0,par.size()){
            parent.push_back(par[i]);
        }
        V_capacity -= 5;
        rep(i,0,edge_len.size()){
            edge_length.push_back(edge_len[i]);
        }
    }else if(N<=30||V>=6){
        vi par = {0,1,2,3,4};
        vi edge_len = {(N-1)/2,8,4,2,1};
        rep(i,0,par.size()){
            parent.push_back(par[i]);
        }
        V_capacity -= 6;
        rep(i,0,edge_len.size()){
            edge_length.push_back(edge_len[i]);
        }
    }
    int unit_len = parent.size();
    while(V_capacity>0){
        //cout << "V_cap:" << V_capacity << "unit_len:" << unit_len << endl;
        rep(i,0,min(unit_len,V_capacity)){
            if(i==0){
                parent.push_back(parent[i+(unit_len-min(unit_len,V_capacity))%unit_len]); 
            }else{
                parent.push_back(parent.size());
            }
            edge_length.push_back(edge_length[i+(unit_len-min(unit_len,V_capacity))%unit_len]);
        }
        V_capacity -= min(unit_len,V_capacity);
    }
    /*parent,edge_lengthからarmを構築*/
    arm[0] = {N/2,N/2};
    rep(i,0,V-1){
        parent_length[i] = {parent[i],edge_length[i]};
        arm_graph[i+1].push_back(parent[i]);
        arm_graph[parent[i]].push_back(i+1);
        arm[i+1] = {arm[parent[i]].first,arm[parent[i]].second + edge_length[i]};
        is_leaf[parent[i]] = false;
    }
}

void print_arm(vpii &parent_length,vpii &arm){
    cout << (parent_length.size() + 1) << endl;
    rep(i,0,parent_length.size()){
        cout << parent_length[i].first << " " << parent_length[i].second << endl;
    }
    cout << arm[0].first << " " << arm[0].second << endl;
}

/*そうさ1*/
void move_root(){

}
/*そうさ2*/
void sub_rotate(int u,vpii &arm,vpii &parent_length,Graph &arm_graph,vector<bool> &is_leaf,vector<bool> &is_holding){
    if(is_leaf[u]==true){
        
    }else{

    }
}

void rotate(vpii &arm,vpii &parent_length,Graph &arm_graph,vector<bool> &is_leaf,vector<bool> &is_holding){
    int u = 1;
    sub_rotate(u,arm,parent_length,arm_graph,is_leaf,is_holding);
}
/*そうさ3*/
void grab(){

}

int main()
{
    int N,M,V;
    cin >> N >> M >> V;
    vector<vector<char>> S(N,vector<char>(N)),T(N,vector<char>(N));
    vpii Stako,Ttako;//たこ焼きの座標
    rep(i,0,N){
        rep(j,0,N){
            cin >> S[i][j];
            if(S[i][j]=='1') Stako.push_back({i,j});
        }
    }
    rep(i,0,N){
        rep(j,0,N){
            cin >> T[i][j];
            if(T[i][j]=='1') Ttako.push_back({i,j});
        }
    }
    /*構築*/
    vpii arm(V);//頂点の座標
    vpii parent_length(V-1);//頂点の親と辺の長さ
    vector<bool> is_holding(V);//頂点がたこ焼きを掴んでいるか
    Graph arm_graph(V);//dfsのためのグラフ
    vector<bool> is_leaf(V,true);

    design_arm(arm,parent_length,arm_graph,is_leaf,N,V);
    print_arm(parent_length,arm);

    /*操作*/
    int nokori = 0;
    rep(i,0,N){
        rep(j,0,N){
            if(T[i][j]=='1'&&S[i][j]=='0'){
                nokori++;
            }
        }
    }

    while(nokori>0){
        /*dfsで全rotateを試す*/
        /*1マス移動をした上で改めてdfs*/

    }
}