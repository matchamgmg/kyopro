#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,W;
    cin >> N >> W;
    vector<ll> val(N);
    vector<int> wei(N);
    rep(i,N){
        cin >> wei[i] >> val[i];
    }
    vector<vector<ll>> dp(N+1,vector<ll>(W+1,0));
    //i番目の荷物の時点でのvalの最大値
    //添え字注意
    rep(i,N){
        rep(j,W+1){
            //荷物iを使う場合
            if(j-wei[i]>=0){
                if(dp[i+1][j]<dp[i][j-wei[i]]+val[i]){
                    dp[i+1][j] = dp[i][j-wei[i]]+val[i];
                }
            }
            //荷物iを使わない場合
            if(dp[i+1][j]<dp[i][j]){
                dp[i+1][j] = dp[i][j];
            }
            //cout << dp[i+1][j] << " ";
        }
        //cout << endl;
    }
    cout << dp[N][W] << endl;
}