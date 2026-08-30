#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

//最長共通部分列(LCS)問題

int main()
{
    string S,T;
    cin >> S >> T;
    int s = S.size(),t = T.size();
    vector<vector<int>> dp(s+1,vector<int>(t+1,0));
    rep(i,s){
        rep(j,t){
            if(S.at(i) == T.at(j)){
                dp.at(i+1).at(j+1) = max({dp.at(i).at(j)+1,dp.at(i+1).at(j),dp.at(i).at(j+1)});
            }else{
                dp.at(i+1).at(j+1) = max(dp.at(i+1).at(j),dp.at(i).at(j+1));
            }
        }
    }
    cout << dp.at(s).at(t) << endl;
}