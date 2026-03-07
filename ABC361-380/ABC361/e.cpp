#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<vector<pair<int,ll>>> Graph(N,vector<pair<int,ll>>());
    rep(i,N-1){
        int A,B;
        ll C;
        cin >> A >> B >> C;
        Graph[A-1].push_back({B-1,C});
        Graph[B-1].push_back({A-1,C});
    }
    vector<ll> cost(N,1<<62); 
    stack<int> stk;
    int s = 0;
    stk.push(s);
    cost()
    while(!stk.empty()){
        int u = stk.top();
        stk.pop();
        for(auto v : Graph[u]){
            stk.push(v.first);
        }
    }
}