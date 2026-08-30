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

int main()
{
    int N;
    int M = 0;
    cin >> N;
    priority_queue<soda,vector<soda>,greater<soda>> AB,BA;
    rep(i,0,N){
        ll a,b;
        cin >> a >> b;
        AB.push({a,b});
        BA.push({b,a});
    }
    set<soda> Soda;
    Soda.insert({0,0});

    vvll output;

    while(!(AB.empty()||BA.empty())){
        //cout << M << endl;
        while(!AB.empty()&&Soda.count(AB.top())){
            AB.pop();
        }
        soda kari;
        if(!BA.empty()){
            kari = BA.top();
        }
        while(!BA.empty()&&Soda.count({kari.second,kari.first})){
            BA.pop();
            kari = BA.top();
        }
        soda u,v;
        if(!AB.empty()) u = AB.top();
        if(!BA.empty()) v = {kari.second,kari.first};

        //作る炭酸飲料を決定する
        int ua,ub,va,vb;
        soda obj1 = {-1,-1};
        soda obj2 = {-1,-1};
        if(!AB.empty()&&!BA.empty()){
            ua = u.first;
            ub = u.second;
            va = v.first;
            vb = v.second;
            if(ua+ub<va+vb){
                obj2 = u;
            }else{
                obj2 = v;
            }
            if(min(ua,ub)<min(va,vb)){
                if(min(ua,ub)==0){
                    obj1 = {min(va,vb),min(va,vb)};
                }else{
                    obj1 = {min(ua,ub),min(ua,ub)};
                }
            }else{
                if(min(va,vb)==0){
                    obj1 = {min(ua,ub),min(ua,ub)};
                }else{
                    obj1 = {min(va,vb),min(va,vb)};
                }
            }
        }else if(!AB.empty()){
            obj2 = u;
            obj1 = {min(ua,ub),min(ua,ub)};
        }else if(!BA.empty()){
            obj2 = v;
            obj1 = {min(va,vb),min(va,vb)};
        }

        //cout << obj1.first << " " << obj1.second << endl;
        //cout << obj2.first << " " << obj2.second << endl;

        //obj1を作る
        if(!(obj1.first==-1&&obj1.second==-1)&&!Soda.count(obj1)){
            soda base;
            ll min_cost = INF;
            ll cost;
            for(soda s : Soda){
                cost = (obj1.first-s.first)+(obj1.second-s.second);
                if(s.first<=obj1.first&&s.second<=obj1.second&&
                min_cost>cost){
                    min_cost = cost;
                    base = s;
                }
            }
            vll op = {base.first,base.second,obj1.first,obj1.second};
            output.push_back(op);
            M++;
            Soda.insert(obj1);
        }

        //obj2を作る
        if(!(obj2.first==-1&&obj2.second==-1)&&!Soda.count(obj2)){
            soda base;
            ll min_cost = INF;
            ll cost;
            for(soda s : Soda){
                cost = (obj2.first-s.first)+(obj2.second-s.second);
                if(s.first<=obj2.first&&s.second<=obj2.second&&
                min_cost>cost){
                    min_cost = cost;
                    base = s;
                }
            }
            vll op = {base.first,base.second,obj2.first,obj2.second};
            output.push_back(op);
            M++;
            Soda.insert(obj2);
            if(obj2.first==ua&&obj2.second==ub){
                AB.pop();
            }else{
                BA.pop();
            }
        }else{
            if(obj2.first==ua&&obj2.second==ub){
                AB.pop();
            }else{
                BA.pop();
            }
        }
    }
    cout << M << endl;
    rep(i,0,M){
        rep(j,0,4){
            cout << output[i][j] << " ";
        }
        cout << endl;
    }
}