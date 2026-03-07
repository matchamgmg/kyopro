#include <bits/stdc++.h>
#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 1'000'000'007

int main()
{
    int N;
    string S;
    cin >> N >> S;
    //文字列DP
    //a at atc atco atcod atcode atcoder
    vector<char> s = {'a','t','c','o','d','e','r'};
    vector<vector<ll>> dp(N+1,vector<ll>(7,0));
    rep(i,0,N){
        rep(j,0,7){
            if(S[i]==s[j]){
                if(j-1>=0){
                    if(dp[i][j-1]==0) continue;
                    else{
                        dp[i+1][j] = dp[i][j] + dp[i+1][j-1];
                        dp[i+1][j] %= MOD;
                    }
                }else{
                    dp[i+1][j] = dp[i][j] + 1;
                    dp[i+1][j] %= MOD;
                }
            }else{
                dp[i+1][j] = dp[i][j];
            }
        }
    }
    cout << dp[N][6] << endl;
}