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
using soda = pair<ll,ll>;
#define INF 1<<29

void dfs(soda base,soda obj,set<soda> &Soda,set<ll> &As,set<ll> &Bs,priority_queue<soda,vector<soda>,greater<soda>> &AB,priority_queue<soda,vector<soda>,greater<soda>> &BA,set<soda> &list){
    if(Soda.count(obj)) return;
    soda mid = {-1,-1};
    if(base.first!=obj.first&&base.second!=obj.second){
        //A,Bどちらの方向に伸ばすか
        //先にコストの小さいほうに伸ばす
        if(obj.first-base.first<obj.second-base.second){
            mid = {obj.first,base.second};
        }else{
            mid = {base.first,obj.second};
        }
        if(!list.count(mid)){
            AB.push(mid);
            BA.push({mid.second,mid.first});
        }
        
    }else{
        if(base.first==obj.first){
            auto itr = Bs.upper_bound(base.second);
            if(*itr<obj.second){
                mid = {base.first,*itr};
                if(!list.count(mid)){
                    AB.push(mid);
                    BA.push({mid.second,mid.first});
                }
            }
        }else{
            auto itr = As.upper_bound(base.first);
            if(*itr<obj.first){
                mid = {*itr,base.second};
                if(!list.count(mid)){
                    AB.push(mid);
                    BA.push({mid.second,mid.first});
                }
            }
        }
    }
}

int main()
{
    int N;
    int M = 0;
    cin >> N;
    priority_queue<soda,vector<soda>,greater<soda>> AB,BA;
    map<ll,vector<soda>> list_A,list_B;
    set<soda> list;
    set<ll> As,Bs;
    rep(i,0,N){
        ll a,b;
        cin >> a >> b;
        AB.push({a,b});
        BA.push({b,a});
        list.insert({a,b});
        list_A[a].push_back({a,b});
        list_B[b].push_back({a,b});
        As.insert(a);
        Bs.insert(b);
    }
    set<soda> Soda;
    Soda.insert({0,0});

    vvll output;
    while(!(AB.empty()||BA.empty())){
        //cout << M << endl;
        //まだ作ってない炭酸水を一つずつ取り出す
        while(!AB.empty()&&Soda.count(AB.top())){
            AB.pop();
            //cout << AB.size() << endl;
        }
        //cout << "bbb" << endl;
        soda kari;
        if(!BA.empty()){
            kari = BA.top();
        }
        while(!BA.empty()&&Soda.count({kari.second,kari.first})){
            BA.pop();
            kari = BA.top();
        }
        //cout << "aaa" << endl;
        soda u = {-1,-1};
        soda v = {-1,-1};
        if(!AB.empty()) u = AB.top();
        if(!BA.empty()) v = {kari.second,kari.first};
        ll ua,ub,va,vb;
        ua = u.first;
        ub = u.second;
        va = v.first;
        vb = v.second;
        //minの小さいほう
        soda obj = {-1,-1};
        if(min(ua,ub)<min(va,vb)){
            if(min(ua,ub)!=-1){
                obj = u;
            }else{
                obj = v;
            }
        }else{
            if(min(va,vb)!=-1){
                obj = v;
            }else{
                obj = u;
            }
        }
        if(obj.first==-1&&obj.second==-1) continue;
        //midを作る必要があるか調べる
        //baseを決定する
        soda base = {-1,-1};
        ll min_cost = INF;
        ll cost;
        for(soda s : Soda){
            cost = (obj.first-s.first)+(obj.second-s.second);
            if(s.first<=obj.first&&s.second<=obj.second&&cost<min_cost){
                min_cost = cost;
                base = s;
            }
        }
        dfs(base,obj,Soda,As,Bs,AB,BA,list);
        //midを作る
        if(AB.top()!=u){
            soda mid = AB.top();
            if(!Soda.count(mid)){
                vll op = {base.first,base.second,mid.first,mid.second};
                output.push_back(op);
                M++;
                AB.pop();
                Soda.insert(mid);
            }
        }
        if(BA.top()!=kari){
            soda mid = BA.top();
            if(!Soda.count(mid)){
                vll op = {base.first,base.second,mid.first,mid.second};
                output.push_back(op);
                M++;
                BA.pop();
                Soda.insert(mid);
            }
        }
        //objを作る
        base = {-1,-1};
        min_cost = INF;
        cost;
        for(soda s : Soda){
            cost = (obj.first-s.first)+(obj.second-s.second);
            if(s.first<=obj.first&&s.second<=obj.second&&cost<min_cost){
                min_cost = cost;
                base = s;
            }
        }
        vll op = {base.first,base.second,obj.first,obj.second};
        output.push_back(op);
        M++;
        if(obj==u){
            AB.pop();
        }else{
            BA.pop();
        }
        Soda.insert(obj);
    }
    cout << M << endl;
    rep(i,0,M){
        rep(j,0,4){
            cout << output[i][j] << " ";
        }
        cout << endl;
    }
}