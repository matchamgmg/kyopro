#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

//最小コスト弾性マッチング
#define INF 1<<29

int main()
{
    int m,n;
    cin>>m>>n;
    /*
    vector<int> a(m),b(n);
    rep(i,m){
        cin>>a.at(i);
    }
    rep(i,n){
        cin>>b.at(i);
    }
    */
    vector<vector<int>> c(m,vector<int>(n));
    rep(i,m){
        rep(j,n){
            cin>>c.at(i).at(j);
        }
    }
    vector<vector<int>> dp(m+1,vector<int>(n+1,0));
    dp.at(0).at(0) = 0;
    rep(i,m){
        rep(j,n){
            dp.at(i+1).at(j+1) = min({dp.at(i).at(j),dp.at(i+1).at(j),dp.at(i).at(j+1)}) + c.at(i).at(j);
        }
    }
    cout << dp.at(m).at(n) << endl;
}