#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 1'000'000'007

int main()
{
    int N,L;
    cin >> N >> L;
    vector<int> dp(N+1,0);
    dp[0] = 1;
    rep(i,N){
        if(i+1-L>=0) dp[i+1] = (dp[i+1] + dp[i+1-L]) % MOD;
        dp[i+1] = (dp[i+1] + dp[i]) % MOD;
    }
    cout << dp[N] << endl;
}