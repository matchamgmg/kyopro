#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,M;
    cin >> N >> M;
    map<int,int>edge;
    int a,b;
    rep(i,M){
        cin >> a >> b;
        if(a>b){
            edge[a]++;
        }else if(a<b){
            edge[b]++;
        }
    }
    int cnt = 0;
    for(auto e:edge){
        if(e.second==1)cnt++;
    }
    cout << cnt << endl;
}