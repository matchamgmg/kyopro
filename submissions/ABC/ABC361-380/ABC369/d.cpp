#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll N;
    cin >> N;
    vector<ll> A(N);
    rep(i,N){
        cin >> A[i];
    }
    vector<vector<pair<ll,ll>>> dp(N+1,vector<pair<ll,ll>>(2,{0,2}));
    //i番目の敵を倒したor見逃した後の最大値
    rep(i,N){
        //i番目をたおす
        ll dp_i_0_f = dp[i][0].first + (3 - dp[i][0].second)*A[i];
        ll dp_i_1_f = dp[i][1].first + (3 - dp[i][1].second)*A[i];
        if(dp_i_0_f > dp_i_1_f){
            dp[i+1][0] = {dp_i_0_f,(3-dp[i][0].second)};
        }else if(dp_i_0_f < dp_i_1_f){
            dp[i+1][0] = {dp_i_1_f,(3-dp[i][1].second)};
        }else{
            if(dp[i][0].second==2){
                dp[i+1][0] = {dp_i_0_f,(3-dp[i][0].second)};
            }else{
                dp[i+1][0] = {dp_i_1_f,(3-dp[i][1].second)};
            }
        }
        //i番目を見逃す
        dp_i_0_f = dp[i][0].first;
        dp_i_1_f = dp[i][1].first;
        if(dp_i_0_f > dp_i_1_f){
            dp[i+1][1] = {dp_i_0_f,dp[i][0].second};
        }else if(dp_i_0_f < dp_i_1_f){
            dp[i+1][1] = {dp_i_1_f,dp[i][1].second};
        }else{
            if(dp[i][0].second==1){
                dp[i+1][1] = {dp_i_0_f,dp[i][0].second};
            }else{
                dp[i+1][1] = {dp_i_1_f,dp[i][1].second};
            }
        }
    }
    if(dp[N][0].first > dp[N][1].first){
        cout << dp[N][0].first << endl;
    }else{
        cout << dp[N][1] .first << endl;
    }
}