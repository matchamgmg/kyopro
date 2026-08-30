#include <bits/stdc++.h>
#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define INF 1LL<<60

int main()
{
    int N;
    ll W;
    cin >> N >> W;
    vector<ll> wei(N);
    vector<int> val(N);
    int val_sum = 0;
    rep(i,0,N){
        cin >> wei[i] >> val[i];
        val_sum += val[i];
    }
    vector<vector<ll>> dp(N+1,vector<ll>(val_sum+1,INF));
    dp[0][0] = 0;
    //dp[i][j]...荷物i,価値の総和jとなる重さの総和の最小値
    rep(i,1,N+1){
        rep(j,0,val_sum+1){
            //荷物iを使うとき
            if(j-val[i-1]>=0){
                dp[i][j] = min(dp[i-1][j],dp[i-1][j-val[i-1]]+wei[i-1]);
            }else{
                dp[i][j] = dp[i-1][j];
            }
            //if(dp[i][j]>=INF) cout << "INF" << " ";
            //else cout << dp[i][j] << " ";
        }
        //cout << endl;
    }
    int ans = val_sum;
    while(dp[N][ans]>W) ans--;
    cout << ans << endl;
}    