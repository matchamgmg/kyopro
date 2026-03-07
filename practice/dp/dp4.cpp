#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

//最小個数部分和問題
//K個以内部分和問題
#define INF 1<<29

int main()
{
    int n,A,K;
    cin>>n>>A>>K;
    vector<int> a(n);
    rep(i,n){
        cin>>a.at(i);
    }
    vector<vector<int>> dp(n+1,vector<int>(A+1,INF));
    dp.at(0).at(0) = 0;
    rep(i,n){
        rep(j,A+1){
            dp.at(i+1).at(j) = min(dp.at(i+1).at(j),dp.at(i).at(j));
            if(j-a.at(i)>=0){
                dp.at(i+1).at(j) = min(dp.at(i+1).at(j),dp.at(i).at(j-a.at(i))+1);
            }
        }
    }
    cout << (dp.at(n).at(A)==INF?-1:dp.at(n).at(A)) << endl;
    cout << (dp.at(n).at(A)<=K?"Yes":"No") << endl;
}