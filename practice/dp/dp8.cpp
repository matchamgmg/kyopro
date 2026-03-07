#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

//レーベンシュタイン距離(diffコマンド)
#define INF 1<<29

int main()
{
    string S,T;
    cin>>S>>T;
    int s=S.size(),t=T.size();
    vector<vector<int>> dp(s+1,vector<int>(t+1,INF));
    dp.at(0).at(0) = 0;
    rep(i,s){
        rep(j,t){
            if(S.at(i)==T.at(j)){
                dp.at(i+1).at(j+1) = min({dp.at(i).at(j),dp.at(i+1).at(j)+1,dp.at(i).at(j+1)+1});
            }else{
                dp.at(i+1).at(j+1) = min({dp.at(i).at(j)+1,dp.at(i+1).at(j)+1,dp.at(i).at(j+1)+1});
            }
        }
    }
    cout << dp.at(s).at(t) << endl;
}